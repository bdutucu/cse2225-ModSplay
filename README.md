# Mod-Splay Tree


This repository contains an implementation of the classic **Splay Tree** and a custom-modified version called the **Mod-Splay Tree**. The project was assigned as a homework task for the course **CSE2225 - Data Structures**.

## What is a Splay Tree?

A **Splay Tree** is a type of self-adjusting binary search tree. After every access (insertion, search, etc.), the accessed node is moved to the root using tree rotations. This helps keep frequently accessed nodes closer to the top of the tree, improving performance for certain usage patterns.

## What is the Mod-Splay Tree?

The **Mod-Splay Tree** in this project is a small twist on the regular splay tree:

- It keeps track of how many times each node (key) is accessed (i.e., its frequency).
- When the frequency of a node surpasses the current recorded maximum, that node is "splayed" (moved up to the root).
- This approach tries to keep the most frequently accessed nodes close to the top, possibly optimizing for access patterns with lots of repeats!

## How does it work?

- The program **reads integer keys from `input.txt`**.
- Each key is inserted into both the standard Splay Tree and the Mod-Splay Tree.
- For each tree, the program keeps track of two costs:
  - **Comparison Cost**: How many key comparisons were made.
  - **Rotation Cost**: How many tree rotations were performed.
- After all keys are processed, the structure and costs of both trees are written to **`output.txt`**.

## File Overview

- `main.c` (or the C file in this repo): Contains all the code for the trees, insertion logic, file reading/writing, and cost calculations.
- `input.txt`: Place your comma-separated (or space-separated) integers here for testing.
- `output.txt`: Program output, showing pre-order traversals and costs for both trees.

## How to Run

1. **Compile the code** (assuming `main.c` is the filename):

    ```bash
    gcc -o modsplay main.c
    ```

2. **Prepare your input file**:

    - Create `input.txt` in the same directory with integers, for example:
      ```
      5, 3, 8, 3, 10, 5, 5, 8
      ```

3. **Run the program**:

    ```bash
    ./modsplay
    ```

4. **Check `output.txt`** for the results!

## Example Output

```
Splay tree
Tree (pre-order traversal): 5 3 8 10 
Cost: 14 
Mod-Splay tree 
Tree (pre-order traversal): (5,2) (3,1) (8,2) (10,1)  
Cost: 11 
```
