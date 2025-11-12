//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char **input, Token *token) {
    const char *p = *input;
    char *dest = token->value;
    int len = 0;
    
    while (isdigit(*p) && len < MAX_TOKEN_LEN - 1) {
        *dest++ = *p++;
        len++;
    }
    *dest = '\0';
    *input = p;
    return len > 0;
}

int parse_identifier(const char **input, Token *token) {
    const char *p = *input;
    char *dest = token->value;
    int len = 0;
    
    if (isalpha(*p) || *p == '_') {
        *dest++ = *p++;
        len++;
        
        while ((isalnum(*p) || *p == '_') && len < MAX_TOKEN_LEN - 1) {
            *dest++ = *p++;
            len++;
        }
        *dest = '\0';
        *input = p;
        return 1;
    }
    return 0;
}

int parse_operator(const char **input, Token *token) {
    const char *p = *input;
    char *dest = token->value;
    int len = 0;
    
    if (is_operator_char(*p)) {
        *dest++ = *p++;
        len++;
        
        if (is_operator_char(*p) && len < MAX_TOKEN_LEN - 1) {
            *dest++ = *p++;
            len++;
        }
        *dest = '\0';
        *input = p;
        return 1;
    }
    return 0;
}

int parse_string(const char **input, Token *token) {
    const char *p = *input;
    char *dest = token->value;
    int len = 0;
    
    if (*p == '"') {
        p++;
        while (*p != '"' && *p != '\0' && len < MAX_TOKEN_LEN - 1) {
            *dest++ = *p++;
            len++;
        }
        if (*p == '"') {
            p++;
            *dest = '\0';
            *input = p;
            return 1;
        }
    }
    return 0;
}

void tokenize(const char *input, Token *tokens, int *token_count) {
    const char *p = input;
    *token_count = 0;
    
    while (*p != '\0' && *token_count < MAX_TOKENS) {
        while (isspace(*p)) p++;
        
        if (*p == '\0') break;
        
        Token *current_token = tokens + *token_count;
        
        if (parse_number(&p, current_token)) {
            current_token->type = TOKEN_NUMBER;
            (*token_count)++;
        } else if (parse_identifier(&p, current_token)) {
            current_token->type = TOKEN_IDENTIFIER;
            (*token_count)++;
        } else if (parse_operator(&p, current_token)) {
            current_token->type = TOKEN_OPERATOR;
            (*token_count)++;
        } else if (parse_string(&p, current_token)) {
            current_token->type = TOKEN_STRING;
            (*token_count)++;
        } else {
            current_token->type = TOKEN_ERROR;
            current_token->value[0] = *p;
            current_token->value[1] = '\0';
            p++;
            (*token_count)++;
        }
    }
    
    if (*token_count < MAX_TOKENS) {
        tokens[*token_count].type = TOKEN_EOF;
        tokens[*token_count].value[0] = '\0';
        (*token_count)++;
    }
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
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
    
    if (len <= 1) {
        printf("No input provided\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = 0;
    
    tokenize(input, tokens, &token_count);
    
    printf("