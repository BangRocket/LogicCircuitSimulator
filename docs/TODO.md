Here's a detailed **To-Do List** to guide you through the process of building the base engine:

---

### **1. Project Setup**
   - [x] Create project directory structure: `src/`, `include/`, `build/`, `docs/`, `assets/`.
   - [x] Initialize a Git repository for version control.
   - [x] Set up the build system (Makefile, CMake, or your preferred system).

---

### **2. Core Engine Loop**
   - [x] Initialize raylib and create a window.
   - [x] Set a target frame rate (60 FPS).
   - [x] Implement a basic core loop that handles:
     - [x] Input handling.
     - [x] Update logic.
     - [x] Rendering.
     - [x] Cleanup (exit conditions).

---

### **3. Input System**
   - [x] Capture mouse position using `GetMousePosition()`.
   - [x] Implement basic mouse click detection (left-click).
   - [x] Add mouse drag detection (for future components and wire placement).
   - [x] Capture keyboard inputs for shortcuts (undo, redo, etc.).
   - [x] Create a placeholder function to handle different input events.

---

### **4. Grid System**
   - [x] Define a grid size (e.g., 32x32 pixels per cell).
   - [x] Implement grid snapping (convert screen coordinates to grid coordinates).
   - [x] Create a basic grid-drawing function using raylib's `DrawLine()`.
   - [x] Ensure components will snap to the grid for placement.

---

### **5. Rendering System**
   - [x] Create a function to draw the grid on the screen.
   - [x] Set up a placeholder function to draw components (e.g., gates, wires) in the future.
   - [x] Add dynamic color changing for grid lines or cells to visualize component placement.
   - [x] Implement a basic camera system for panning and zooming the grid.
   - [x] Add a function to highlight the grid cell under the mouse cursor.
   - [x] Implement a function to draw placeholder components on the grid.
   - [x] Implement component rotation in rendering.
   - [x] Implement texture handling for components.

---

### **6. Resource Management**
   - [x] Implement a basic resource loader.
   - [x] Load placeholder textures (or draw shapes) for gates and components.
   - [x] Set up a system to easily add and manage textures/sprites for future components.
   - [x] Implement a resource cleanup function to free memory when closing the application.

---

### **7. Debugging Tools**
   - [x] Implement simple debug printouts (e.g., display mouse position on-screen).
   - [x] Create a toggle for showing/hiding debug information.
   - [x] Add debug information for component rotation.
   - [ ] Prepare for future debugging needs (circuit errors, signal flows).

---

### **8. Testing and Iteration**
   - [x] Test the core loop for stability (input, rendering, and frame rate consistency).
   - [x] Test grid snapping with different grid sizes.
   - [x] Test mouse input handling and ensure clicks/positions are correctly captured.
   - [x] Test the dynamic color changing of grid cells for responsiveness and visual clarity.
   - [x] Test the camera system for smooth panning and zooming.
   - [x] Test component rotation functionality thoroughly.
   - [x] Test wire connections after component rotation.
   - [ ] Ensure that the window can resize without breaking the grid system.
   - [ ] Test texture rendering for all component types.

---

### **9. Component System**
   - [x] Implement a basic Component class with position and texture.
   - [x] Create a system for placing and removing components on the grid.
   - [x] Implement AND gate as a derived class from Component.
   - [x] Implement OR gate as a derived class from Component.
   - [x] Implement NOT gate as a derived class from Component.
   - [x] Implement Input Switch as a derived class from Component.
   - [x] Implement a way to switch between different component types when placing.
   - [x] Add input and output pins to components for wire connections.
   - [x] Implement a system for connecting components' input and output pins.
   - [x] Add rotation property to Component class.
   - [x] Implement component rotation functionality.
   - [x] Implement component scaling functionality.
   - [x] Add texture handling to Component class.

---

### **10. Wire System**
   - [x] Implement a Wire class to connect component pins.
   - [x] Create a system for drawing wires between components.
   - [x] Implement wire placement and removal functionality.
   - [x] Ensure proper signal propagation through wires.
   - [x] Update wire connections when components are rotated.
   - [ ] Implement wire routing algorithm to avoid overlapping with components.
   - [ ] Add visual feedback for invalid wire connections.
   - [ ] Implement wire snapping to grid.
   - [ ] Add ability to create curved or angled wires.

---

