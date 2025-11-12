//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
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
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void tokenize(const char *input, struct Token *tokens, int max_tokens, int *token_count) {
    const char *p = input;
    *token_count = 0;
    
    while (*p && *token_count < max_tokens) {
        while (isspace((unsigned char)*p)) p++;
        
        if (!*p) break;
        
        if (isdigit((unsigned char)*p)) {
            int i = 0;
            while (isdigit((unsigned char)*p) && i < 31) {
                tokens[*token_count].value[i++] = *p++;
            }
            tokens[*token_count].value[i] = '\0';
            tokens[*token_count].type = TOKEN_NUMBER;
            (*token_count)++;
        }
        else if (isalpha((unsigned char)*p) || *p == '_') {
            int i = 0;
            while ((isalnum((unsigned char)*p) || *p == '_') && i < 31) {
                tokens[*token_count].value[i++] = *p++;
            }
            tokens[*token_count].value[i] = '\0';
            tokens[*token_count].type = TOKEN_IDENTIFIER;
            (*token_count)++;
        }
        else if (*p == '"') {
            int i = 0;
            p++;
            while (*p && *p != '"' && i < 31) {
                tokens[*token_count].value[i++] = *p++;
            }
            if (*p == '"') {
                p++;
            }
            tokens[*token_count].value[i] = '\0';
            tokens[*token_count].type = TOKEN_STRING;
            (*token_count)++;
        }
        else if (is_operator_char(*p)) {
            tokens[*token_count].value[0] = *p++;
            tokens[*token_count].value[1] = '\0';
            tokens[*token_count].type = TOKEN_OPERATOR;
            (*token_count)++;
        }
        else {
            tokens[*token_count].value[0] = *p++;
            tokens[*token_count].value[1] = '\0';
            tokens[*token_count].type = TOKEN_INVALID;
            (*token_count)++;
        }
    }
    
    if (*token_count < max_tokens) {
        tokens[*token_count].type = TOKEN_EOF;
        strcpy(tokens[*token_count].value, "");
        (*token_count)++;
    }
}

void print_token(struct Token token) {
    switch (token.type) {
        case TOKEN_NUMBER:
            printf("NUMBER: %s\n", token.value);
            break;
        case TOKEN_IDENTIFIER:
            printf("IDENTIFIER: %s\n", token.value);
            break;
        case TOKEN_OPERATOR:
            printf("OPERATOR: %s\n", token.value);
            break;
        case TOKEN_STRING:
            printf("STRING: \"%s\"\n", token.value);
            break;
        case TOKEN_EOF:
            printf("EOF\n");
            break;
        case TOKEN_INVALID:
            printf("INVALID: %s\n", token.value);
            break;
    }
}

int main(void) {
    char input[256];
    struct Token tokens[50];
    int token_count;
    
    printf("Enter expression to tokenize: ");
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
    
    tokenize(input, tokens, 50, &token_count);
    
    printf("\nTokens:\n");
    for (int i = 0; i < token_count; i++) {
        printf("%d: ", i + 1);
        print_token(tokens[i]);
    }
    
    return 0;
}