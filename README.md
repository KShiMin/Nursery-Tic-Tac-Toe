# Nursery Tic Tac Toe
This system is an interactive Tic Tac Toe game aimed at helping children develop motor skills, social skills and logical thinking through two game modes, two-player and single-player. The single-player mode utilises Artificial Intelligence such as Minimax and Q-learning. 

## Features
-  Child-Friendly Interface: Simple and colorful UI designed for young children.
-  Single-Player Mode: Enhance logical thinking through challenging plays against two different AI algorithms (Minimax and Q-learning).
-  Two-Player Mode: Encourages social interaction through turn-based play.

## Game GUI
Starting Page

![image](https://github.com/user-attachments/assets/3bc06f4d-9e8b-432f-8b3c-047e997964a4)


Tic Tac Toe Board

![image](https://github.com/user-attachments/assets/bafd4e8b-2435-4fec-9272-29629767648d)



## Technology Used
- **Language:** C
- **Graphics Library:** raylib
- **IDE:** Visual Studio Code
- **Development Environment:** Linux and Windows

## Installation Guide
### Prerequisites:
- C Compiler (e.g., gcc)
- raylib library installed on your system

### Steps to install and run
1. Clone this repository
```bash
git clone https://github.com/KShiMin/Nursery-Tic-Tac-Toe.git
```
2. Navigate to tic-tac-toe folder
```bash
cd tic-tac-toe
```
3. Compile the game

   For Windows:
   ```bash
   gcc -o <executable_name>.exe main.c gui.c game_logic.c minimax.c q_learning.c -lraylib -lopengl32 -lgdi32 -lwinmm
   ```

   For Linux:
   ```bash
   gcc -o <executable_name> main.c gui.c game_logic.c minimax.c q_learning.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
   ```
4. Run the game

   For Windows:
   ```bash
   ./<executable_name>.exe
   ```

   For Linux:
   ```bash
   ./<executable_name>
   ```

## License 
This repository is license under the [MIT License](https://github.com/KShiMin/Nursey-Tic-Tac-Toe/blob/dev/LICENSE).
