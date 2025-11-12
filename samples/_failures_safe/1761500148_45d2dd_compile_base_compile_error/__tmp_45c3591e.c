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

int parse_operator(const char *input, char *output) {
    const char *p = input;
    char *out = output;
    
    if (!is_operator_char(*p)) {
        return 0;
    }
    
    *out++ = *p++;
    
    if ((*p == '=' && (*(p-1) == '=' || *(p-1) == '!' || *(p-1) == '<' || *(p-1) == '>')) ||
        (*p == '&' && *(p-1) == '&') ||
        (*p == '|' && *(p-1) == '|')) {
        *out++ = *p++;
    }
    
    *out = '\0';
    return (int)(p - input);
}

int parse_number(const char *input, char *output) {
    const char *p = input;
    char *out = output;
    
    if (!isdigit(*p)) {
        return 0;
    }
    
    while (isdigit(*p)) {
        if ((size_t)(out - output) >= MAX_TOKEN_LEN - 1) {
            return -1;
        }
        *out++ = *p++;
    }
    
    *out = '\0';
    return (int)(p - input);
}

int parse_string(const char *input, char *output) {
    const char *p = input;
    char *out = output;
    
    if (*p != '"') {
        return 0;
    }
    
    p++;
    while (*p != '"' && *p != '\0') {
        if ((size_t)(out - output) >= MAX_TOKEN_LEN - 1) {
            return -1;
        }
        *out++ = *p++;
    }
    
    if (*p != '"') {
        return -1;
    }
    
    p++;
    *out = '\0';
    return (int)(p - input);
}

int parse_identifier(const char *input, char *output) {
    const char *p = input;
    char *out = output;
    
    if (!isalpha(*p) && *p != '_') {
        return 0;
    }
    
    while (isalnum(*p) || *p == '_') {
        if ((size_t)(out - output) >= MAX_TOKEN_LEN - 1) {
            return -1;
        }
        *out++ = *p++;
    }
    
    *out = '\0';
    return (int)(p - input);
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    const char *p = input;
    Token *current = tokens;
    int token_count = 0;
    
    while (*p != '\0' && token_count < max_tokens) {
        while (isspace(*p)) {
            p++;
        }
        
        if (*p == '\0') {
            break;
        }
        
        int len;
        
        if ((len = parse_operator(p, current->value)) > 0) {
            current->type = TOKEN_OPERATOR;
            p += len;
        } else if ((len = parse_string(p, current->value)) > 0) {
            current->type = TOKEN_STRING;
            p += len;
        } else if ((len = parse_number(p, current->value)) > 0) {
            current->type = TOKEN_NUMBER;
            p += len;
        } else if ((len = parse_identifier(p, current->value)) > 0) {
            current->type = TOKEN_IDENTIFIER;
            p += len;
        } else {
            current->type = TOKEN_ERROR;
            strncpy(current->value, "UNKNOWN", MAX_TOKEN_LEN - 1);
            current->value[MAX_TOKEN_LEN - 1] = '\0';
            p++;
        }
        
        token_count++;
        current++;
    }
    
    if (token_count < max_tokens) {
        current->type = TOKEN_EOF;
        current->value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

void print_tokens(const Token *tokens, int count) {
    const char *type_names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    
    for (int i = 0; i < count; i++) {
        printf("%-12s: %s\n", type_names[tokens[i].type], tokens[i].value);
        if (tokens[i].type == TOKEN_EOF) {
            break;
        }
    }
}

int main(void) {
    char input[1024];
    Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof