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
    int i = 0;
    
    while (input[i] != '\0' && token_count < max_tokens) {
        if (isspace((unsigned char)input[i])) {
            if (input[i] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            i++;
            continue;
        }
        
        struct Token *token = &tokens[token_count];
        token->line = line;
        token->column = column;
        
        if (isdigit((unsigned char)input[i])) {
            token->type = TOKEN_NUMBER;
            int j = 0;
            while (isdigit((unsigned char)input[i]) && j < MAX_TOKEN_LEN - 1) {
                token->value[j++] = input[i++];
                column++;
            }
            token->value[j] = '\0';
            token_count++;
        } else if (isalpha((unsigned char)input[i]) || input[i] == '_') {
            token->type = TOKEN_IDENTIFIER;
            int j = 0;
            while ((isalnum((unsigned char)input[i]) || input[i] == '_') && j < MAX_TOKEN_LEN - 1) {
                token->value[j++] = input[i++];
                column++;
            }
            token->value[j] = '\0';
            token_count++;
        } else if (input[i] == '"') {
            token->type = TOKEN_STRING;
            int j = 0;
            i++;
            column++;
            while (input[i] != '"' && input[i] != '\0' && j < MAX_TOKEN_LEN - 1) {
                token->value[j++] = input[i++];
                column++;
            }
            if (input[i] == '"') {
                i++;
                column++;
            }
            token->value[j] = '\0';
            token_count++;
        } else if (is_operator_char(input[i])) {
            token->type = TOKEN_OPERATOR;
            int j = 0;
            while (is_operator_char(input[i]) && j < MAX_TOKEN_LEN - 1) {
                token->value[j++] = input[i++];
                column++;
            }
            token->value[j] = '\0';
            token_count++;
        } else {
            i++;
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