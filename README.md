# Enigma Cipher Machine Simulator

A software simulation of the historic **Enigma cipher machine** used in World War II, built using **C** for core logic and **Python (Tkinter)** for the GUI.

## About

The Enigma machine was used by Nazi Germany to encrypt military communications. It worked through a series of rotating wheels (rotors), a reflector, and a plugboard to scramble messages. This project replicates that mechanism in code, with a modified algorithm for improved strength.

## Features

- Encrypt and decrypt messages via rotor-based logic
- Configurable rotor positions (A–Z)
- Interactive Python GUI for visual simulation
- CLI version in C for direct terminal use

## How to Run

**C (Terminal)**
```bash
gcc main.c -o enigma
./enigma
```

**Python GUI**
```bash
python enigma_gui_full.py
```

## Sample Output

```
=== ENIGMA MACHINE SIMULATION ===
Rotor I position: 0  |  Rotor II: 2  |  Rotor III: 4

Enter message: HELLO
Ciphertext:    ORAWV
```

## Team

| Name | Contribution |
|------|-------------|
| Prabanjan | Research & Enigma algorithm study |
| Ruchitha | Core encryption/decryption in C |
| Thanuja | Python GUI design & implementation |

## Future Work

Implement **Alan Turing's Bombe Machine** in C — the legendary device that cracked the Enigma cipher during WWII.

