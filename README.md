# Breakout

> A simple Breakout copy, made with C++ using SFML. The project uses CMake for the buildsystem.

## About
The aim of this project, is to learn and have fun with C++
* **Standard_** C++23 
* **Build System:** CMake 3.30+

### Features
    * Simple Breakout gameplay.
    * Sound.

## How to build 
* Clone the repository using the recursive flag.
```bash
git clone --recursive https://github.com/SebastianM0eller/Breakout
cd Breakout
```

*Build the game using the appropriate CMake preset.
```bash
# If you build for linux:
cmake --preset release
cmake --build --preset release

# If you build for Windows:
cmake --preset windows-release
cmake --build --preset windows-release
```

### How to Play
* Run the executable, located at build/preset/bin/
    * Spawn the ball, using 'W' or 'Up Key'.
    * Move the paddle, using 'A, D' or 'Left, Right Key'.
