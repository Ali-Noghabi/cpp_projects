/*
Authors:
Mohammad Hossein Ghaforian 40013021
Ali Abdollahian Noghabi 9913062
*/
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*
Structures and enums
*/
enum Type
{
    Keywords,
    ID,
    Integer,
    OperatorSymbol,
    Delimiters,
    String
};

struct Token
{
    enum Type type;
    char *value;
    int line;
};

struct symbolTableElement
{
    struct Token token;
    int id;
};

/*
Functions prototype
*/
void printToken(struct Token token);
char *getTokenTypeString(enum Type type);
void insertToken(struct Token token);
int tokenExists(char *value);

void recongnizer(char *word);
int isValidInteger(char *word);
int isKeyWord(char *word);
int isValidID(char *word);
int isOperator(char c);
int isDoubleOperator(char *c);
int isDelimeters(char c);

void stringReader(FILE *file);
char operatorReader(char c, FILE *file);
void comment(int i, FILE *file);
void lexical_analysis(FILE *file);

/*
Global Variables and Definitions
*/
const char *keyWords[] = {"array", "boolean", "char", "else", "false", "for", "function", "if",
                          "integer", "print", "return", "string", "true", "void", "while"};
const char operators[] = {'+', '-', '!', '*', '/', '%', '=', '|', '^', '<', '>', '&'};
const char *doubleOperators[] = {"++", "--", "!=", "==", "<=", ">=", "||", "&&"};
const char delimeters[] = {'(', ')', '[', ']', '{', '}', ',', ';', ':', '\'', '"', '\\'};
struct symbolTableElement *symbolTable;
int tableMaxSize = 100;
int currentSize = 1;
int line = 1;
int isFirst = 1;
const char *inputPath = "..\\input.txt"; // For Windows
// const char *inputPath = "../input.txt";  // For Linux
const char *outputPath = "..\\output.txt"; // For Windows
// const char *outputPath = "../output.txt";  // For Linu
int main()
{
    symbolTable = (struct symbolTableElement *)malloc(tableMaxSize * sizeof(struct symbolTableElement));
    FILE *file = fopen(inputPath, "r+");
    lexical_analysis(file);
    free(symbolTable);
    return 0;
}

// Prints token information to console and file
// Replacing token value with an identifier if it exists in the symbol table.
void printToken(struct Token token)
{

    if (isFirst)
    {
        isFirst = 0;
        printf("%-20s %-20s %-20s\n------------------------------------------------------\n", "token", "token_type", "token_value");

        FILE *outputFile;
        outputFile = fopen(outputPath, "w");
        if (outputFile != NULL)
        {
            fprintf(outputFile, "%-20s %-20s %-20s\n------------------------------------------------------\n", "token", "token_type", "token_value");
        }
        fclose(outputFile);
    }

    char tokenValue[30];
    int result = tokenExists(token.value);
    if (result)
    {
        sprintf(tokenValue, "%d", result);
    }
    else
    {
        strcpy(tokenValue, token.value);
    }
    printf("%-20s %-20s %-20s\n", token.value, getTokenTypeString(token.type), tokenValue);

    FILE *outputFile;
    outputFile = fopen(outputPath, "a");
    if (outputFile != NULL)
    {
        fprintf(outputFile, "%-20s %-20s %-20s\n", token.value, getTokenTypeString(token.type), tokenValue);
    }
    fclose(outputFile);
}

// Returns a string representation of the given token type.
char *getTokenTypeString(enum Type type)
{
    switch (type)
    {
    case Keywords:
        return "keywords";
    case ID:
        return "ID";
    case Integer:
        return "Integer";
    case OperatorSymbol:
        return "operators";
    case Delimiters:
        return "delimiters";
    case String:
        return "STRING";
    }
}

