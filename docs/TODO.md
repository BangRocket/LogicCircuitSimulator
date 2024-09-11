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
   - [ ] Add dynamic color changing for grid lines or cells to visualize component placement.
   - [ ] Implement a basic camera system for panning and zooming the grid.

---

### **6. Resource Management**
   - [x] Implement a basic resource loader.
   - [x] Load placeholder textures (or draw shapes) for gates and components.
   - [ ] Set up a system to easily add and manage textures/sprites for future components.
   - [ ] Implement a resource cleanup function to free memory when closing the application.

---

### **7. Debugging Tools**
   - [x] Implement simple debug printouts (e.g., display mouse position on-screen).
   - [ ] Create a toggle for showing/hiding debug information.
   - [ ] Prepare for future debugging needs (circuit errors, signal flows).

---

### **8. Testing and Iteration**
   - [x] Test the core loop for stability (input, rendering, and frame rate consistency).
   - [x] Test grid snapping with different grid sizes.
   - [x] Test mouse input handling and ensure clicks/positions are correctly captured.
   - [ ] Ensure that the window can resize without breaking the grid system.

---

### **Future Steps (Post Base Engine)**
   - [ ] Begin adding basic components (logic gates, wires).
   - [ ] Implement basic logic simulation (signal propagation, gate behavior).
   - [ ] Design and implement the UI for component selection, control panel, etc.
   - [ ] Add task/challenge systems and sandbox mode.
   - [ ] Implement a save/load system for circuits.
   - [ ] Add an undo/redo system for user actions.
   - [ ] Create a basic tutorial system to guide new users.
   - [ ] Implement a circuit validation system to check for errors.
