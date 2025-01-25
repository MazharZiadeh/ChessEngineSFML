

# Chess Engine with Minimax & Simple Evolutionary Evaluation

This project is a **C++ Chess Engine** that supports:
- Human vs. AI gameplay with a **SFML** graphical interface,
- **Minimax** search with alpha-beta pruning for move selection,
- A **basic evolutionary algorithm** to tune the engine’s evaluation parameters (piece values).

**Disclaimer:** This is a simplified demonstration rather than a complete chess rules engine. We skip some special moves (e.g., en passant, castling) and do not fully check for checkmate in the traditional sense—only detecting no legal moves. 

## Table of Contents
1. [Project Structure](#project-structure)
2. [Features & Description](#features--description)
3. [Dependencies](#dependencies)
4. [Build & Run](#build--run)
5. [Usage Instructions](#usage-instructions)
6. [Extending the Project](#extending-the-project)

---

## Project Structure

```
.
├── ChessTypes.h        // Basic definitions (PieceType, Color, structs Piece & Move)
├── Board.h             // Board class (header)
├── Board.cpp           // Board class (implementation)
├── Evaluation.h        // Evaluation parameters & evolutionary training (header)
├── Evaluation.cpp      // Evaluation parameters & evolutionary training (implementation)
├── Minimax.h           // Minimax functions (header)
├── Minimax.cpp         // Minimax functions (implementation)
├── main.cpp            // The main SFML GUI application
└── README.md           // This file
```

1. **ChessTypes.h**  
   Contains basic enumerations for piece types and colors, along with the `Piece` and `Move` struct definitions.

2. **Board.h / Board.cpp**  
   Implements the `Board` class, which holds an 8×8 array of `Piece` objects and provides methods to initialize a standard chess position, generate pseudo-legal moves, make and undo moves, etc.

3. **Evaluation.h / Evaluation.cpp**  
   - `EvalParameters` struct for storing piece values (pawn, knight, bishop, rook, queen).  
   - A **naive evaluation function** (`evaluateBoard`) that sums up material using these piece values.  
   - A **simple evolutionary algorithm** to mutate and train piece values:
     - Creates a population of random `EvalParameters`.
     - Evaluates each candidate on a small set of test positions.
     - Keeps the top half of candidates, mutates the bottom half, repeats for some generations.
   
4. **Minimax.h / Minimax.cpp**  
   Implements **alpha-beta pruning** (`alphaBeta`) and a helper function to find the best move (`findBestMove`).

5. **main.cpp**  
   - Runs the optional “training” step for evaluation parameters.  
   - Initializes SFML, creates a game window, draws the chessboard and pieces.  
   - Lets the human (White) click+drag to move pieces, while the AI (Black) responds with `findBestMove`.  
   - Renders everything in a simple 2D GUI using SFML’s shapes and colors.

---

## Features & Description

- **Human vs. AI**: White is controlled by mouse clicks; Black is controlled by the minimax AI.
- **Minimax Search (Alpha-Beta)**: The AI searches up to a fixed depth (default 4), calling the `evaluateBoard` function at leaf nodes.
- **Simplified Evolutionary Algorithm**: Called at the start of `main`, it attempts to tune the engine’s piece values by measuring how “balanced” the evaluation is on a small set of test positions.
- **SFML GUI**: Renders an 8×8 board with colored tiles and circular pieces:
  - Outline color indicates the piece type (e.g., red = king, green = queen, etc.).
  - Fill color indicates side (white or black).
  
**Limitations**:
- No check detection or special move logic (e.g., no castling, no en passant).
- No detection of draws, insufficient material, or threefold repetition.
- Move generation is only pseudo-legal (it doesn’t check if the king is left in check).

Despite these simplifications, it’s suitable for demonstrating a functional minimax engine, basic evaluation, and how an evolutionary approach might adjust piece values.

---

## Dependencies

1. **C++17 or later**: The code uses modern C++ features (e.g., `<random>`).
2. [**SFML**](https://www.sfml-dev.org/) **2.5+** (Graphics, Window, System):
   - Required for rendering the chessboard and handling user mouse events.
3. A **C++ compiler** that can link SFML (e.g., G++, Clang, or MSVC).

On most Linux distros, you can install SFML via your package manager (e.g., `sudo apt-get install libsfml-dev`). On Windows, you can download precompiled SFML binaries or build from source, then configure the library paths in your build environment.

---