// Inserts a token into the symbol table, dynamically resizing the table if needed.
void insertToken(struct Token token)
{
    if (currentSize >= tableMaxSize)
    {
        // If the array is full, double its size using realloc
        tableMaxSize *= 2;
        symbolTable = (struct symbolTableElement *)realloc(symbolTable, tableMaxSize * sizeof(struct symbolTableElement));
        if (symbolTable == NULL)
        {
            fprintf(stderr, "Memory reallocation failed\n");
            exit(1);
        }
    }

    symbolTable[currentSize].token = token;
    symbolTable[currentSize].id = currentSize;
    currentSize++;
}

// Checks if a token with the given value exists in the symbol table.
// Returns the corresponding identifier if found; otherwise, returns 0.
int tokenExists(char *value)
{
    for (int i = 0; i < currentSize; ++i)
    {
        if (strcmp(symbolTable[i].token.value, value) == 0)
        {
            return symbolTable[i].id;
        }
    }
    return 0;
}

// Recognizes and processes a word, creating a token based on its characteristics.
// Prints the token if valid, otherwise handles errors.
void recongnizer(char *word)
{
    if (isdigit(word[0]))
    {
        if (isValidInteger(word))
        {
            struct Token token;
            token.type = Integer;
            token.line = line;
            token.value = (char *)malloc(strlen(word) + 1);
            strcpy(token.value, word);
            printToken(token);
        }
        else
        {
            printf("ERROR : %s is not a valid integer value in line %d.\n" , word , line);
            return;
        }
    }
    else
    {
        if (isalpha(word[0]) || word[0] == '_')
        {
            if (isKeyWord(word))
            {
                struct Token token;
                token.type = Keywords;
                token.line = line;
                token.value = (char *)malloc(strlen(word) + 1);
                strcpy(token.value, word);
                printToken(token);
                return;
            }
            if (isValidID(word))
            {
                struct Token token;
                token.type = ID;
                token.line = line;
                token.value = (char *)malloc(strlen(word) + 1);
                if (!tokenExists(token.value))
                {
                    insertToken(token);
                }
                strcpy(token.value, word);
                printToken(token);
            }
            else
            {
                printf("ERROR : %s is not a valid identifier in line %d.\n" , word , line);
                return;
            }
        }
        else
        {
            printf("ERROR : %c is a invalid character in line %d.\n" , word[0] , line);
            return;
        }
    }
    free(word);
}

// Checks if the given string is a valid integer (within the range of a 64-bit integer).
// Returns 1 if valid, 0 otherwise.
int isValidInteger(char *word)
{
    /// be smaller than  Max-64-bit integer
    for (int i = 0; i < strlen(word); i++)
    {
        if (isdigit(word[i]))
        {
            continue;
        }
        return 0;
    }
    return 1;
}

// Checks if the given word is a keyword.
// Returns 1 if it is a keyword, 0 otherwise.
int isKeyWord(char *word)
{
    for (int i = 0; i < 15; i++)
    {
        if (!strcmp(word, keyWords[i]))
        {
            return 1;
        }
    }
    return 0;
}

// Checks if the given word is a valid identifier.
// Returns 1 if valid, 0 if invalid, and 0 if the length exceeds 256 characters (error).
int isValidID(char *word)
{
    if (strlen(word) > 256)
    {
        return 0;
    }
    for (int i = 1; i < strlen(word); i++)
    {
        if (isdigit(word[i]) || isalpha(word[i]) || word[i] == '_')
        {
            continue;
        }
        return 0;
    }
    return 1;
}

// Checks if the given character is an operator symbol.
// Returns 1 if it is an operator, 0 otherwise.
int isOperator(char c)
{
    for (int i = 0; i < sizeof(operators) / sizeof(char); i++)
    {
        if (c == operators[i])
        {
            return 1;
        }
    }
    return 0;
}

// Checks if the given string represents a double operator.
// Returns 1 if it is a double operator, 0 otherwise.
int isDoubleOperator(char *c)
{
    for (int i = 0; i < 8; i++)
    {
        if (!strcmp(c, doubleOperators[i]))
        {
            return 1;
        }
    }
    return 0;
}

// Checks if the given character is a delimiter.
// Returns 1 if it is a delimiter, 0 otherwise.
int isDelimeters(char c)
{
    for (int i = 0; i < 12; i++)
    {
        if (c == delimeters[i])
        {
            return 1;
        }
    }
    return 0;
}

