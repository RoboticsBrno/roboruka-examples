function Manager(logElementId) {
    this.socket = null;
    this.joysticks = [];
    this.arm = null;

    this.divider = 0;

    this.mustArriveIdIn = 0;
    this.mustArriveIdOut = 0;
    this.mustArriveCommands = {};
    this.MUST_ARRIVE_TIMER_FULL = 50;
    this.MUST_ARRIVE_RETRIES = 15;
    this.mustArriveTimer = this.MUST_ARRIVE_TIMER_FULL;

    this.log = new Log(logElementId);
}

Manager.prototype.addJoystick = function(joy) {
    this.joysticks.push(joy);
}

Manager.prototype.createArm = function() {
    if(this.arm !== null)
        return;
    if(this.socket && this.socket.readyState === WebSocket.OPEN) {
        this.sendMustArrive("arminfo", {});
    }
    setTimeout(this.createArm.bind(this), 500);
}

Manager.prototype.start = function(address) {
    this.log.write("Connecting to " + address + "... ", true);

    if(!('WebSocket' in window)) {
        this.log.write("\nWebSockets are not supported on this device!");
        return
    }

    this.socket = new ReconnectingWebSocket(address);
    this.socket.addEventListener('open', function (event) {
        this.log.write("connected!")
        this.log.write("Attempting to possess the robot...")
        this.sendMustArrive("possess", {}, true);
    }.bind(this));

    this.socket.addEventListener('error', function(event) {
        this.log.write("Connection FAILED!")
    }.bind(this));

    this.socket.addEventListener('message', this.onMessage.bind(this));

    this.lastUpdate = Date.now();
    requestAnimationFrame(this.update.bind(this));
}

Manager.prototype.update = function() {
    if(++this.divider >= 2) {
        this.divider = 0;
    } else {
        requestAnimationFrame(this.update.bind(this));
        return;
    }

    var now = Date.now();
    var diff = (now - this.lastUpdate);
    this.lastUpdate = now;

    if(this.socket.readyState === WebSocket.OPEN && this.arm) {
        var data = []
        for(var i = 0; i < this.joysticks.length; ++i) {
            data.push({
                "x": this.joysticks[i].x,
                "y": this.joysticks[i].y,
            })
        }
        this.socket.send(JSON.stringify({ "c": "joy", "data": data }))

        if(this.arm && this.arm.shouldSend()) {
            var pos = this.arm.getTargetPos();
            pos["c"] = "arm";
            this.socket.send(JSON.stringify(pos));
        }
    }

    if(diff >= this.mustArriveTimer) {
        for (var id in this.mustArriveCommands) {
            if (this.mustArriveCommands.hasOwnProperty(id)) {
                var info = this.mustArriveCommands[id];
                this.socket.send(info.payload);
                if(info.attempts !== null) {
                    if(++info.attempts >= this.MUST_ARRIVE_RETRIES) {
                        delete this.mustArriveCommands[id];
                    }
                }
            }
        }
        this.mustArriveTimer = this.MUST_ARRIVE_TIMER_FULL;
    } else {
        this.mustArriveTimer -= diff;
    }

    requestAnimationFrame(this.update.bind(this))
}

Manager.prototype.onMessage = function(event) {
    var data = JSON.parse(event.data);
    if("f" in data) {
        var cmd = this.mustArriveCommands[data["f"]];
        if (cmd !== undefined) {
            delete this.mustArriveCommands[data["f"]];
            if(cmd["callback"] !== undefined) {
                cmd["callback"]();
            }
        }
        return;
    } else if("e" in data) {
        this.socket.send(JSON.stringify({"c": data["c"], "e": data["e"]}));
        if(data["e"] <= this.mustArriveIdIn) {
            return;
        } else {
            this.mustArriveIdIn = data["e"];
        }
    }

    switch(data["c"]) {
    case "pong":
        break;
    case "log":
        this.log.write(data["msg"]);
        break;
    case "arminfo":
        this.arm = new Arm(data, "arm0", this);
        break;
    }
}

Manager.prototype.sendMustArrive = function(command, data, unlimitedAttempts, callback) {
    var id = ++this.mustArriveIdOut;
    data["c"] = command;
    data["f"] = id;

    var payload = JSON.stringify(data);
    this.mustArriveCommands[id] = {
        "payload": payload,
        "attempts": (unlimitedAttempts !== true) ? 0 : null,
        "callback": callback,
    };
    this.socket.send(payload);
}

Manager.prototype.flashBody = function() {
    var body = document.getElementById("body");
    body.style.backgroundColor = "#ff5454";
    setTimeout(function() {
        body.style.backgroundColor = "white";
    }, 50);
}
