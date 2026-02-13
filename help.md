# Ghid Construire ROV cu Arduino și Raspberry Pi

Salut! Iată un fișier `help.md` complet, structurat ca un ghid pas-cu-pas pentru lucrarea ta de licență despre construirea unui Remotely Operated Vehicle (ROV) subacvatic folosind Arduino pentru controlul motoarelor și Raspberry Pi pentru procesare video și control wireless. [instructables](https://www.instructables.com/Raspberry-Pi-Rover-simple-Using-Rpi-Arduino/)

## Componente Necesare
- **Procesor principal**: Raspberry Pi 4/Zero W (pentru streaming video și interfață web). [youtube](https://www.youtube.com/watch?v=IoCaTgmUSvc)
- **Controler low-level**: Arduino Uno/Mega (pentru drivere motoare și senzori). [instructables](https://www.instructables.com/Raspberry-Pi-Rover-simple-Using-Rpi-Arduino/)
- **Motoare**: 4-6 thruster-e brushless (ex: 6V DC pentru propulsie). [youtube](https://www.youtube.com/watch?v=IoCaTgmUSvc)
- **Camere**: Raspberry Pi Camera Module pentru video FPV subacvatic. [youtube](https://www.youtube.com/watch?v=xXPwo44QuQ8)
- **Drivere**: L298N sau ESC-uri pentru motoare; buck converter pentru alimentare stabilă (5V/12V). [instructables](https://www.instructables.com/Raspberry-Pi-Rover-simple-Using-Rpi-Arduino/)
- **Cadru**: Structură waterproof din PVC/acrilic, cu flotație (ex: tuburi de spumă). [instructables](https://www.instructables.com/Build-an-Outdoor-Rover-With-a-Raspberry-Pi-Simple-/)
- **Senzori**: Presiune (pentru adâncime), IMU (MPU6050), buzzer pentru feedback. [youtube](https://www.youtube.com/watch?v=tIdLlfjGr40)
- **Alimentare**: Baterie LiPo 11.1V-14.8V cu XT60 conectori; regulator la 5V pentru Pi/Arduino. [youtube](https://www.youtube.com/watch?v=IoCaTgmUSvc)
- **Accesorii**: Wifi dongle (dacă Pi fără WiFi), jumper wires, PCB proto, etanșeizare silicon.

## Arhitectură Sistem
ROV-ul folosește Raspberry Pi ca "creier" pentru procesare video (OpenCV) și control via web/app mobil. Arduino gestionează motoarele prin PWM și comunică cu Pi via serial (USB/UART). Exemplu flux: Comandă web → Pi trimite serial 'F' (forward) → Arduino activează thruster-e. [instructables](https://www.instructables.com/Raspberry-Pi-Rover-simple-Using-Rpi-Arduino/)

## Configurare Software
### Pe Raspberry Pi
1. Instalează Raspbian OS (Lite recomandat).
2. Activează SSH/VNC: `sudo raspi-config` → Interfacing Options.
3. Instalează dependințe: `sudo apt update && sudo apt install python3-pip tightvncserver wiringpi`.
4. Pentru video: `sudo raspi-config` → Camera → Enable; instalează `libcamera` sau Picamera2.
5. Script Python principal (Flask web server):
   ```python
   from flask import Flask, render_template
   import serial, cv2
   app = Flask(__name__)
   ser = serial.Serial('/dev/ttyACM0', 9600)  # Arduino port

   @app.route('/')
   def index(): return render_template('control.html')  # Joystick web

   @app.route('/<cmd>')
   def control(cmd):
       ser.write(cmd.encode())  # Trimite 'F','B','L','R' către Arduino
       return 'OK'
   ```
   Rulează cu `python3 app.py`. [youtube](https://www.youtube.com/watch?v=xXPwo44QuQ8)

### Pe Arduino
Încarcă sketch via Arduino IDE:
```cpp
#include <Servo.h>  // Pentru ESC-uri
Servo thruster1, thruster2;  // Exemplu 2 thruster-e
void setup() {
  Serial.begin(9600);
  thruster1.attach(9); thruster2.attach(10);
}
void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();
    if (cmd == 'F') { thruster1.write(150); thruster2.write(150); }  // Forward
    else if (cmd == 'S') { thruster1.write(90); thruster2.write(90); }  // Stop
    // Adaugă L/R/B, limitează la 1000-2000us PWM pentru ESC
  }
}
```
Testează cu Serial Monitor: 'f' forward, 's' stop. [instructables](https://www.instructables.com/Raspberry-Pi-Rover-simple-Using-Rpi-Arduino/)

## Montaj Hardware
1. Construiește cadrul: 3D print/PVC box waterproof (calibrate flotație pentru neutral buoyancy).
2. Conectează Arduino: Motoare la L298N (IN1-IN4 la pini digitali, PWM la 3/5/6/11).
3. Leagă Pi de Arduino: USB pentru serial; GPIO pentru senzori extra.
4. Alimentare: Baterie → DC-DC buck (12V→5V pentru Pi); separă circuite pentru siguranță.
5. Camere: Montată frontală în dome waterproof; stream MJPEG la `http://pi-ip:8080`.
6. Test terestru: Verifică etanșeitate în cadă înainte de scufundare. [instructables](https://www.instructables.com/Build-an-Outdoor-Rover-With-a-Raspberry-Pi-Simple-/)

## Testare și Debugging
- **Nivel 1**: Test motors Arduino solo (potentiometre pe drivere).
- **Nivel 2**: Comunicare Pi-Arduino (echo comenzi serial).
- **Nivel 3**: Control web + video stream (accesează Pi IP din browser).
- Probleme comune: Serial baud mismatch (9600 fix); supracurent (adaugă fuses); lag video (optimizează rezoluție 640x480). [youtube](https://www.youtube.com/watch?v=Q0zFEfsovds)
- Metrici performanță: Viteză max (m/s), adâncime max (5-10m), autonomie baterie (30min).

## Extensii Avansate
- Navigație autonomă: OpenCV pentru line following subacvatic.
- Senzori: Sonar (HC-SR04 adaptat), LED-uri pentru vizibilitate.
- Control app: WebSocket pentru joystick real-time.
- Siguranță: Tether Ethernet pentru control stabil, kill-switch. [youtube](https://www.youtube.com/watch?v=tIdLlfjGr40)

Adaugă diagrame (ex: Fritzing pentru wiring) și cod complet în anexe. Succes la licență! Dacă vrei modificări, spune. [youtube](https://www.youtube.com/watch?v=IoCaTgmUSvc)
