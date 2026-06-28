#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "CarBot";
const char* password = "12345678";

ESP8266WebServer server(80);

// Motor Pins
#define IN1 5   // D1
#define IN2 4   // D2
#define IN3 0   // D3
#define IN4 2   // D4
#define ENA 14  // D5
#define ENB 12  // D6

#define SPEED 128  // 50% speed (0-255)

void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

void forward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void backward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void turnRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html><html>
<head>
  <title>CarBot</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; }
    body {
      font-family: Arial;
      background: #111;
      color: white;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      height: 100vh;
    }
    h2 { margin-bottom: 30px; font-size: 24px; }
    button {
      width: 90px;
      height: 90px;
      font-size: 32px;
      margin: 8px;
      border-radius: 18px;
      background: #222;
      color: white;
      border: 2px solid #444;
      cursor: pointer;
      user-select: none;
      -webkit-user-select: none;
      touch-action: none;
    }
    button:active { background: #0077ff; border-color: #0077ff; }
    .row {
      display: flex;
      justify-content: center;
    }
    .stop-btn { background: #440000; border-color: #880000; }
  </style>
</head>
<body>
  <h2>🚗 CarBot Control</h2>
  <div class="row">
    <button id="F">⬆️</button>
  </div>
  <div class="row">
    <button id="L">⬅️</button>
    <button class="stop-btn" id="S">⏹️</button>
    <button id="R">➡️</button>
  </div>
  <div class="row">
    <button id="B">⬇️</button>
  </div>

  <script>
    const buttons = document.querySelectorAll('button');

    buttons.forEach(btn => {
      // Mouse events
      btn.addEventListener('mousedown', () => send(btn.id));
      btn.addEventListener('mouseup', () => send('S'));
      btn.addEventListener('mouseleave', () => send('S'));

      // Touch events
      btn.addEventListener('touchstart', (e) => {
        e.preventDefault();
        send(btn.id);
      });
      btn.addEventListener('touchend', (e) => {
        e.preventDefault();
        send('S');
      });
    });

    function send(cmd) {
      fetch('/cmd?dir=' + cmd).catch(() => {});
    }
  </script>
</body>
</html>
)rawliteral";
  server.send(200, "text/html", html);
}

void handleCmd() {
  if (server.hasArg("dir")) {
    String dir = server.arg("dir");
    if      (dir == "F") forward();
    else if (dir == "B") backward();
    else if (dir == "R") turnRight();
    else if (dir == "L") turnLeft();
    else                 stopMotors();
  }
  server.send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);

  // Set 50% speed
  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);

  stopMotors();

  // Create WiFi hotspot
  WiFi.softAP(ssid, password);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/cmd", handleCmd);
  server.begin();
  Serial.println("Server started!");
}

void loop() {
  server.handleClient();
}