#!/usr/bin/env bash

# Determine install path based on OS
detect_os() {
    case "$(uname -s)" in 
        Linux*)     INSTALL_PATH="/usr/local/bin";;
        Darwin*)    INSTALL_PATH="/usr/local/bin";;
        CYGWIN*|MINGW32*|MSYS*|MINGW*)  INSTALL_PATH="$HOME/bin";;
        *)          echo "Unsupported OS"; exit 1;;
    esac
}

# Ensure the install path directory exists
prepare_install_path() {
    if [ ! -d "$INSTALL_PATH" ]; then
        mkdir -p "$INSTALL_PATH"
    fi
}

# Install the compile script
install_script() {
    if [ "$OS" = "Windows" ]; then
        # Call PowerShell script to request admin privileges on Windows
        powershell.exe -ExecutionPolicy Bypass -File "$(dirname "$0")/request_admin.ps1"
        exit 0
    else
        # Check for write permissions on Unix systems
        if [ ! -w "$INSTALL_PATH" ]; then
            echo "Permission denied: Unable to write to $INSTALL_PATH. Please run the script with 'sudo' for installation."
            exit 1
        fi

        # Copy and set permissions
        cp "$(dirname "$0")/compile.sh" "$INSTALL_PATH/compile"
        chmod +x "$INSTALL_PATH/compile"
        echo "Installed 'compile' to $INSTALL_PATH. You can now run 'compile -h' from anywhere."
    fi
}

# Add the directory to PATH if on Windows Git Bash
add_to_path() {
    if [[ "$OS" == "Windows" && ":$PATH:" != *":$INSTALL_PATH:"* ]]; then
        echo "export PATH=\"$INSTALL_PATH:\$PATH\"" >> ~/.bashrc
        echo "Added $INSTALL_PATH to PATH. Please restart your terminal or run 'source ~/.bashrc' for changes to take effect."
    fi
}

# Prompt for confirmation 
echo "Do you want to install 'compile' to $INSTALL_PATH ? (y/n)"
read -r answer
if [[ "$answer" =~ ^[Yy]$ ]]; then 
    detect_os
    prepare_install_path
    install_script
    add_to_path
else
    echo "Installation Canceled."
    exit 0
fi
