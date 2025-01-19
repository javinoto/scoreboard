# Bowling Score Parser and Scoreboard

### Description

A C program to calculate and display bowling scores using custom data structures like linked lists and queues.

### Goal Project

1. Implement two related tasks as outlined in the provided instructions.
2. Maintain a version history during development to capture different save points.
3. Document the programming paradigms used for each task, such as conditions, loops, file I/O, dynamic memory allocation, etc.

## Project Structure

```plaintext
scoreboard/
├── src/                              # Source code (implementation files)
│   ├── linked_list.c                 # Implementation of linked list operations
│   ├── queue.c                       # Implementation of queue operations
│   ├── tpb.c                         # Bowling score parser and scoreboard logic
│   ├── score_calculator.c            # Score calculation and validation
│   ├── helpers.c                     # Utility/helper functions
│   ├── linked_list.h                 # Header file for linked list
│   ├── queue.h                       # Header file for queue
│   ├── tpb.h                         # Header file for tpb.c
│   ├── helpers.h                     # Header file for helpers
│   ├── Makefile                      # Build automation file
├── tests/                            # Test cases and resources
│   ├── test_cases.c                  # Unit tests for the project
│   ├── test_input.txt                # Input data for testing
│   ├── expected_output.txt           # Expected outputs for validation
├── docs /                            # Instructions and resources
│   ├── c_program_instructions.pdf    # Instrusctions of the C project
├── README.md                         # Project documentation
├── .gitignore                        # Ignore patterns for version control

```

## Requirements

Install the following tools in a Unix enviroment:

- `build-essential` (includes `gcc`, `g++`, and `make` for compiling and building C/C++ programs))
- `gdb` (GNU Debugger)
- `valgrind`(Memory analysis)

### Installation

```bash
sudo apt update && sudo apt install build-essential gdb valgrind
```

## Suggestion

To run the project code, consider using VScode with the following extensions:

- **Code Runner**: Quickly run your C programs directly from the editor.
- **C/C++ Extension Pack**: Provides debugging, IntelliSense, and other essential features for C/C++ dev.