// Reads a string enclosed in double quotes from the file and creates a String token.
// Prints the token if valid, otherwise handles errors.
void stringReader(FILE *file)
{
    char *string = (char *)malloc(1);
    string[0] = '\0';
    char c;
    while ((c = fgetc(file)) != '"')
    {
        if (c == EOF)
        {
            /// ERROR
            return;
        }
        if (c == '\n')
        {
            line++;
        }
        int n = strlen(string);
        string = (char *)realloc(string, n + 2);
        string[n + 1] = '\0';
        string[n] = c;
    }
    if (strlen(string) > 256)
    {
        /// ERROR
        return;
    }
    struct Token token;
    token.type = String;
    token.line = line;
    token.value = (char *)malloc(strlen(string) + 1);
    strcpy(token.value, string);
    printToken(token);
}

// Reads an operator symbol from the file and creates an OperatorSymbol token.
// Prints the token if valid, otherwise handles errors.
// Returns the next character after processing the operator.
char operatorReader(char c, FILE *file)
{
    char c2 = fgetc(file);
    char *cc2 = (char *)malloc(3);
    cc2[0] = c;
    cc2[1] = c2;
    cc2[2] = '\0';
    if (!strcmp(cc2, "/*"))
    {
        comment(1, file);
        return NULL;
    }
    if (!strcmp(cc2, "//"))
    {
        comment(2, file);
        return NULL;
    }
    if (isDoubleOperator(cc2))
    {
        struct Token token;
        token.type = OperatorSymbol;
        token.line = line;
        token.value = (char *)malloc(3);
        token.value[0] = c;
        token.value[1] = c2;
        token.value[1] = '\0';
        printToken(token);
        return NULL;
    }
    struct Token token;
    token.type = OperatorSymbol;
    token.line = line;
    token.value = (char *)malloc(2);
    token.value[0] = c;
    token.value[1] = '\0';
    printToken(token);
    return c2;
}

void comment(int i, FILE *file)
{
    char c ;
    char b = NULL;
    switch(i){
        case 1 :

            while(1){
                if(b != NULL){
                    c = b;
                    b = NULL;
                }else{
                    c = fgetc(file);
                }
                if(c == EOF){
                    return;
                }
                if(c == '*'){
                    b = fgetc(file);
                    if(b == '/'){
                        return;
                    }
                }
                if(c == '\n'){
                    line++;
                }
             }
             break;
        case 2 :
            while ((c = fgetc(file)) != EOF)
            {
                if(c == '\n'){
                    break;
                }
            }
    }
}

// Performs lexical analysis on the given file, identifying tokens and printing their information.
void lexical_analysis(FILE *file)
{
    char c;
    char b = NULL;
    char *str = NULL;
    while (1)
    {
        if (b == NULL)
        {
            c = fgetc(file);
        }
        else
        {
            c = b;
            b = NULL;
        }
        if (c == EOF)
        {
            break;
        }

        if (isspace(c))
        {
            if (str != NULL)
            {
                recongnizer(str);
                str = NULL;
            }
            if (c == '\n')
            {
                line++;
            }
            continue;
        }
        if (isDelimeters(c))
        {
            if (str != NULL)
            {
                recongnizer(str);
                str = NULL;
            }
            if (c == '"')
            {
                stringReader(file);
            }
            else
            {
                struct Token token;
                token.type = Delimiters;
                token.line = line;
                token.value = (char *)malloc(2);
                token.value[0] = c;
                token.value[1] = '\0';
                printToken(token);
            }
            continue;
        }
        if (isOperator(c))
        {
            if (str != NULL)
            {
                recongnizer(str);
                str = NULL;
            }
            b = operatorReader(c, file);
            continue;
        }
        if (str == NULL)
        {
            str = (char *)malloc(2);
            str[0] = c;
            str[1] = '\0';
        }
        else
        {
            int n = strlen(str);
            str = (char *)realloc(str, n + 2);
            str[n + 1] = '\0';
            str[n] = c;
        }
    }
}
