#!/bin/bash

HELPSTR="Usage: $(basename "$0") [-h] [-f <file>] [-F <folder>] [-o <output>]
    -h          Show this help message. 
    -f <file>   Compile a single C file. 
    -F <Folder> Compile all C files in the specified folder.
    -o <output> Specify the output executable name.
"

detect_os(){
    case "$(uname -s)" in 
        Linux*)     OS="Linux";;
        Darwin*)    OS="Mac";;
        CYGWIN*|MINGW32*|MSYS*|MINGW*)  OS="Windows";;
        *)          OS="Unknown";;
    esac
}

# function to normalise paths for windows (when using Git bash or Cygwin)
normalise_path() {
    if [ "$0" = "Windows" ]; then 
        echo "$(cygpath -w "$1")"
    else
        echo "$1"
    fi
}

# function to display help 
display_help(){
    echo "$HELPSTR"
    exit 0
}

# function to compile a single C file 
compile_file(){
    local file="$1"
    local output="$2"

    local filepath="$(normalise_path "$SCRIPTPATH/$FILE")"

    if [ ! -f "$filepath" ]; then 
        echo "Error: File '$file' not found."
        exit 1
    fi

    if [ -z "$output" ]; then 
        output="${file%.*}" # default output file name if -o is not provided
    fi

    echo "Compiling $file..."

    gcc -o "$output" "$filepath"

    if [ $? -eq 0 ]; then 
        echo "Compilation successful."
        echo -e  "Executing... \n\n"
        "$SCRIPTPATH/$output"
    else
        echo "Compilation failed."
        exit 1
    fi
}

# function to compile all C files in folder 
compile_folder() {
    local folder="$1"
    local output="$2"

    local folderpath="$(normalise_path "$SCRIPTPATH/$folder")"

    if [ ! -d "$folderpath" ]; then
        echo "Error: Folder '$folder' not found."
        exit 1
    fi 

    echo "Compiling all C files in folder $folder..."

    # Create an array to hold all C files 
    local files=()

    for file in "$folderpath"/*.c; do 
        [ -e "$file" ] || continue 
        files+=("$file") # Add each file to the array
    done

    # Check if any C files were found 
    if [ ${#files[@]} -eq 0 ]; then
        echo "No C files found in folder $folder." 
        exit 1
    fi

    # Set output name 
    if [ -z "$output" ]; then
        output="$folderpath${files[0]##*/}" # use the name of the first file 
        output="${output%.*}" # remove the file extension
    else 
        output="$folderpath$output" # comebine path with executable
    fi

    gcc -o "$output" "${files[@]}"

    if [ $? -eq 0 ]; then 
        echo "Compilation successful: $output" 
        echo -e "Executing... $output \n\n"
        "$output"
    else
        echo "Compilation failed."
        exit 1
    fi
}


# main script logic 
detect_os
SCRIPTPATH="$(cd --"$(dirname "$0")" > /dev/null 2>&1 ; pwd -P)"
output_file=""
FILE=""
FOLDER=""

# parse flags
while getopts "hf:F:o:" flag; do
    case $flag in 
        h) display_help ;;
        f) FILE="$OPTARG" ;;
        F) FOLDER="$OPTARG" ;;
        o) output_file="$OPTARG" ;;
        \?) display_help ;;
    esac
done

# validate input 
if [ -n "$FILE" ] && [ -n "$FOLDER" ]; then 
    echo "Error: You cannot specify both -f and -F options."
    exit 1
fi

# if only -f is provided
if [ -n "$FILE" ]; then 
    echo "This is working"
    compile_file "$FILE" "$output_file"
fi

# if only -F is provided
if [ -n "$FOLDER" ]; then
    compile_folder "$FOLDER" "$output_file"
fi
    

# if no flags are provided, show the help message
if [ $OPTIND -eq 1 ]; then
    display_help
fi