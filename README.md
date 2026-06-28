# 🌋 ASHCORE

An open-source, high-performance hardcore survival game written in pure **C++** using the **Raylib** library. Inspired by classic top-down survival mechanics (like doomed.io), built for ultra-responsive gameplay, lag-free physics, and intense PvP.

---

## 🚀 Current Status: Alpha v0.3.0

The project is evolving rapidly from a basic prototype into a deep, playable survival framework.

### 🎮 Implemented Features:
* **The Golden Age (New!):** Added **Gold ore** generation into the world, introducing a new resource tier for future high-level progression, trading, and crafting.
* **Bug Fixes & Stability (New!):** Crushed multiple minor and major bugs related to player physics, object hitboxes, and rendering anomalies.
* **Core Mechanics & Smooth Animations:** Reworked player movement and tool/weapon swinging. Item tracking and jumping are now calculated smoothly using **sinusoidal functions** instead of instant teleportation.
* **Advanced Collision Engine (New!):** Overhauled physics to support true collision handling. Movement handles boundaries dynamically without jerky teleports.
* **Prototype Biomes:** Added a basic conceptual layout for multiple world zones (Forest, Desert, Snow, Wasteland, and Volcano). Currently represented as colored zones to lay the groundwork for procedural world generation.
* **Player Attributes:** Implemented **Health (HP)** and **Stamina** systems, essential for survival and combat balance.
* **Interaction System:** Basic attack animations, object hitboxes, and hand-to-object harvesting.
* **Resource Gathering:** Dynamic spawning of world objects (Trees, Rocks, and Gold veins) with a complete destructibility/harvesting system.
* **Inventory & Progression:** Basic resource items (**Wood**, **Stone**, **Gold**) and a multi-tier tool upgrade system (Axes, Pickaxes).

---

## 📸 Screenshots

### Gameplay Screenshots
![Early Prototype 1](images/prototype1.jpg)
![Early Prototype 2](images/prototype2.jpg)
![Early Prototype 3](images/prototype3.jpg)
![Early Prototype 4](images/prototype4.jpg)

---

## 🛠️ How to Build & Run

### Dependencies
Make sure you have a working C++ compiler, **CMake**, and the **Raylib** development packages installed on your system. Code style is enforced using `.clang-format`.

```bash
mkdir build && cd build
cmake ..
make
./ASHCORE
