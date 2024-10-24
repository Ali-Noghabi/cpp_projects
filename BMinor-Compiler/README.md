## B Minor Language Lexical Analyzer

This repository contains a lexical analyzer implemented in C for the B Minor programming language. The program reads a source code file and performs lexical analysis, identifying and categorizing tokens such as keywords, identifiers, integers, operators, delimiters, and strings in the B Minor language.

### How It Works

The program utilizes a set of functions to recognize and process different types of tokens. It maintains a symbol table to store unique identifiers and replaces token values with corresponding identifiers when printing token information. The lexical analysis is performed character by character, identifying and printing tokens based on their characteristics specific to the B Minor language.

### Tutorial: How to Run

#### In main.c, choose the definitions of `inputPath` and `outputPath` based on your operating system:

```c
const char *inputPath = "..\\input.txt";  // For Windows
// const char *inputPath = "../input.txt";  // For Linux
const char *outputPath = "..\\output.txt";  // For Windows
// const char *outputPath = "../output.txt";  // For Linux
```

#### Windows

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/Ali-Noghabi/BMinor-Compiler.git
   ```

2. **Compile the Code:**
   ```bash
   cd BMinor-Compiler
   mkdir build
   cd build
   gcc ..\main.c -o b_minor_lexical_analyzer
   ```

3. **Run the Program:**
   ```bash
   b_minor_lexical_analyzer.exe
   ```

#### Linux

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/Ali-Noghabi/BMinor-Compiler.git
   ```

2. **Compile the Code:**
   ```bash
   cd BMinor-Compiler
   mkdir build
   cd build
   gcc ..\main.c -o b_minor_lexical_analyzer
   ```

3. **Run the Program:**
   ```bash
   ./b_minor_lexical_analyzer
   ```

### Input and Output

The input source code file is specified in the `inputPath` variable within the code. The program performs lexical analysis and prints token information to both the console and an output file specified in the `outputPath` variable.

### Symbol Table

The program maintains a dynamic symbol table to store unique identifiers encountered during lexical analysis. This table is used to replace token values with corresponding identifiers, reducing redundancy in the output.

Feel free to explore and modify the code according to your needs to suit the lexical analysis requirements of the B Minor language!

## Contributors

- **Ali Noghabi**: a.noghabi2002@gmail.com
- **Ghaf0o0r**: [profile](https://github.com/Ghaf0o0r)