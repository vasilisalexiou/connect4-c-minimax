# Connect 4 - Minimax AI (C)

A terminal-based implementation of the classic Connect 4 game written in C. Play against a computer opponent that utilizes the **Minimax algorithm with Alpha-Beta pruning** to make intelligent moves.

## 📌 Features
* **Terminal UI:** Clean, ASCII-based board rendering.
* **AI Opponent:** Powered by a recursive Minimax algorithm.
* **Alpha-Beta Pruning:** Optimizes the search tree to look ahead 4 moves (Easy) or 6 moves (Hard) efficiently.
* **Heuristic Evaluation:** The AI scores board states by prioritizing the center column, creating 3-in-a-row threats, and blocking the player's winning moves.
* **Memory Management:** Utilizes dynamic memory allocation (`malloc`/`free`) to clone game states during the AI's recursive search.

## 🛠️ Technologies Used
* **Language:** C
* **Concepts:** Pointers, Structs, Dynamic Memory Allocation, Recursion, Game Trees, Minimax AI

## 🚀 How to Compile and Run
You will need a C compiler (like GCC) installed on your machine or you can run it using an online C compiler (like https://www.onlinegdb.com/online_c_compiler).

1. Clone this repository.
2. Open your terminal and navigate to the project directory.
3. Compile the code:
   ```bash
   gcc connect4.c -o connect4
