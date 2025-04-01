# Tank Battle Game - TAU (Semester B 2025)

## Overview
A deterministic tank battle game for "Advanced Topics in Programming" at TAU. Features movement, shooting, and strategic gameplay.

## Input File Format
- **Game board (text file)**: Specifies width, height, walls (`#`), mines (`@`), and tanks (`1`, `2`).
- **Tank direction**: Player 1 faces left, Player 2 faces right.

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

## Logging
- Logs **all actions, invalid moves, and results**.

## Running the Program
```
tanks_game <game_board_input_file>
```

## Submission Deadline
**April 27, 2025, 23:30**

