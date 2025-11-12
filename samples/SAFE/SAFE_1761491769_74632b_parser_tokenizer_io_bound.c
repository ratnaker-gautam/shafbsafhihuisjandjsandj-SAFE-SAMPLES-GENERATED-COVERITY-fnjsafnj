//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

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
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int line = 1;
    int column = 1;
    int token_count = 0;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        while (isspace((unsigned char)input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
        }
        
        if (input[pos] == '\0') break;
        
        int start_pos = pos;
        int start_column = column;
        enum TokenType type = TOKEN_INVALID;
        
        if (isdigit((unsigned char)input[pos])) {
            type = TOKEN_NUMBER;
            while (isdigit((unsigned char)input[pos])) {
                pos++;
                column++;
            }
        } else if (isalpha((unsigned char)input[pos]) || input[pos] == '_') {
            type = TOKEN_IDENTIFIER;
            while (isalnum((unsigned char)input[pos]) || input[pos] == '_') {
                pos++;
                column++;
            }
        } else if (is_operator_char(input[pos])) {
            type = TOKEN_OPERATOR;
            while (is_operator_char(input[pos])) {
                pos++;
                column++;
            }
        } else if (input[pos] == '"') {
            type = TOKEN_STRING;
            pos++;
            column++;
            while (input[pos] != '"' && input[pos] != '\0') {
                pos++;
                column++;
            }
            if (input[pos] == '"') {
                pos++;
                column++;
            }
        } else {
            pos++;
            column++;
        }
        
        int len = pos - start_pos;
        if (len > 0 && len < MAX_TOKEN_LEN) {
            tokens[token_count].type = type;
            tokens[token_count].line = line;
            tokens[token_count].column = start_column;
            strncpy(tokens[token_count].value, &input[start_pos], len);
            tokens[token_count].value[len] = '\0';
            token_count++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        token_count++;
    }
    
    return token_count;
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
    struct Token tokens[MAX_TOKENS];
    
    printf("Enter text to tokenize (max %d chars):\n", (int)sizeof(input) - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("\nTokens found (%d):\n", token_count);
    printf("Type       Value               Line  Column\n");
    printf("---------- ------------------- ----- ------\n");
    
    for (int i = 0; i < token_count; i++) {
        printf("%-10s %-18s %5d %6d\n",
               token_type_name(tokens[i].type),
               tokens[i].value,
               tokens[i].line,
               tokens[i].column);
    }
    
    return 0;
}