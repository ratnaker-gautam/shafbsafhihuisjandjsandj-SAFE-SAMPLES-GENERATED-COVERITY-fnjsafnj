//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

Token* tokenize(const char* input, int* token_count) {
    static Token tokens[MAX_TOKENS];
    *token_count = 0;
    const char* p = input;
    
    while (*p != '\0' && *token_count < MAX_TOKENS) {
        while (isspace(*p)) p++;
        
        if (*p == '\0') break;
        
        if (isalpha(*p) || *p == '_') {
            char* dest = tokens[*token_count].value;
            int i = 0;
            while ((isalnum(*p) || *p == '_') && i < MAX_TOKEN_LEN - 1) {
                *dest++ = *p++;
                i++;
            }
            *dest = '\0';
            tokens[*token_count].type = TOKEN_IDENTIFIER;
            (*token_count)++;
        }
        else if (isdigit(*p)) {
            char* dest = tokens[*token_count].value;
            int i = 0;
            while (isdigit(*p) && i < MAX_TOKEN_LEN - 1) {
                *dest++ = *p++;
                i++;
            }
            *dest = '\0';
            tokens[*token_count].type = TOKEN_NUMBER;
            (*token_count)++;
        }
        else if (*p == '"') {
            char* dest = tokens[*token_count].value;
            int i = 0;
            p++;
            while (*p != '"' && *p != '\0' && i < MAX_TOKEN_LEN - 1) {
                *dest++ = *p++;
                i++;
            }
            *dest = '\0';
            if (*p == '"') p++;
            tokens[*token_count].type = TOKEN_STRING;
            (*token_count)++;
        }
        else if (is_operator_char(*p)) {
            char* dest = tokens[*token_count].value;
            int i = 0;
            while (is_operator_char(*p) && i < MAX_TOKEN_LEN - 1) {
                *dest++ = *p++;
                i++;
            }
            *dest = '\0';
            tokens[*token_count].type = TOKEN_OPERATOR;
            (*token_count)++;
        }
        else {
            tokens[*token_count].value[0] = *p;
            tokens[*token_count].value[1] = '\0';
            tokens[*token_count].type = TOKEN_ERROR;
            (*token_count)++;
            p++;
        }
    }
    
    if (*token_count < MAX_TOKENS) {
        tokens[*token_count].type = TOKEN_EOF;
        strcpy(tokens[*token_count].value, "EOF");
        (*token_count)++;
    }
    
    return tokens;
}

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main() {
    char input[1024];
    
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
        printf("No input provided\n");
        return 1;
    }
    
    int token_count;
    Token* tokens = tokenize(input, &token_count);
    
    printf("\nTokens found:\n");
    printf("%-12s %-12s %s\n", "Index", "Type", "Value");
    printf("------------ ------------ ------------\n");
    
    for (int i = 0; i < token_count; i++) {
        printf("%-12d %-12s %-12s\n", 
               i, 
               token_type_to_string(tokens[i].type),
               tokens[i].value);
    }
    
    return 0;
}