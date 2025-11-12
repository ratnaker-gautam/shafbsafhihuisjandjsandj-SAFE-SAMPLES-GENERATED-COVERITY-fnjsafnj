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
    TOKEN_EOF
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
    
    while (*p && isdigit(*p) && len < MAX_TOKEN_LEN - 1) {
        *dest++ = *p++;
        len++;
    }
    *dest = '\0';
    *input = p;
    token->type = TOKEN_NUMBER;
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
        token->type = TOKEN_IDENTIFIER;
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
        token->type = TOKEN_OPERATOR;
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
            token->type = TOKEN_STRING;
            return 1;
        }
    }
    return 0;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    const char *p = input;
    int token_count = 0;
    
    while (*p && token_count < max_tokens) {
        while (*p && isspace(*p)) {
            p++;
        }
        
        if (!*p) {
            break;
        }
        
        Token *current = tokens + token_count;
        
        if (parse_number(&p, current) ||
            parse_identifier(&p, current) ||
            parse_operator(&p, current) ||
            parse_string(&p, current)) {
            token_count++;
        } else {
            break;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "");
        token_count++;
    }
    
    return token_count;
}

void print_tokens(const Token *tokens, int count) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"
    };
    
    for (int i = 0; i < count; i++) {
        printf("Token %d: %s -> '%s'\n", i, type_names[tokens[i].type], tokens[i].value);
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
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count >= MAX_TOKENS) {
        fprintf(stderr, "Too many tokens\n");
        return 1;
    }
    
    printf("Tokenized %d tokens:\n", token_count);
    print_tokens(tokens, token_count);
    
    return 0;
}