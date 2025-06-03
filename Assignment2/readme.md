# Tank Battle Game - TAU (Semester B 2025)
## Names and IDS
Itai Missri, 322330820

Moshe Zeitouny, 211388913

## Overview
A deterministic tank battle game for "Advanced Topics in Programming" at TAU. Features movement, shooting, and strategic gameplay.

## Approved Implementation Choices
1. We implemented GetBattleInfo so it would cancel backward movements.
2. After waiting for two steps to execute the MoveBackward command, on the 3rd step, if a MoveForward or GetBattleInfo command is received we cancel the backward movement.
All other commands are ignored during this waiting period, and the backward movement proceeds as planned.

## Input File Format

The input file is divided into two main sections: **Game Settings** and **Board Description**.

### 1. **Game Settings (First 5 Lines)**

The first 5 lines of the input file define the game settings. These lines must appear in this exact order:

    <MapName>
    MaxSteps = <NUM>
    NumShells = <NUM>
    Rows = <NUM>
    Cols = <NUM>

Where:
- `MapName` - A free text naming or describing the map
- `MaxSteps` — The maximum number of steps the game will run for.
- `NumShells` — Number of shells each tank starts with.
- `Rows` — Number of rows in the game board.
- `Cols` — Number of columns in the game board.

### 2. **Board Layout (Lines 6 and onward)**

Starting from line 6, each line corresponds to a row of the game board. The characters in each line describe the initial state of the game board, from top to bottom (i.e., line 6 is row 0, line 7 is row 1, etc.).

The ith character in the jth row represents the object at position (i, j) on the board.

#### Allowed Map Characters

- `#`: Wall  
- `@`: Mine  
- `1`–`9`: Tank belonging to Player 1 through Player 9  
  (initial direction is defined in the `config.txt` file, there also exist a default value)  
- Space (` `): Empty cell  
- Any other character: Treated as Empty cell  

#### Example Input

```
random1
MaxSteps = 100
NumShells = 10
Rows = 10
Cols = 10
    #  # @
    ##    
 #  # 2@  
 @ @      
        
    ##   #
# @  # ###
#  #@ @   
#1  #  @# 
  @ @#     
```

This input defines a 10x10 board with various objects placed on the map. Each row from line 6 onward describes a visual representation of the game board.


## Output Files

The game generates three output files:

1. **output_<input_file>.txt**  
2. **input_errors.txt**  
3. **runtime_errors.txt**

### 1. output_<input_file>.txt
> The output file will be named `output_<input_file>.txt`, where `<input_file>` is the name of the input file used (without extension).
The output is a plain text file that logs the game round-by-round. Each line corresponds to a full round and lists the action taken by each tank in the order they were "born".

#### Tank Order
Tanks are listed in the order they appear on the board, scanned row by row from top to bottom, and left to right within each row. This order remains fixed throughout the game.

#### Per-Round Format
Each line contains the action performed by every tank during that round, separated by commas. Actions are named exactly as in the `Action` enum.

- If an action cannot be executed, it is marked with `(ignored)`.
- If a tank is destroyed during the round, its action in that round will include `(killed)`.
- A dead tank’s entry will be the word `killed`.

If an action is both ignored and the tank is killed in the same round, it will be shown as:
```
SomeAction (ignored) (killed)
```

#### Final Line
The last line in the file indicates the game result in one of the following formats:

- `Player <X> won with <Y> tanks still alive`
- `Tie, both players have zero tanks`
- `Tie, reached max steps = <MAX_STEPS>, player 1 has <X> tanks, player 2 has <Y> tanks`
- `Tie, both players have zero shells for <zero_shells_steps> steps`

*(Where `zero_shells_steps` is a constant in the config.txt, e.g., 40.)*

#### Example Output

```
MoveForward, MoveBackward, RotateLeft90, RotateRight90
RotateLeft45, RotateRight45 (ignored), Shoot, GetBattleInfo
DoNothing, DoNothing, Shoot (ignored), MoveForward
RotateLeft45, MoveForward (killed), MoveForward, MoveForward
MoveForward, killed, MoveForward, MoveForward (ignored) (killed)
Player 2 won with 2 tanks still alive
```

In addition to the main game output file, two additional files may be generated:

### 2. input_errors.txt

This file logs errors found when parsing the input file. These include:

- Missing configuration lines.
- Unrecognized characters in the map.
- Invalid board dimensions.

### 3. runtime_errors.txt

This file logs unexpected issues that occur during the game’s execution.

- This file is typically empty for successful runs.

## Game Mechanics
- **Actions**: MoveForward, MoveBackward, Rotate45Right, Rotate90Right, Rotate45Left, Rotate90Left, Shoot, DoNothing, GetBattleInfo.
- **Shells**: Move <shells_speed> squares per step, destroy tanks, weaken walls (deal 1 hp damage).
- **Ammo limit**: Each tank starts with <NumShells> shells (no reloading).
- **Board wraps around** (looped boundaries).
- **Win Conditions**: Destroy opponent’s tank.

## Game Manager
- Controls the flow of the game by asking the Algorithms for actions.
- Manages game rules and steps.
- Executes algorithms actions on tanks and moves shells. 
- Logs all actions and invalid moves in the output file.

### Hierarchy Structure
```
GameManager
│
├── Algorithms
│   ├── Aggresive Algorithm (Chasing Opponent - Dijkstra)
│   └── Passive Algorithm (Survival - Evading Shells & Mines)
│
├── Players
│   └── Interact with algorithms using updateTankWithBattleInfo()
│
├── CollisionHandler
│   └── Handle collision of objects
│
└── Game Board
    ├── Walls, Mines
    ├── Shells
    ├── Tank 
    └── Metric calculations 
```

## Win & Tie Conditions
* **Elimination** – A player wins immediately when the opponent has no tanks left on the board.
* **Simultaneous knockout** – If the last tank of each player is destroyed in the same turn    
  the game ends in a **tie**.
* **Shell-out stalemate** – When **all remaining tanks run out of shells**, the game continues for **<zero_shells_steps>**.  
  If neither tank is destroyed during those steps, the game is declared a **tie**.
* **Max-steps stalemate** - after <MaxSteps> steps without a winner the game is declared as a **tie**.
* **End-game check** – All win/tie conditions are evaluated at the beginning of the step.
  For instance, if Player 1’s last tank is destroyed in the first half-turn and Player 2’s in the second, both events are considered together at the beginning of the next step, the game is declared as a **tie**.

## Building
The repository includes a standard **Makefile**.  
Simply run:

    make         # compiles tanks_game

## Running
After a successful build:

    ./tanks_game <game_board_input_file>
    

## Submission Deadline
**June 8, 2025, 23:30**
