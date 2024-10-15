# Tic-Tac-Toe-Assignment
This repository contains C files for a university assignment focues on implementing a Tic Tac Toe game. The project includes various game modes, such as player vs. player and player vs. CPU

## Repository Structure 
``` bash
.
├── compile.sh
├── LICENSE
├── README.md
├── test.c
└── tic-tac-toe
```

## Collaboration Guidelines 
To maintain a smooth workflow, please adhere to the following best practices when collaborating on this repository: 

1. **Branching:**
    - Do not work direcly on the `main` branch.
    - For any new feature or part of the assignment, create a new branch based on the `dev branch. Use the following command: 
        ```bash 
        git checkout -b <feat_branch> dev
        ```
        **Notes:** Do change the `<feat_branch>` to a unique name based on what you are adding to the repo. 
    - Once your work is complete, create a pull request to merge your changes into the `dev` branch and contact @[KShiMin](https://github.com/KShiMin) to review and merge.

2. **Folder Structure:**
    - For each new part of the assignment, create a new folder to hold the relevant C files. This helps keep the repository organised and prevents clutter in the main directory.

3. **Commit Messages:**
    - Use clear and descriptive commit messages. 
    - Follow this format: 
        - `feat: Description of the feature`
        - `fix: Description of the bug fix`
        - `refactor: Description of the refactoring`
        - `docs: Description of documentation changes`

## Using the Auto Compiler 
The provided `compile.sh` script simplifies the compilation process for this project. Here's how to use it:

### Help Menu
To view the help menu for the auto compiler script, run: <br>
**For linux**
```bash
./compile.sh -h
```
**For Windows**
```bash 
.\compile.sh -h
```


### Compilation Options
- **-h:** Help menu for the script
- **-f:** Compile and execute a single C file.
- **-F:** Compile all C files in a specified folder, assuming all files are part of one main system.
- **-o:** Specify an output file name (optional):
    - If using `-f`, the output file will default to the file without its extension.
    - If using `-F`, the output file will take the name of the first file read by the script without its extension.

### Important Note
The compiler script is designed to work with GCC. It has only been tested using GCC, so ensure you have it installed before using the script.

## Manual Compilation
To compile a C file manually using GCC, use the following command: 
```bash
gcc -o <output_filename> <c_file>
```
This is a basic way to compile your C files. It's important to understand how to compile manually, even is you are using the auto compiiler for ease of testing.

## License 
This repository is license under the [MIT License](https://github.com/KShiMin/Nursey-Tic-Tac-Toe/blob/dev/LICENSE).