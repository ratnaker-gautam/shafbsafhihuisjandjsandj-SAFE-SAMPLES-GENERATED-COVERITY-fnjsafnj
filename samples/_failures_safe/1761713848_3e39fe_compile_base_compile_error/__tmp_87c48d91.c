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
    TOKEN_INVALID
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_number(const char **input, Token *token) {
    const char *p = *input;
    char *dest = token->value;
    int len = 0;
    
    while (*p && isdigit(*p) && len < MAX_TOKEN_LEN - 1) {
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
    
    if (*p && (isalpha(*p) || *p == '_')) {
        *dest++ = *p++;
        len++;
        
        while (*p && (isalnum(*p) || *p == '_') && len < MAX_TOKEN_LEN - 1) {
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
    
    if (*p && is_operator_char(*p)) {
        *dest++ = *p++;
        len++;
        
        if (*p && is_operator_char(*p) && len < MAX_TOKEN_LEN - 1) {
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
        while (*p && *p != '"' && len < MAX_TOKEN_LEN - 1) {
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

int tokenize(const char *input, Token *tokens, int max_tokens) {
    const char *p = input;
    Token *current = tokens;
    int token_count = 0;
    
    while (*p && token_count < max_tokens) {
        while (*p && isspace(*p)) {
            p++;
        }
        
        if (!*p) {
            break;
        }
        
        if (parse_string(&p, current)) {
            current->type = TOKEN_STRING;
        } else if (parse_number(&p, current)) {
            current->type = TOKEN_NUMBER;
        } else if (parse_identifier(&p, current)) {
            current->type = TOKEN_IDENTIFIER;
        } else if (parse_operator(&p, current)) {
            current->type = TOKEN_OPERATOR;
        } else {
            current->type = TOKEN_INVALID;
            current->value[0] = *p;
            current->value[1] = '\0';
            p++;
        }
        
        current++;
        token_count++;
    }
    
    if (token_count < max_tokens) {
        current->type = TOKEN_EOF;
        current->value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

void print_token_type(TokenType type) {
    const char *names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF", "INVALID"
    };
    if (type >= TOKEN_IDENTIFIER && type <= TOKEN_INVALID) {
        printf("%s", names[type]);
    }
}

int main(void) {
    char input[1024];
    Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("Tokens found: %d\n", token_count);
    printf("Type\t\tValue\n");
    printf("----\t\t-----\n");