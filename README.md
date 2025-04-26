# Tank Battle Game - TAU (Semester B 2025)
## Names and IDS
Itai Missri, 322330820

Moshe Zeitouny, 211388913

## Overview
A deterministic tank battle game for "Advanced Topics in Programming" at TAU. Features movement, shooting, and strategic gameplay.

## Input File Format

The input file is divided into two main sections: **Game Settings** and **Game Objects**.

### 1. **Game Settings (First Line)**

The first line of the input file defines the game settings. The values should be in the following order, separated by spaces:

```
height width tanks_p1 tanks_p2 walls mines
```

Where:
- `height` = The number of rows (vertical size) of the game board.
- `width` = The number of columns (horizontal size) of the game board.
- `tanks_p1` = The number of tanks for player 1.
- `tanks_p2` = The number of tanks for player 2.
- `walls` = The number of walls on the board.
- `mines` = The number of mines on the board.

### 2. **Game Objects (Subsequent Lines)**

Each subsequent line describes the positions of the game objects (tanks, walls, and mines) on the board. Each line should be in the format:

```
game_object_char x y
```

Where:
- `game_object_char` = A character representing the type of object:
  - `#` for wall
  - `@` for mine
  - `1` for tank1 (player 1's tank)
  - `2` for tank2 (player 2's tank)
- `x` = The x-coordinate (horizontal position, starting from 0).
- `y` = The y-coordinate (vertical position, starting from 0).

#### **Important Notes:**
- Positions are **0-indexed**, meaning the first position on the board is `(0,0)`.
- The order of the objects does **not** matter, but only the **first n objects** of each type (where `n` is defined by the first line) will be used.
  - If there are fewer objects listed than expected, the number of objects used will be adjusted to match the number declared.
- **Empty lines** are ignored.
- **Duplicate start positions** – During board setup, if several objects are listed for the same cell, only the first one is placed and any others for that cell are skipped.

### Example Input:

```
4 5 3 2 1 1
1 0 0
1 0 1
1 0 2
2 1 1
2 1 2
# 2 2
@ 3 3
```

This represents a 4x5 game board with:
- 3 tanks for player 1 (`1` at (0,0), (0,1), (0,2))
- 2 tanks for player 2 (`2` at (1,1), (1,2))
- 1 wall (`#` at (2,2))
- 1 mine (`@` at (3,3))


## Output Files

There are 3 types of output files generated during the game:

1. **output**: This file logs the actions performed during the game.
   - **Format**: 
     ```
     [INFO] (In)Valid action: Tank type '1'/'2' tried action {Action Name}
     ```
   - The last log will always declare the winner or a tie in the following format:
     ```
     [INFO] The winner is: Player {player number}! {winning reason}
     [INFO] The game is tied! {tie reason}
     ```

2. **input_errors**: This file logs identified mismatches between the required format of the input file and the actual file.
   - These errors may include:
     - Unrecognized object character
     - Object location out of board bounds
     - And more
   - **Format**:
     ```
     [ERROR] {error description}.
     ```

3. **runtime_errors**: This file logs any other errors encountered during code execution.
   - In most cases, this file will be empty.
   - **Format**: Same as `input_errors`:
     ```
     [ERROR] {error description}.
     ```


## Game Mechanics
- **Actions**: Move, rotate (1/8 or 1/4), shoot.
- **Shells**: Move 2 squares per step, destroy tanks, weaken walls.
- **Ammo limit**: Each tank starts with **16 shells** (no reloading).
- **Board wraps around** (looped boundaries).
- **Win Conditions**: Destroy opponent’s tank or survive 40 extra steps if ammo runs out.

## Game Manager
- Controls the flow of the game by asking the players for actions.
- Manages game rules, turns, and action validation.
- Executes valid moves by modifying the game board.
- Logs all actions and invalid moves in the output file.

### Hierarchy Structure
```
GameManager
│
├── Player (x2)
│   ├── Strategy 1 (Chasing Opponent - DFS/BFS)
│   ├── Strategy 2 (Survival - Evading Shells & Mines)
│   ├── Has access to game board data (read-only)
│   └── Requests actions from GameManager
│
└── Game Board
    ├── Walls, Mines
    ├── Shells
    ├── Tank Positions & Directions
    └── Movement & Collision Logic (controlled by GameManager)
```

## Player Strategies
- **Each player has two different strategies**:
  - One **chases the opponent** (DFS/BFS-based pathfinding).
  - One **focuses on survival** (avoiding mines, evading shots).

## Win & Tie Conditions
* **Elimination** – A player wins immediately when the opponent has no tanks left on the board.
* **Simultaneous knockout** – If the last tank of each player is destroyed in the same turn  
  (e.g., Player 1’s tank is destroyed during the first half-turn and Player 2’s during the second),  
  the game ends in a **tie**.
* **Shell-out stalemate** – When **all remaining tanks run out of shells**, the game continues for **40
  more turns** (shell-movement steps).  
  If neither tank is destroyed during those 40 turns, the game is declared a **tie**.
* **Adjacent duel rule** – If two enemy tanks stand on adjacent cells and both fire at each other
  during the same turn, **both tanks are destroyed**.

## Logging
- Logs **all actions, invalid moves, and results**.

## Running the Program
```
tanks_game <game_board_input_file>
```


## Submission Deadline
**April 27, 2025, 23:30**
