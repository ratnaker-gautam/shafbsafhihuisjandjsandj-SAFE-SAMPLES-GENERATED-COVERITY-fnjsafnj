//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
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

int parse_number(const char **input, Token *token) {
    const char *p = *input;
    char *dest = token->value;
    
    while (*p && isdigit(*p) && (size_t)(dest - token->value) < MAX_TOKEN_LEN - 1) {
        *dest++ = *p++;
    }
    *dest = '\0';
    *input = p;
    return 1;
}

int parse_identifier(const char **input, Token *token) {
    const char *p = *input;
    char *dest = token->value;
    
    if (!isalpha(*p) && *p != '_') return 0;
    
    *dest++ = *p++;
    while (*p && (isalnum(*p) || *p == '_') && (size_t)(dest - token->value) < MAX_TOKEN_LEN - 1) {
        *dest++ = *p++;
    }
    *dest = '\0';
    *input = p;
    return 1;
}

int parse_operator(const char **input, Token *token) {
    const char *p = *input;
    char *dest = token->value;
    
    if (!is_operator_char(*p)) return 0;
    
    *dest++ = *p++;
    if (is_operator_char(*p) && (size_t)(dest - token->value) < MAX_TOKEN_LEN - 1) {
        *dest++ = *p++;
    }
    *dest = '\0';
    *input = p;
    return 1;
}

int parse_string(const char **input, Token *token) {
    const char *p = *input;
    char *dest = token->value;
    
    if (*p != '"') return 0;
    
    *dest++ = *p++;
    while (*p && *p != '"' && (size_t)(dest - token->value) < MAX_TOKEN_LEN - 1) {
        *dest++ = *p++;
    }
    if (*p == '"' && (size_t)(dest - token->value) < MAX_TOKEN_LEN - 1) {
        *dest++ = *p++;
    } else {
        return 0;
    }
    *dest = '\0';
    *input = p;
    return 1;
}

Token get_next_token(const char **input) {
    Token token = {TOKEN_ERROR, ""};
    const char *p = *input;
    
    while (*p && isspace(*p)) p++;
    
    if (!*p) {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        *input = p;
        return token;
    }
    
    if (parse_number(&p, &token)) {
        token.type = TOKEN_NUMBER;
    } else if (parse_identifier(&p, &token)) {
        token.type = TOKEN_IDENTIFIER;
    } else if (parse_operator(&p, &token)) {
        token.type = TOKEN_OPERATOR;
    } else if (parse_string(&p, &token)) {
        token.type = TOKEN_STRING;
    } else {
        token.value[0] = *p;
        token.value[1] = '\0';
        p++;
    }
    
    *input = p;
    return token;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    const char *p = input;
    int count = 0;
    
    while (count < max_tokens) {
        Token token = get_next_token(&p);
        tokens[count] = token;
        count++;
        
        if (token.type == TOKEN_EOF || token.type == TOKEN_ERROR) {
            break;
        }
    }
    
    return count;
}

void print_token_type(TokenType type) {
    const char *names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    if (type >= TOKEN_IDENTIFIER && type <= TOKEN_ERROR) {
        printf("%s", names[type]);
    } else {
        printf("UNKNOWN");
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
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
    
    Token tokens[MAX_TOK