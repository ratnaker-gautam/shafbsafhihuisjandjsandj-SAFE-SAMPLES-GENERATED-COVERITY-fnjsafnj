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
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '(' || c == ')';
}

int parse_number(const char *str, Token *token) {
    char *endptr;
    strtod(str, &endptr);
    if (endptr != str && *endptr == '\0') {
        strncpy(token->value, str, MAX_TOKEN_LEN - 1);
        token->value[MAX_TOKEN_LEN - 1] = '\0';
        token->type = TOKEN_NUMBER;
        return 1;
    }
    return 0;
}

int parse_identifier(const char *str, Token *token) {
    if (!isalpha((unsigned char)str[0])) return 0;
    
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalnum((unsigned char)str[i])) return 0;
    }
    
    strncpy(token->value, str, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int tokenize(const char *input, Token tokens[], int *token_count) {
    char buffer[MAX_TOKEN_LEN];
    int buf_pos = 0;
    *token_count = 0;
    
    for (int i = 0; input[i] != '\0' && *token_count < MAX_TOKENS; i++) {
        if (strchr(DELIMITERS, input[i])) {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                if (!parse_number(buffer, &tokens[*token_count]) && 
                    !parse_identifier(buffer, &tokens[*token_count])) {
                    tokens[*token_count].type = TOKEN_UNKNOWN;
                    strncpy(tokens[*token_count].value, buffer, MAX_TOKEN_LEN - 1);
                    tokens[*token_count].value[MAX_TOKEN_LEN - 1] = '\0';
                }
                (*token_count)++;
                buf_pos = 0;
            }
            continue;
        }
        
        if (is_operator(input[i])) {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                if (!parse_number(buffer, &tokens[*token_count]) && 
                    !parse_identifier(buffer, &tokens[*token_count])) {
                    tokens[*token_count].type = TOKEN_UNKNOWN;
                    strncpy(tokens[*token_count].value, buffer, MAX_TOKEN_LEN - 1);
                    tokens[*token_count].value[MAX_TOKEN_LEN - 1] = '\0';
                }
                (*token_count)++;
                buf_pos = 0;
            }
            
            if (*token_count < MAX_TOKENS) {
                tokens[*token_count].type = TOKEN_OPERATOR;
                tokens[*token_count].value[0] = input[i];
                tokens[*token_count].value[1] = '\0';
                (*token_count)++;
            }
            continue;
        }
        
        if (buf_pos < MAX_TOKEN_LEN - 1) {
            buffer[buf_pos++] = input[i];
        }
    }
    
    if (buf_pos > 0 && *token_count < MAX_TOKENS) {
        buffer[buf_pos] = '\0';
        if (!parse_number(buffer, &tokens[*token_count]) && 
            !parse_identifier(buffer, &tokens[*token_count])) {
            tokens[*token_count].type = TOKEN_UNKNOWN;
            strncpy(tokens[*token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[*token_count].value[MAX_TOKEN_LEN - 1] = '\0';
        }
        (*token_count)++;
    }
    
    return 1;
}

void print_tokens(const Token tokens[], int token_count) {
    for (int i = 0; i < token_count; i++) {
        const char *type_str;
        switch (tokens[i].type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            default: type_str = "UNKNOWN"; break;
        }
        printf("Token %d: %s ('%s')\n", i + 1, type_str, tokens[i].value);
    }
}

int main() {
    char input[1024];
    Token tokens[MAX_TOKENS];
    int token_count;
    
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
    
    if