window.addEventListener("load", function(){
    var man = new Manager("log");
    man.addJoystick(new Joystick("joy0", "blue", "wasd"));
    man.start("ws://localhost:9000");
    man.createArm();
});
