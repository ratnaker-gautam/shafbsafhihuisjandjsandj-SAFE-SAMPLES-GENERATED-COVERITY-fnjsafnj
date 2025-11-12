//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
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
    if (!input || !*input || !token) return 0;
    
    char *end = NULL;
    double num = strtod(*input, &end);
    if (end == *input) return 0;
    
    int len = end - *input;
    if (len <= 0 || len >= MAX_TOKEN_LEN) return 0;
    
    strncpy(token->value, *input, len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    *input = end;
    return 1;
}

int parse_string(const char **input, Token *token) {
    if (!input || !*input || !token || **input != '"') return 0;
    
    const char *start = *input + 1;
    const char *ptr = start;
    int len = 0;
    
    while (*ptr && *ptr != '"' && len < MAX_TOKEN_LEN - 1) {
        if (*ptr == '\\' && *(ptr + 1)) {
            ptr += 2;
            len += 2;
        } else {
            ptr++;
            len++;
        }
    }
    
    if (*ptr != '"') return 0;
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_STRING;
    *input = ptr + 1;
    return 1;
}

int parse_identifier(const char **input, Token *token) {
    if (!input || !*input || !token) return 0;
    if (!isalpha(**input) && **input != '_') return 0;
    
    const char *start = *input;
    const char *ptr = start + 1;
    int len = 1;
    
    while (*ptr && (isalnum(*ptr) || *ptr == '_')) {
        ptr++;
        len++;
        if (len >= MAX_TOKEN_LEN - 1) break;
    }
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    *input = ptr;
    return 1;
}

int parse_operator(const char **input, Token *token) {
    if (!input || !*input || !token) return 0;
    if (!is_operator_char(**input)) return 0;
    
    const char *start = *input;
    const char *ptr = start + 1;
    int len = 1;
    
    if (*ptr && ((*start == '<' && *ptr == '=') ||
        (*start == '>' && *ptr == '=') ||
        (*start == '!' && *ptr == '=') ||
        (*start == '=' && *ptr == '='))) {
        len = 2;
        ptr++;
    }
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_OPERATOR;
    *input = ptr;
    return 1;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return 0;
    
    int count = 0;
    
    while (*input && count < max_tokens - 1) {
        while (isspace(*input)) input++;
        if (!*input) break;
        
        Token *current = tokens + count;
        
        if (parse_number(&input, current) ||
            parse_string(&input, current) ||
            parse_identifier(&input, current) ||
            parse_operator(&input, current)) {
            count++;
            continue;
        }
        
        current->type = TOKEN_ERROR;
        current->value[0] = *input;
        current->value[1] = '\0';
        input++;
        count++;
    }
    
    if (count < max_tokens) {
        tokens[count].type = TOKEN_EOF;
        tokens[count].value[0] = '\0';
        count++;
    }
    return count;
}

void print_tokens(const Token *tokens, int count) {
    if (!tokens || count <= 0) return;
    
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    
    for (int i = 0; i < count; i++) {
        printf("Token %d: %s -> '%s'\n", i, type_names[tokens[i].type], tokens[i].value);
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin