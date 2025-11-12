//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
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
    TOKEN_EOF
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
        if (isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
            continue;
        }
        
        if (isdigit(input[pos])) {
            int start = pos;
            while (isdigit(input[pos])) {
                pos++;
                column++;
            }
            int len = pos - start;
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            strncpy(tokens[token_count].value, &input[start], len);
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            tokens[token_count].line = line;
            tokens[token_count].column = column - len;
            token_count++;
            continue;
        }
        
        if (isalpha(input[pos]) || input[pos] == '_') {
            int start = pos;
            while (isalnum(input[pos]) || input[pos] == '_') {
                pos++;
                column++;
            }
            int len = pos - start;
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            strncpy(tokens[token_count].value, &input[start], len);
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            tokens[token_count].line = line;
            tokens[token_count].column = column - len;
            token_count++;
            continue;
        }
        
        if (input[pos] == '"') {
            int start = pos;
            pos++;
            column++;
            while (input[pos] != '\0' && input[pos] != '"') {
                if (input[pos] == '\\' && input[pos + 1] != '\0') {
                    pos += 2;
                    column += 2;
                } else {
                    pos++;
                    column++;
                }
            }
            if (input[pos] == '"') {
                pos++;
                column++;
            }
            int len = pos - start;
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            strncpy(tokens[token_count].value, &input[start], len);
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            tokens[token_count].line = line;
            tokens[token_count].column = column - len;
            token_count++;
            continue;
        }
        
        if (is_operator_char(input[pos])) {
            int start = pos;
            while (is_operator_char(input[pos])) {
                pos++;
                column++;
            }
            int len = pos - start;
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            strncpy(tokens[token_count].value, &input[start], len);
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            tokens[token_count].line = line;
            tokens[token_count].column = column - len;
            token_count++;
            continue;
        }
        
        pos++;
        column++;
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

void print_token(const struct Token *token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"
    };
    printf("Line %d, Column %d: %s '%s'\n", 
           token->line, token->column, type_names[token->type], token->value);
}

int main(void) {
    char input[1024];
    printf("Enter text to tokenize (max %d chars):\n", (int)sizeof(input) - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    struct Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("\nTokens found:\n");
    for (int