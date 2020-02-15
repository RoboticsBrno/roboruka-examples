function Joystick(elementId, color, keys, buttonText, buttonClickHandler) {
    var zone = document.getElementById(elementId);

    this.radius = zone.offsetWidth/1.7 / 2;
    this.x = 0;
    this.y = 0;
    this.buttonClickHandler = buttonClickHandler;
    this.touchStart = null;
    this.keys = keys;
    this.pressedKeys = {};

    document.addEventListener("keydown", this.onKeyDown.bind(this));
    document.addEventListener("keyup", this.onKeyUp.bind(this));

    this.manager = nipplejs.create({
        zone: zone,
        mode: "static",
        color: color,
        size: this.radius*2,
        position: {
            "top": "50%",
            "left": "50%",
        },
        restOpacity: 0.9,
        fadeTime: 0,
        lockX: !!buttonText,
        lockY: !!buttonText,
    });

    this.joy = this.manager.get(this.manager.id);

    if(buttonText) {
        var nipple = this.joy.ui.front;
        nipple.innerHTML = buttonText;
        nipple.style.fontWeight = "bold";
        nipple.style.fontFamily = "Arial, Helvetica, sans-serif";
        nipple.style.color = "white";
        nipple.style.textAlign = "center";
        nipple.style.verticalAlign = "middle";
        nipple.style.lineHeight = nipple.style.height;
    }

    this.manager.on("move", function(event, data) {
        var dist = data.distance/(this.radius)*32767;
        this.x = Math.cos(data.angle.radian)*dist | 0;
        this.y = Math.sin(data.angle.radian)*dist | 0;
    }.bind(this));

    this.manager.on("start", function(event, data) {
        this.touchStart = Date.now();
        this.x = 0;
        this.y = 0;
    }.bind(this));

    this.manager.on("end", function(event, data) {
        var diff = Date.now() - this.touchStart;
        if(this.buttonClickHandler && diff < 150 && Math.abs(this.x) < 8000 && Math.abs(this.y) < 8000) {
            this.buttonClickHandler();
        }

        this.x = 0;
        this.y = 0;
    }.bind(this));
}

Joystick.prototype.onKeyDown = function(ev) {
    var idx = this.keys.indexOf(ev.key);
    if(idx === -1)
        return;
    if(idx === 4) {
        this.buttonClickHandler();
    }
    this.pressedKeys[idx] = true;
    this.updateNippleFront();
}

Joystick.prototype.onKeyUp = function(ev) {
    var idx = this.keys.indexOf(ev.key);
    if(idx === -1)
        return;
    this.pressedKeys[idx] = false;
    this.updateNippleFront();
}

Joystick.prototype.updateNippleFront = function() {
    var p = this.pressedKeys;
    var left = 0;
    var top = 0;
    if(p[0] === true)
        top -= 1;
    if(p[2] ===  true)
        top += 1;
    if(p[1] === true)
        left -= 1;
    if(p[3] === true)
        left += 1;

    this.x = left*28000;
    this.y = top*-28000;

    var fr = this.joy.ui.front;
    fr.style.left = (left*this.radius) + "px";
    fr.style.top = (top*this.radius) + "px";
}
