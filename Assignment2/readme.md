# Tank Game Simulator – Assignment 2  
**Advanced Topics in Programming – Tel Aviv University, Semester B 2025**

## Overview

This project implements a turn-based tank game simulator as defined in Assignment 2. The system is designed to manage tank movements, simulate artillery fire, handle collisions, and determine win conditions between two players. The architecture separates responsibilities cleanly among the `GameManager`, `Player`, and `TankAlgorithm` components, with support for dynamic object creation through factories.

## Development To-Dos

### Code Refactoring

- **Refactor `readBoard` Logic**  
  Move the logic inside `readBoard` to a separate class—similar to the original `BoardFactory` class from Assignment 1.  
  This class can be defined either as an inner class within `Manager`, or as an independent class.  
  The `readBoard` method must remain a `Manager` function, but should simply delegate to this new board creation class.

### Code Quality

- **Review All `TODO`s in `Manager`**  
  Go through and resolve all `TODO` comments in the `Manager` source file.

- **Constant Usage**  
  Revisit and correct usage of `const` for variables and functions to ensure best practices are followed.

### Code Reuse

- **Salvage Logic from `GameManager`**  
  Reuse any valuable logic or structure from the original `GameManager` implementation.

### Design Considerations

- **Clarify Roles**  
  Clearly define the difference between an *Algorithm* and a *Player* in our system.

- **Understand Factory Responsibilities**
  - Why do we have both Algorithm and Player factories?
  - Should these factories create instances based on `PlayerID` or `TankID`?
  - Should each player have its own dedicated factory?

### Memory Management

- **Fix Board Instantiation**  
  Eliminate the use of raw `new` for the board.  
  Consider using smart pointers (`std::unique_ptr` or `std::shared_ptr`) to manage memory safely and efficiently.

- **Handle Destructors in Inheritance Hierarchies**  
  Ensure that all base classes (e.g., `GameObject`, `Player`, `TankAlgorithm`) declare a `virtual` destructor.  
  This is essential for correct cleanup of derived objects when managed via `std::unique_ptr<Base>`, and avoids undefined behavior or memory leaks.

<!-- ## Build and Run

To compile and run the project:

```bash
make && ./tanks_game <game_board_input_file> -->
