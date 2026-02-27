# Syntax Tree
#### Group members:
    - Alfa Reynisdóttir
    - Auðunn Daníel Jónsson
    - Karen Yin Guðnadóttir


### What needs to be written in the terminal to compile and run the program, explaining how to run each of the two parts.

To compile the program:
```
make
```


Run – Part 1 (Build AST and Write to File)
To run the program:
```
./a.out <ast_output_file> <expression_input_file>
```
* <ast_output_file>: file where the AST will be written
* <expression_input_file>: file containing the arithmetic expression
* If <expression_input_file> is missing, the program reads from stdin


Run – Part 2 (Read and Evaluate AST)
The program will:

    ```
    ./a.out <ast_input_file>
    ```

Build the Abstract Syntax Tree (AST)
Print the tree in a readable format
Store the AST in tree.txt using BFS format

After writing tree.txt, the program:
- Reads the AST back from tree.txt
- Reconstructs the tree from the stored format
- Reads variable values from vars.txt
- Calculates the expression
- Prints the result

### What additional features were implemented above the base version, if any.
    - Whitespace
    - Modern C++ Tree
    - Varibles
    - AST optimisations
    - High speed
    - Error handling
    - (Printing out the tree)

### An explanation of the format you store the AST (Abstract Syntax Tree) in that the two versions read/write.
The Abstract Syntax Tree (AST) is stored in a text file using a Breadth-First Search (BFS) order. This means the tree is written level by level, from left to right.

Format (tree is written as):
```
{ node1 node2 node3 ... }
```

How the Two Versions Use This Format
- The writer version converts the AST into this BFS format and saves it to a file.
- The reader version reconstructs the tree by reading tokens from left to right and rebuilding the structure recursively.

### Anything else a student wishes to say. Put it here and not in the Canvas comments!
