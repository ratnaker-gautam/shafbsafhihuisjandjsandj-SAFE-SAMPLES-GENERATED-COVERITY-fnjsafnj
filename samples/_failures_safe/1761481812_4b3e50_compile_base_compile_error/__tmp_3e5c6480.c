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
    TOKEN_EOF
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int tokenize(const char* input, struct Token* tokens, int max_tokens) {
    int token_count = 0;
    int line = 1;
    int column = 1;
    int i = 0;
    
    while (input[i] != '\0' && token_count < max_tokens) {
        if (isspace(input[i])) {
            if (input[i] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            i++;
            continue;
        }
        
        if (isdigit(input[i])) {
            int start = i;
            while (isdigit(input[i])) {
                i++;
                column++;
            }
            int len = i - start;
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            strncpy(tokens[token_count].value, &input[start], len);
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            tokens[token_count].line = line;
            tokens[token_count].column = column - len;
            token_count++;
            continue;
        }
        
        if (isalpha(input[i]) || input[i] == '_') {
            int start = i;
            while (isalnum(input[i]) || input[i] == '_') {
                i++;
                column++;
            }
            int len = i - start;
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            strncpy(tokens[token_count].value, &input[start], len);
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            tokens[token_count].line = line;
            tokens[token_count].column = column - len;
            token_count++;
            continue;
        }
        
        if (input[i] == '"') {
            int start = i;
            i++;
            column++;
            while (input[i] != '"' && input[i] != '\0') {
                i++;
                column++;
            }
            if (input[i] == '"') {
                i++;
                column++;
            }
            int len = i - start - 2;
            if (len < 0) len = 0;
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            if (len > 0) {
                strncpy(tokens[token_count].value, &input[start + 1], len);
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            tokens[token_count].line = line;
            tokens[token_count].column = column - len - 2;
            token_count++;
            continue;
        }
        
        if (is_operator_char(input[i])) {
            int start = i;
            while (is_operator_char(input[i])) {
                i++;
                column++;
            }
            int len = i - start;
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            strncpy(tokens[token_count].value, &input[start], len);
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            tokens[token_count].line = line;
            tokens[token_count].column = column - len;
            token_count++;
            continue;
        }
        
        i++;
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

void print_token(struct Token token) {
    const char* type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"
    };
    printf("Line %d, Column %d: %s", token.line, token.column, type_names[token.type]);
    if (token.type != TOKEN_EOF) {
        printf(" ('%s')", token.value);
    }
    printf("\n");
}

int main(void) {
    char input[1024];
    printf("Enter text to tokenize (max 1023 characters):\n");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    struct Token tokens[MAX_TOK