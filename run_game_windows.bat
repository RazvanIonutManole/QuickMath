@echo off
echo ===================================================
echo   Starting Math Game (Backend + Frontend Servers)
echo ===================================================

:: 1. Check if Python itself is installed
where python >nul 2>nul
if %errorlevel% neq 0 (
    echo [WARNING] Python is NOT installed on this machine!
    echo Since this game uses a Python Flask backend, Python is required.
    echo.
    echo Downloading the Python installer for you automatically...
    
    :: This downloads the official Windows 64-bit Python installer quietly to the current folder
    powershell -Command "Invoke-WebRequest -Uri 'https://www.python.org/ftp/python/3.11.9/python-3.11.9-amd64.exe' -OutFile 'python_installer.exe'"
    
    echo.
    echo [ACTION REQUIRED] The installer has been downloaded. Opening it now...
    echo.
    echo CRUCIAL: In the Python installer window, you MUST check the box:
    echo "Add python.exe to PATH" at the very bottom before clicking Install!
    echo.
    
    :: Launch the installer
    start python_installer.exe
    
    echo After the installation finishes, please CLOSE this terminal 
    echo and run 'run_game.bat' again.
    pause
    exit
)

:: 2. If Python exists, check for Flask and Flask-CORS dependencies
echo Checking Python dependencies...
python -c "import flask, flask_cors" 2>nul
if %errorlevel% neq 0 (
    echo Dependencies missing. Installing flask and flask-cors...
    pip install flask flask-cors
    echo Dependencies installed successfully!
    echo ---------------------------------------------------
) else (
    echo All dependencies are already installed. Proceeding...
    echo ---------------------------------------------------
)

:: 3. Launch the Python Backend Server in a new window
echo Launching Flask backend on port 5000...
start "Math Game Backend" cmd /k "cd server && python server.py"

:: 4. Launch the Frontend Web Server in a new window
echo Launching Frontend server on port 8000...
start "Math Game Frontend" cmd /k "cd program && python -m http.server 8000"

:: 5. Automatically open the game in the user's default browser
echo Opening game in your web browser...
timeout /t 2 >nul
start http://localhost:8000

echo All systems running! You can close this main window.
pause