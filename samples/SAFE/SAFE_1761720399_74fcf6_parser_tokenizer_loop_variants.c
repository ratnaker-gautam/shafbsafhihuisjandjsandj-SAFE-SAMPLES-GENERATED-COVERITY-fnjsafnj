//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

struct Tokenizer {
    const char *input;
    int position;
    int line;
    int column;
};

void tokenizer_init(struct Tokenizer *t, const char *input) {
    t->input = input;
    t->position = 0;
    t->line = 1;
    t->column = 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

struct Token tokenizer_next(struct Tokenizer *t) {
    struct Token token = {TOKEN_INVALID, "", t->line, t->column};
    
    while (t->input[t->position] != '\0') {
        char current = t->input[t->position];
        
        if (isspace(current)) {
            if (current == '\n') {
                t->line++;
                t->column = 1;
            } else {
                t->column++;
            }
            t->position++;
            continue;
        }
        
        if (isdigit(current)) {
            int i = 0;
            do {
                if (i < 31) {
                    token.value[i++] = current;
                }
                t->position++;
                t->column++;
                current = t->input[t->position];
            } while (isdigit(current));
            token.value[i] = '\0';
            token.type = TOKEN_NUMBER;
            return token;
        }
        
        if (isalpha(current) || current == '_') {
            int i = 0;
            do {
                if (i < 31) {
                    token.value[i++] = current;
                }
                t->position++;
                t->column++;
                current = t->input[t->position];
            } while (isalnum(current) || current == '_');
            token.value[i] = '\0';
            token.type = TOKEN_IDENTIFIER;
            return token;
        }
        
        if (is_operator_char(current)) {
            int i = 0;
            do {
                if (i < 31) {
                    token.value[i++] = current;
                }
                t->position++;
                t->column++;
                current = t->input[t->position];
            } while (is_operator_char(current) && i < 2);
            token.value[i] = '\0';
            token.type = TOKEN_OPERATOR;
            return token;
        }
        
        if (current == '"') {
            int i = 0;
            t->position++;
            t->column++;
            current = t->input[t->position];
            
            while (current != '"' && current != '\0' && i < 31) {
                token.value[i++] = current;
                t->position++;
                t->column++;
                current = t->input[t->position];
            }
            
            if (current == '"') {
                t->position++;
                t->column++;
            }
            token.value[i] = '\0';
            token.type = TOKEN_STRING;
            return token;
        }
        
        token.value[0] = current;
        token.value[1] = '\0';
        token.type = TOKEN_INVALID;
        t->position++;
        t->column++;
        return token;
    }
    
    token.type = TOKEN_EOF;
    strcpy(token.value, "EOF");
    return token;
}

const char* token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "INVALID";
    }
}

int main(void) {
    char input[1024];
    printf("Enter text to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    struct Tokenizer tokenizer;
    tokenizer_init(&tokenizer, input);
    
    printf("\nTokens:\n");
    printf("%-12s %-15s %-6s %-6s\n", "Type", "Value", "Line", "Column");
    printf("------------------------------------------------\n");
    
    struct Token token;
    do {
        token = tokenizer_next(&tokenizer);
        printf("%-12s %-15s %-6d %-6d\n", 
               token_type_name(token.type), 
               token.value, 
               token.line, 
               token.column);
    } while (token.type != TOKEN_EOF);
    
    return 0;
}