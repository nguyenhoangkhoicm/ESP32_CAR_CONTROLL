var socket = new WebSocket("ws://192.168.120.225/ws");

function sendCommand(command) {
  socket.send(command);
}

document.getElementById("forward").addEventListener("click", function () {
  sendCommand("forward");
});

document.getElementById("backward").addEventListener("click", function () {
  sendCommand("backward");
});

document.getElementById("left").addEventListener("click", function () {
  sendCommand("left");
});

document.getElementById("right").addEventListener("click", function () {
  sendCommand("right");
});

document.getElementById("stop").addEventListener("click", function () {
  sendCommand("stop");
});

// document.getElementById("speedBtn").addEventListener("click", function() {
//   var speed = document.getElementById("speedInput").value;
//   var command = "speed:" + speed;
//   sendCommand(command);
// });
