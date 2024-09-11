# Logic Circuit Simulator

Logic Circuit Simulator is a C++ application that allows users to design and simulate digital logic circuits. Built with raylib, this interactive tool provides a user-friendly interface for creating and testing various logic gates and circuits.

## Features

- Interactive placement of logic gates (AND, OR, NOT)
- Input switches for circuit testing
- Wire connections between components
- Real-time circuit simulation
- Grid-based component placement
- Debug information display

## Prerequisites

To build and run this project, you'll need:

- C++17 compatible compiler
- CMake (version 3.10 or higher)
- raylib (automatically fetched by CMake)

## Building and Running

1. Clone the repository:
   ```
   git clone https://github.com/yourusername/LogicCircuitSimulator.git
   cd LogicCircuitSimulator
   ```

2. Create a build directory and run CMake:
   ```
   mkdir build
   cd build
   cmake ..
   ```

3. Build the project:
   ```
   cmake --build .
   ```

4. Run the application:
   ```
   ./dist/LogicCircuitSimulator
   ```

## Usage

- Use the mouse to place and connect components
- Press 'A' to select AND gate
- Press 'O' to select OR gate
- Press 'N' to select NOT gate
- Press 'I' to select Input Switch
- Left-click to place a component or start/end a wire connection
- Right-click to cancel wire placement
- Press 'D' to toggle debug information display

## Project Structure

```
LogicCircuitSimulator/
├── assets/
│   ├── and_gate.svg
│   ├── not_gate.svg
│   └── or_gate.svg
├── docs/
│   └── TODO.md
├── include/
│   ├── nanosvg.h
│   └── nanosvgrast.h
├── src/
│   ├── core/
│   │   ├── Component.cpp
│   │   └── Component.h
│   ├── gates/
│   │   ├── AndGate.cpp
│   │   ├── AndGate.h
│   │   ├── NotGate.cpp
│   │   ├── NotGate.h
│   │   ├── OrGate.cpp
│   │   └── OrGate.h
│   ├── utils/
│   │   ├── ResourceManager.cpp
│   │   ├── ResourceManager.h
│   │   ├── Wire.cpp
│   │   └── Wire.h
│   ├── main.cpp
│   └── nanosvg_impl.cpp
├── CMakeLists.txt
└── .gitignore
```

## Dependencies

- [raylib](https://www.raylib.com/): A simple and easy-to-use library for video game programming

## License

[Include license information here, if applicable]

## Contributing

[Include contribution guidelines here, if applicable]

## Acknowledgements

- raylib developers and contributors
- [Any other acknowledgements]