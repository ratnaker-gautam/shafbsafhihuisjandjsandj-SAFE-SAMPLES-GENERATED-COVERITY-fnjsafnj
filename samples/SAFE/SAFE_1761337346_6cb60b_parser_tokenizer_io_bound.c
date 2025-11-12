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
    return strchr("+-*/=<>!&|", c) != NULL;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int token_count = 0;
    int line = 1;
    int column = 1;
    const char *p = input;
    
    while (*p && token_count < max_tokens) {
        while (isspace(*p)) {
            if (*p == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            p++;
        }
        
        if (!*p) break;
        
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        
        if (isdigit(*p)) {
            tokens[token_count].type = TOKEN_NUMBER;
            int i = 0;
            while (isdigit(*p) && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *p++;
                column++;
            }
            tokens[token_count].value[i] = '\0';
            token_count++;
        } else if (isalpha(*p) || *p == '_') {
            tokens[token_count].type = TOKEN_IDENTIFIER;
            int i = 0;
            while ((isalnum(*p) || *p == '_') && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *p++;
                column++;
            }
            tokens[token_count].value[i] = '\0';
            token_count++;
        } else if (*p == '"') {
            tokens[token_count].type = TOKEN_STRING;
            int i = 0;
            p++;
            column++;
            while (*p != '"' && *p != '\0' && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *p++;
                column++;
            }
            tokens[token_count].value[i] = '\0';
            if (*p == '"') {
                p++;
                column++;
            }
            token_count++;
        } else if (is_operator_char(*p)) {
            tokens[token_count].type = TOKEN_OPERATOR;
            int i = 0;
            while (is_operator_char(*p) && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *p++;
                column++;
            }
            tokens[token_count].value[i] = '\0';
            token_count++;
        } else {
            p++;
            column++;
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

void print_token(const struct Token *token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"
    };
    printf("Line %d, Column %d: %s", token->line, token->column, type_names[token->type]);
    if (token->value[0] != '\0') {
        printf(" ('%s')", token->value);
    }
    printf("\n");
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
    for (int i = 0; i < token_count; i++) {
        print_token(&tokens[i]);
    }
    
    return 0;
}