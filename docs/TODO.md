Here's a detailed **To-Do List** to guide you through the process of building the base engine:

---

### **1. Project Setup**
   - [ ] Create project directory structure: `src/`, `include/`, `build/`, `docs/`, `assets/`.
   - [ ] Initialize a Git repository for version control.
   - [ ] Set up the build system (Makefile, CMake, or your preferred system).

---

### **2. Core Engine Loop**
   - [ ] Initialize raylib and create a window.
   - [ ] Set a target frame rate (60 FPS).
   - [ ] Implement a basic core loop that handles:
     - [ ] Input handling.
     - [ ] Update logic.
     - [ ] Rendering.
     - [ ] Cleanup (exit conditions).

---

### **3. Input System**
   - [ ] Capture mouse position using `GetMousePosition()`.
   - [ ] Implement basic mouse click detection (left-click).
   - [ ] Add mouse drag detection (for future components and wire placement).
   - [ ] Capture keyboard inputs for shortcuts (undo, redo, etc.).
   - [ ] Create a placeholder function to handle different input events.

---

### **4. Grid System**
   - [ ] Define a grid size (e.g., 32x32 pixels per cell).
   - [ ] Implement grid snapping (convert screen coordinates to grid coordinates).
   - [ ] Create a basic grid-drawing function using raylib’s `DrawLine()`.
   - [ ] Ensure components will snap to the grid for placement.

---

### **5. Rendering System**
   - [ ] Create a function to draw the grid on the screen.
   - [ ] Set up a placeholder function to draw components (e.g., gates, wires) in the future.
   - [ ] Add dynamic color changing for grid lines or cells to visualize component placement.

---

### **6. Resource Management**
   - [ ] Implement a basic resource loader (optional for now).
   - [ ] Load placeholder textures (or draw shapes) for gates and components.
   - [ ] Set up a system to easily add and manage textures/sprites for future components.

---

### **7. Debugging Tools**
   - [ ] Implement simple debug printouts (e.g., display mouse position on-screen).
   - [ ] Create a toggle for showing/hiding debug information.
   - [ ] Prepare for future debugging needs (circuit errors, signal flows).

---

### **8. Testing and Iteration**
   - [ ] Test the core loop for stability (input, rendering, and frame rate consistency).
   - [ ] Test grid snapping with different grid sizes.
   - [ ] Test mouse input handling and ensure clicks/positions are correctly captured.
   - [ ] Ensure that the window can resize without breaking the grid system.

---

### **Future Steps (Post Base Engine)**
   - [ ] Begin adding basic components (logic gates, wires).
   - [ ] Implement basic logic simulation (signal propagation, gate behavior).
   - [ ] Design and implement the UI for component selection, control panel, etc.
   - [ ] Add task/challenge systems and sandbox mode.
