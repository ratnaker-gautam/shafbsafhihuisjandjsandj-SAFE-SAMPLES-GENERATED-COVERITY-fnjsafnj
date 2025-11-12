//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_IDENTIFIER,
    TOKEN_EOF,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
};

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

int tokenize_expression(const char *input, struct Token *tokens, int max_tokens) {
    int token_count = 0;
    const char *ptr = input;
    
    while (*ptr != '\0' && token_count < max_tokens) {
        while (isspace(*ptr)) ptr++;
        
        if (*ptr == '\0') break;
        
        if (isdigit(*ptr)) {
            int i = 0;
            while (isdigit(*ptr) && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *ptr++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        } else if (isalpha(*ptr)) {
            int i = 0;
            while (isalnum(*ptr) && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *ptr++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
        } else if (is_operator(*ptr)) {
            tokens[token_count].value[0] = *ptr++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        } else {
            tokens[token_count].value[0] = *ptr++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_INVALID;
            token_count++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "OPERATOR", "IDENTIFIER", "EOF", "INVALID"
    };
    printf("Type: %-12s Value: '%s'\n", type_names[token.type], token.value);
}

int main(void) {
    char input[512];
    struct Token tokens[MAX_TOKENS];
    
    printf("Enter an expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int token_count = tokenize_expression(input, tokens, MAX_TOKENS);
    
    printf("\nTokenized expression:\n");
    for (int i = 0; i < token_count; i++) {
        print_token(tokens[i]);
    }
    
    int num_count = 0;
    int op_count = 0;
    int id_count = 0;
    int invalid_count = 0;
    
    int i = 0;
    while (i < token_count) {
        switch (tokens[i].type) {
            case TOKEN_NUMBER: num_count++; break;
            case TOKEN_OPERATOR: op_count++; break;
            case TOKEN_IDENTIFIER: id_count++; break;
            case TOKEN_INVALID: invalid_count++; break;
            default: break;
        }
        i++;
    }
    
    printf("\nToken statistics:\n");
    printf("Numbers: %d\n", num_count);
    printf("Operators: %d\n", op_count);
    printf("Identifiers: %d\n", id_count);
    printf("Invalid: %d\n", invalid_count);
    
    return 0;
}