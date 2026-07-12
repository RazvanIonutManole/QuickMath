# QuickMath Project

A C++ based math game compiled to WebAssembly (WASM) using Emscripten, paired with a local Python server to manage game data.

## Getting Started
To run this project, you will need to have [Emscripten](https://emscripten.org/) and Python installed on your system.

### Prerequisites
*   **Emscripten SDK (emsdk):** Required to compile the C++ source code to WebAssembly.
*   **Python 3:** Required for the local server and dependency management.

---

## 1. Compilation Steps
This project includes a build script to compile the C++ source into a web-ready format.

1.  Open your terminal or command prompt.
2.  Activate your Emscripten environment (run `emsdk_env` in your terminal).
3.  Navigate to the project root folder.
4.  Run the build script:
    *   **Windows:** `compile.bat`
    *   **Linux/macOS:** `./compile.sh`

*Note: This script will generate the necessary `.wasm`, `.js`, `.data` and `.html` files in your program directory.*

---

## 2. Running the Project

### Option A: Windows
1.  Ensure you have compiled the project using the steps above.
2.  Double-click `run_game_windows.bat`.
3.  This script will automatically install necessary dependencies (`flask`, `flask-cors`) and start the local server.

### Option B: Linux / macOS
1.  Open a terminal in the project root folder.
2.  Install required Python dependencies:
    ```bash
    pip3 install flask flask-cors
    ```
3.  Start the server and the local web host:
    ```bash
    python3 -m http.server 8000 --directory program & cd server && python3 server.py
    ```
4.  Open your browser and navigate to: `http://localhost:8000`.

*To stop the project, press `CTRL+C` in your terminal and run `killall python3` to free up the ports.*

---

## Technical Notes
*   **Built with:** C++, SDL2, Emscripten, and Python (Flask).