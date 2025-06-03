# Minty the Hexapod

**USYD MTRX5700 Major Project**

> Hexapod kinematics and dynamics library, deployed on a customised version of the [MakeYourPet "Chipo"](https://github.com/MakeYourPet/hexapod) hexapod.

![Project Demo](link-to-image-or-gif)  
[![Watch the Demo](https://img.shields.io/badge/Watch-Demo-blue)](link-to-video)

---

## 🚀 Features

- ✅ Independent longitudinal, lateral, and rotational velocity control
- ✅ Dynamically adjustable gait, step size, and stance height
- ✅ Reconfigurable for deployment on any hexapod

---

## 🔩 Hardware

| Component            | Description              |
|---------------------|--------------------------|
| Microcontroller      | Raspberry Pi 5     |
| Servomotor Controller | Pimoroni Servo2040 |
| Servomotors         | MG996R              |
| Touch Sensing        | Microswitches      |

See the [official repository](https://github.com/MakeYourPet/hexapod) for hardware STL files and wiring schematics.

---

## 🧠 Software Architecture

- **Languages**: C++, MicroPython
- **Key Modules**:
  - `brain`: coordinates leg actuation with a central pattern generation
  - `legs`: calculates inverse kinematics to actuate movement instructions
  - `controller`: provides command-line interface for remote control
  - `state_transmitter`: communicates the hexapod state to the servocontroller via serial

---

## 📄 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

## 🙌 Credits

- Angus – [@TheFlamingBadger](https://github.com/TheFlamingBadger)
- Prithik – [@psai8449](https://github.com/psai8449)
- Siddarth – [@sram8836](https://github.com/sram8836)
- Harry – [@hkerms](https://github.com/hkerms)
