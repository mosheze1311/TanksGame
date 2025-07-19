# Tank Battle Simulator - TAU (Semester B 2025)

## Names and IDs

Itai Missri, 322330820
Moshe Zeitouny, 211388913

## Overview

A deterministic tank battle game for "Advanced Topics in Programming" at TAU. Features movement, shooting, and strategic gameplay.

## Approved Implementation Choices

1. We implemented a specialized `CompetitionPlayer` class and corresponding `CompetitionAlgorithm`, an advanced strategy adapted for assignment 3.
2. We extended our static multiton Logger design to ensure synchronized logging across all GameManager instances and Simulator threads.
3. All Managers and Algorithms are compiled to `.so` files and loaded via a registrar mechanism. Validation is performed on load.
4. Multithreading is used to improve runtime performance in both modes.
5. In addition to local Makefiles for each module, we provide a top-level Makefile to streamline building the entire project from root.
6. We implemented GetBattleInfo so it would cancel backward movements.
7. After waiting for two steps to execute the MoveBackward command, on the 3rd step, if a MoveForward or GetBattleInfo command is received we cancel the backward movement.
All other commands are ignored during this waiting period, and the backward movement proceeds as planned.

## Project Structure

```
Root
├── Makefile  
├── Assets
│   ├── Algorithms
│   │   └── Algorithm_*.so
│   ├── Managers
│   │   └── GameManager_*.so
│   └── Maps
│       
│
├── Simulator
│   ├── main.cpp
│   ├── Simulator.h/.cpp
│   ├── Registrar/, ThreadPool/, MapFileParser/
│   └── ParsedCmdArguments.h, ConsoleArgumentsParser.h
│
├── GameManager
│   ├── GameManager.cpp/.h
│   └── RegisterManager.cpp
│
├── Algorithm
│   ├── RegisterAlgorithm.cpp (plus competition registration)
│   ├── Players/
│   │   ├── CompetitionPlayer.h
│   │   └── Player1.h, Player2.h, etc.
│   └── Tactics/, Formation/, BattleInfo/
│
├── UserCommon
│   └── Logger/, GameBoard/, BoardSatelliteView/, GameObjects/, Utils/
│
├── common
  └── Abstract headers: GameManager, TankAlgorithm, Player, etc.

```
## Input File Format

The input file is divided into two main sections: **Game Settings** and **Board Description**.

### 1. **Game Settings (First 5 Lines)**

```
<MapName>
MaxSteps = <NUM>
NumShells = <NUM>
Rows = <NUM>
Cols = <NUM>
```

### 2. **Map Layout**

Each following line describes a row of the board.

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

## Output Files

### GameManager Verbose Output

When `-verbose` is passed as a command-line argument, each GameManager creates a file `output_<map_filename>.txt`.

#### Round-by-Round Format

Each line contains actions taken by each tank in the order they appear (top-to-bottom, left-to-right).

* Ignored actions: `(ignored)`
* Killed tanks: `killed`
* Action + killed: `SomeAction (ignored) (killed)`

#### Last Line Format

```
Player <X> won with <Y> tanks still alive
Tie, both players have zero tanks
Tie, reached max steps = <MAX_STEPS>, player 1 has <X> tanks, player 2 has <Y> tanks
Tie, both players have zero shells for <zero_shells_steps> steps
```

### input\_errors.txt

Logged by GameManager during input parsing.

### runtime\_errors.txt

Logged by GameManager on exceptions (usually empty if no issues).

### Comparative Mode Output

File is created as `comparative_results_<time>.txt` under the managers folder.
If file cannot be created, writes to screen.

#### Format

```
game_map=<map_filename>
algorithm1=<algorithm1.so>
algorithm2=<algorithm2.so>

<grouped_manager_names>
<game_result_message>
<round_number>
<final_map_state>

... (repeat if needed for other groups)
```

### Competitive Mode Output

File is created as `competition_<time>.txt` under the algorithms folder.
If file cannot be created, writes to screen.

#### Format

```
game_maps_folder=<maps_folder>
game_manager=<manager.so>

<algorithm_name_1> <score>
<algorithm_name_2> <score>
...
```

(Sorted descending by score)

## Simulator Modes

The simulator can be launched in two modes:

### 1. **Comparative Mode**

* **Command**:

  ```bash
  ./simulator -mode comparative -map <map.txt> -algo1 <algo1.so> -algo2 <algo2.so> -managers <managers_folder> -output <output.txt>
  ```
* **Logic**:
  Runs each manager with the given two algorithms on the specified map.
  Grouped results are written to the output file.

### 2. **Competitive Mode**

* **Command**:

  ```bash
  ./simulator -mode competitive -maps <folder> -algorithms <folder> -managers <folder> -output <output.txt>
  ```
* **Logic**:
  All `.so` managers play all maps with all algorithm pairs. Round-robin.
  
* **Scoring**:

  * Win: 3 points
  * Tie: 1 point
  * Loss: 0 points
* **Ranking**:
  Results are tabulated and ranked per manager.

## Game Mechanics

* **Actions**: MoveForward, MoveBackward, Rotate45Right, Rotate90Right, Rotate45Left, Rotate90Left, Shoot, DoNothing, GetBattleInfo.
* **Shells**: Move \<shells\_speed> squares per step, destroy tanks, weaken walls (deal 1 hp damage).
* **Ammo limit**: Each tank starts with <NumShells> shells (no reloading).
* **Board wraps around** (looped boundaries).
* **Win Conditions**: Destroy opponent’s tank.

## Win & Tie Conditions

* **Elimination** – A player wins immediately when the opponent has no tanks left on the board.
* **Simultaneous knockout** – If the last tank of each player is destroyed in the same turn the game ends in a **tie**.
* **Shell-out stalemate** – When **all remaining tanks run out of shells**, the game continues for **\<zero\_shells\_steps>**. If neither tank is destroyed during those steps, the game is declared a **tie**.
* **Max-steps stalemate** – After <MaxSteps> steps without a winner the game is declared as a **tie**.
* **End-game check** – All win/tie conditions are evaluated at the beginning of the step.
  For instance, if Player 1’s last tank is destroyed in the first half-turn and Player 2’s in the second, both events are considered together at the beginning of the next step, the game is declared as a **tie**.

## Building

Each major component (Simulator, GameManager, Algorithms, Scripts) has its own `Makefile`. You may also use the top-level Makefile for convenience.

To build everything:

```bash
make
```

Or per module:

```bash
cd Simulator && make
cd GameManager && make
cd Algorithm && make
```

## Running

For comparative mode:

```bash
./simulator -mode comparative -map Assets/Maps/input1.txt \
    -algo1 Assets/Algorithms/Algorithm_*.so \
    -algo2 Assets/Algorithms/Algorithm_*.so \
    -managers Assets/Managers -output result.txt
```

For competitive mode:

```bash
./simulator -mode competitive \
    -maps Assets/Maps -algorithms Assets/Algorithms \
    -managers Assets/Managers -output rankings.txt
```

## Final Notes

* The `CompetitionPlayer` and `CompetitionAlgorithm` were created specifically for this assignment to showcase strategic behavior.
* The system adheres to all constraints outlined in Assignment 3 PDF.
* Tested thoroughly with different `.so` combinations and maps.

## Submission Deadline

**August 24th, 2025, 23:30**