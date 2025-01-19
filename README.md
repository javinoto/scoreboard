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


## Setting Up the Project

1. Clone the repository:
   ```bash
   git clone git@github.com:javinoto/scoreboard.git
   cd scoreboard
   ```
2. Go to the project directory:
   ```bash
   cd src
   ```

### Test Execution

💡 To execute the code according the instructions need to choose the correct branch
 
 - **brach for task1:** feature/task1
 - **brach for task2:** main
    
Before to change of brach need to execute `make clean` 

```bash
# example for execution code for Task 1
make clean
git checkout feature/task1
```


For the task1:
 
 1. Compile task1:
     ```bash
    make task1_test
    ```
 2. Run task1::
    ```bash
    make task1_test_run
    ```
    Example output:
    ```bash
    +-----+-----+-----+-----+-----+-----+-----+-----+-----+-------+
    | 7 - | 5 / | x   | x   | x   | x   | x   | x   | 7 1 | x x x |
    |   7 |  27 |  57 |  87 | 117 | 147 | 174 | 192 | 200 |   230 |
    +-----+-----+-----+-----+-----+-----+-----+-----+-----+-------+ 
    ```
 3. Check memory for task1:
    ```bash
    make task1_test_memcheck
    ```
    Example Valgrind output:
    ```bash
    ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
    ```
For task2:
1. Compile task2:
    ```bash
    make task2_test
     ```

2. Run task2:
    ```bash
    make task2_test_run
     ```
   Example output:

   ```bash
   +-----+-----+-----+-----+-----+-----+-----+-----+-----+-------+
   | 7 - | 5 / | x   | x   | x   | x   | x   | x   | 7 1 | x x x |
   |   7 |  27 |  57 |  87 | 117 | 147 | 174 | 192 | 200 |   230 |
   +-----+-----+-----+-----+-----+-----+-----+-----+-----+-------+
   +-----+-----+-----+-----+-----+-----+-----+-----+-----+-------+
   | x   | x   | x   | x   | x   | x   | x   | x   | x   | x x x |
   |  30 |  60 |  90 | 120 | 150 | 180 | 210 | 240 | 270 |   300 |
   +-----+-----+-----+-----+-----+-----+-----+-----+-----+-------+
   +-----+-----+-----+-----+-----+-----+-----+-----+-----+-------+
   | x   | x   | 8 - | 6 - | 7 / |     |     |     |     |       |
   |  28 |  46 |  54 |  60 |  70 |     |     |     |     |       |
   +-----+-----+-----+-----+-----+-----+-----+-----+-----+-------+
     ```
3. Check memory for task2:
    ```bash
    make task2_test_memcheck
     ```
    Example Valgrind output: 
    ```bash
    ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)    
    ```

Another executions:

1. Another examples of instructions doc:
    ```bash
    # input
    ./task1 547-9/5/8-X5/XX5/X
     ```

     ```bash
    #output
    +-----------------------------------------+
    |5 4|7 -|9 /|5 /|8 -|X  |5 /|X  |X  |5 / X|
    |  9| 16| 31| 49| 57| 77| 97|122|142|  162|
    +-----------------------------------------+

     ```




 



