//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50
#define DELIMITERS " \t\n"

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void tokenize(const char* input, Token* tokens, int* token_count) {
    const char* ptr = input;
    *token_count = 0;
    
    while (*ptr && *token_count < MAX_TOKENS) {
        while (*ptr && strchr(DELIMITERS, *ptr)) ptr++;
        
        if (!*ptr) break;
        
        if (isdigit(*ptr)) {
            int i = 0;
            while (isdigit(*ptr) && i < MAX_TOKEN_LEN - 1) {
                tokens[*token_count].value[i++] = *ptr++;
            }
            tokens[*token_count].value[i] = '\0';
            tokens[*token_count].type = TOKEN_NUMBER;
            (*token_count)++;
        }
        else if (isalpha(*ptr) || *ptr == '_') {
            int i = 0;
            while ((isalnum(*ptr) || *ptr == '_') && i < MAX_TOKEN_LEN - 1) {
                tokens[*token_count].value[i++] = *ptr++;
            }
            tokens[*token_count].value[i] = '\0';
            tokens[*token_count].type = TOKEN_IDENTIFIER;
            (*token_count)++;
        }
        else if (*ptr == '"') {
            int i = 0;
            ptr++;
            while (*ptr && *ptr != '"' && i < MAX_TOKEN_LEN - 1) {
                tokens[*token_count].value[i++] = *ptr++;
            }
            tokens[*token_count].value[i] = '\0';
            if (*ptr == '"') ptr++;
            tokens[*token_count].type = TOKEN_STRING;
            (*token_count)++;
        }
        else if (is_operator(*ptr)) {
            tokens[*token_count].value[0] = *ptr;
            tokens[*token_count].value[1] = '\0';
            tokens[*token_count].type = TOKEN_OPERATOR;
            (*token_count)++;
            ptr++;
        }
        else {
            tokens[*token_count].value[0] = *ptr;
            tokens[*token_count].value[1] = '\0';
            tokens[*token_count].type = TOKEN_UNKNOWN;
            (*token_count)++;
            ptr++;
        }
    }
}

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_UNKNOWN: return "UNKNOWN";
        default: return "INVALID";
    }
}

int main() {
    char input[1024];
    Token tokens[MAX_TOKENS];
    int token_count = 0;
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    tokenize(input, tokens, &token_count);
    
    if (token_count == 0) {
        printf("No tokens found\n");
        return 0;
    }
    
    printf("Tokens found: %d\n", token_count);
    printf("Type\t\tValue\n");
    printf("----\t\t-----\n");
    
    for (int i = 0; i < token_count; i++) {
        printf("%-12s\t%s\n", token_type_to_string(tokens[i].type), tokens[i].value);
    }
    
    return 0;
}