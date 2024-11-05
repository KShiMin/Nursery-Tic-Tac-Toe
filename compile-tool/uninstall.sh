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

# Uninstall the compile script
uninstall_script() {
    # Check if the file exists
    if [ ! -f "$INSTALL_PATH/compile" ]; then
        echo "'compile' is not installed at $INSTALL_PATH."
        exit 1
    fi

    # Check for permissions on Unix systems
    if [ "$OS" != "Windows" ] && [ ! -w "$INSTALL_PATH" ]; then
        echo "Permission denied: Unable to write to $INSTALL_PATH. Please run the script with 'sudo' to uninstall."
        exit 1
    fi

    # Remove the script
    rm "$INSTALL_PATH/compile"
    echo "'compile' has been uninstalled from $INSTALL_PATH."
}

# Remove from PATH if on Windows Git Bash
remove_from_path() {
    if [[ "$OS" == "Windows" && -f ~/.bashrc ]]; then
        sed -i '/export PATH=.*\/bin/d' ~/.bashrc
        echo "Removed $INSTALL_PATH from PATH in ~/.bashrc."
        echo "Please restart your terminal or run 'source ~/.bashrc' for changes to take effect."
    fi
}

# Prompt for confirmation
echo "Do you want to uninstall 'compile' from $INSTALL_PATH? (y/n)"
read -r answer
if [[ "$answer" =~ ^[Yy]$ ]]; then
    detect_os
    uninstall_script
    remove_from_path
else
    echo "Uninstallation canceled."
    exit 0
fi
