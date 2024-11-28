# GITLite: Simplified Version Control System

## Project Overview

GITLite is a simplified version control system inspired by Git, developed as a semester project for CS2001: Data Structures at the National University of Computer and Emerging Science (Fall 2024). The project demonstrates the implementation of advanced data structures and version control concepts.

## Key Features

### Core Functionalities
- **Repository Initialization**: Initialize a repository with CSV data using different tree structures
- **Branching**: Create, switch, and manage multiple branches
- **Commit Management**: Track and log changes across branches
- **Efficient Data Storage**: Optimize memory usage by storing tree nodes in separate files

### Tree Structures Supported
- AVL Tree
- B-Tree
- Red-Black Tree

### Advanced Capabilities
- Hash-based data integrity checks
- Merkle Tree implementation for change tracking
- Efficient data transfer and corruption detection

## Commands

- `init`: Initialize repository
- `branch <name>`: Create a new branch
- `checkout <name>`: Switch between branches
- `commit "<message>"`: Save changes with a descriptive message
- `log`: View commit history
- `branches`: List all branches
- `delete-branch <name>`: Remove a branch
- `merge <source> <target>`: Merge branches
- `visualize-tree <branch>`: Display tree structure (Bonus)

## Dataset

The project uses a healthcare operations dataset featuring:
- Patient demographics
- Medical conditions
- Hospital services
- Billing information
- Medications and test results

## Hash Generation Methods

Two hash generation methods are implemented:
1. **Instructor Hash**
   - Integers: Multiply digits, modulo 29
   - Strings: Multiply ASCII values, modulo 29
2. **SHA-256**: Standard cryptographic hashing

## Project Setup

1. Clone the repository
2. Compile the source code
3. Run the application
4. Follow on-screen prompts to initialize and manage your repository

## Technologies

- Language: C++
- Data Structures: AVL Tree, B-Tree, Red-Black Tree
- Hashing: Custom and SHA-256

## Contributors

[Your Group Names]

## License

[Specify your license, e.g., MIT, GPL]

## Acknowledgments

- National University of Computer and Emerging Science
- Course Instructor and Lab Instructors
