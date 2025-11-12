//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

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
    char *end = token->value;
    
    while (*p && isdigit(*p)) {
        if ((size_t)(end - token->value) >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
        *end++ = *p++;
    }
    
    *end = '\0';
    token->type = TOKEN_NUMBER;
    *input = p;
    return 1;
}

int parse_identifier(const char **input, Token *token) {
    const char *p = *input;
    char *end = token->value;
    
    if (!isalpha(*p) && *p != '_') {
        return 0;
    }
    
    while (*p && (isalnum(*p) || *p == '_')) {
        if ((size_t)(end - token->value) >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
        *end++ = *p++;
    }
    
    *end = '\0';
    token->type = TOKEN_IDENTIFIER;
    *input = p;
    return 1;
}

int parse_operator(const char **input, Token *token) {
    const char *p = *input;
    char *end = token->value;
    
    if (!is_operator_char(*p)) {
        return 0;
    }
    
    *end++ = *p++;
    
    if (is_operator_char(*p) && (*(p - 1) == '<' || *(p - 1) == '>' || *(p - 1) == '!' || *(p - 1) == '=')) {
        if ((size_t)(end - token->value) >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
        *end++ = *p++;
    }
    
    *end = '\0';
    token->type = TOKEN_OPERATOR;
    *input = p;
    return 1;
}

int parse_string(const char **input, Token *token) {
    const char *p = *input;
    char *end = token->value;
    
    if (*p != '"') {
        return 0;
    }
    
    p++;
    while (*p && *p != '"') {
        if ((size_t)(end - token->value) >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
        *end++ = *p++;
    }
    
    if (*p != '"') {
        return 0;
    }
    
    p++;
    *end = '\0';
    token->type = TOKEN_STRING;
    *input = p;
    return 1;
}

int tokenize(const char *input, Token *tokens, size_t max_tokens) {
    const char *p = input;
    Token *current = tokens;
    Token *end = tokens + max_tokens;
    
    while (*p && current < end) {
        while (*p && isspace(*p)) {
            p++;
        }
        
        if (!*p) {
            break;
        }
        
        if (parse_number(&p, current)) {
            current++;
            continue;
        }
        
        if (parse_identifier(&p, current)) {
            current++;
            continue;
        }
        
        if (parse_operator(&p, current)) {
            current++;
            continue;
        }
        
        if (parse_string(&p, current)) {
            current++;
            continue;
        }
        
        current->type = TOKEN_ERROR;
        strncpy(current->value, "Unexpected character", MAX_TOKEN_LEN - 1);
        current->value[MAX_TOKEN_LEN - 1] = '\0';
        current++;
        p++;
    }
    
    if (current < end) {
        current->type = TOKEN_EOF;
        current->value[0] = '\0';
        current++;
    }
    
    return (int)(current - tokens);
}

void print_token_type(TokenType type) {
    const char *names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    if (type >= TOKEN_NUMBER && type <= TOKEN_ERROR) {
        printf("%s", names[type]);
    } else {
        printf("UNKNOWN");
    }
}

int main(void) {
    char input[1024];
    Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if