### **11. Logic Simulation**
   - [x] Implement basic logic simulation for AND gates.
   - [x] Extend logic simulation to OR gates.
   - [x] Implement logic simulation for NOT gates.
   - [x] Create a system for propagating signals through the circuit.
   - [x] Ensure logic simulation works correctly with rotated components.
   - [ ] Implement a clock system for synchronous logic (if needed).
   - [ ] Add support for multi-input gates (e.g., 3+ input AND/OR gates).
   - [ ] Implement signal propagation delay simulation.
   - [ ] Add support for floating inputs and high-impedance states.

---

### **12. User Interface Improvements**
   - [x] Add a toolbar or menu for selecting different component types.
   - [x] Implement a status bar showing current mode (e.g., placing component, connecting wires).
   - [x] Add visual feedback when hovering over components or pins.
   - [x] Implement a way to rotate components.
   - [x] Implement keyboard shortcuts for rotation (e.g., 'R' to rotate 90 degrees clockwise).
   - [ ] Add a way to delete wires.
   - [ ] Implement component dragging and repositioning.
   - [ ] Add UI element to show current rotation angle.
   - [ ] Implement a context menu for component actions (rotate, delete, etc.).
   - [ ] Add undo/redo functionality for user actions.
   - [ ] Implement a zoom in/out feature for the circuit view.
   - [ ] Add a mini-map for large circuits.
   - [ ] Implement a search function to find components in complex circuits.

---

### **13. Circuit Interaction**
   - [x] Implement a way to set input values for circuit testing (Input Switch).
   - [x] Add visual representation of signal states (high/low) on wires and component pins.
   - [ ] Create a step-by-step simulation mode for debugging circuits.
   - [ ] Add the ability to pause and resume the simulation.
   - [ ] Implement an output display component to show circuit results.
   - [x] Ensure circuit interaction works correctly with rotated components.
   - [ ] Add a circuit reset functionality.
   - [ ] Implement a circuit state save/load feature.
   - [ ] Add a circuit validation tool to check for common errors.

---

### **14. Performance Optimization**
   - [ ] Implement efficient data structures for storing and updating circuit state.
   - [ ] Optimize rendering for large circuits.
   - [ ] Implement culling for off-screen components and wires.
   - [ ] Optimize rotation calculations and rendering.
   - [ ] Implement component instancing for improved rendering performance.
   - [ ] Profile and optimize critical paths in the simulation loop.
   - [ ] Implement multi-threading for parallel simulation of independent circuit sections.
   - [ ] Add level-of-detail rendering for zoomed-out views.

---

### **15. Additional Components**
   - [ ] Implement XOR gate.
   - [ ] Implement NAND gate.
   - [ ] Implement NOR gate.
   - [ ] Implement D Flip-Flop for sequential logic.
   - [ ] Create a 7-segment display component for output visualization.
   - [x] Ensure all components work correctly with rotation.
   - [ ] Add customizable delay for components to simulate propagation delay.
   - [ ] Implement multiplexer and demultiplexer components.
   - [ ] Add counter components (e.g., binary counter, decade counter).
   - [ ] Implement shift register components.

---

### **Future Steps (Post Base Engine)**
   - [ ] Design and implement the UI for component selection, control panel, etc.
   - [ ] Add more complex logic components (e.g., multiplexers, decoders).
   - [ ] Implement subcircuits or hierarchical designs.
   - [ ] Add task/challenge systems and sandbox mode.
   - [ ] Implement a save/load system for circuits.
   - [ ] Add an undo/redo system for user actions.
   - [ ] Create a basic tutorial system to guide new users.
   - [ ] Implement a circuit validation system to check for errors.
   - [ ] Add performance optimizations for large circuits.
   - [ ] Implement export functionality (e.g., to VHDL or Verilog).
   - [ ] Add support for custom component creation.
   - [ ] Implement a simulation speed control feature.
   - [ ] Add advanced rotation features (e.g., arbitrary angles, mirroring).
   - [ ] Implement a component search functionality.
   - [ ] Add a minimap for navigating large circuits.
   - [ ] Implement a circuit sharing system for users to exchange designs.
   - [ ] Add a timing diagram view for signal analysis.
   - [ ] Implement a circuit testing framework with automated test cases.
   - [ ] Add support for analog components and mixed-signal simulation.
   - [ ] Implement a plugin system for extending functionality.
