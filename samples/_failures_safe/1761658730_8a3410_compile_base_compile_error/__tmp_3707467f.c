//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_number(const char **input, struct Token *token, int line, int col) {
    char *end = NULL;
    double val = strtod(*input, &end);
    if (end == *input) return 0;
    
    int len = end - *input;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    strncpy(token->value, *input, len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    token->line = line;
    token->column = col;
    *input = end;
    return 1;
}

int parse_string(const char **input, struct Token *token, int line, int col) {
    if (**input != '"') return 0;
    
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
    token->line = line;
    token->column = col;
    *input = ptr + 1;
    return 1;
}

int parse_identifier(const char **input, struct Token *token, int line, int col) {
    if (!isalpha(**input) && **input != '_') return 0;
    
    const char *start = *input;
    const char *ptr = start + 1;
    int len = 1;
    
    while (isalnum(*ptr) || *ptr == '_') {
        ptr++;
        len++;
        if (len >= MAX_TOKEN_LEN - 1) break;
    }
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    token->line = line;
    token->column = col;
    *input = ptr;
    return 1;
}

int parse_operator(const char **input, struct Token *token, int line, int col) {
    if (!is_operator_char(**input)) return 0;
    
    const char *start = *input;
    const char *ptr = start + 1;
    int len = 1;
    
    if ((*start == '&' && *ptr == '&') || (*start == '|' && *ptr == '|') ||
        (*start == '=' && *ptr == '=') || (*start == '!' && *ptr == '=') ||
        (*start == '<' && *ptr == '=') || (*start == '>' && *ptr == '=')) {
        len = 2;
        ptr++;
    }
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_OPERATOR;
    token->line = line;
    token->column = col;
    *input = ptr;
    return 1;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int count = 0;
    int line = 1;
    int col = 1;
    
    while (*input && count < max_tokens) {
        while (isspace(*input)) {
            if (*input == '\n') {
                line++;
                col = 1;
            } else {
                col++;
            }
            input++;
        }
        
        if (!*input) break;
        
        struct Token *token = tokens + count;
        
        if (parse_number(&input, token, line, col) ||
            parse_string(&input, token, line, col) ||
            parse_identifier(&input, token, line, col) ||
            parse_operator(&input, token, line, col)) {
            col += strlen(token->value);
            count++;
        } else {
            input++;
            col++;
        }
    }
    
    if (count < max_tokens) {
        tokens[count].type = TOKEN_EOF;
        tokens[count].value[0] = '\0';
        tokens[count].line = line;
        tokens[count].column = col;
        count++;
    }
    
    return count;
}

void print_token(const struct Token *token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"
    };
    printf("Line %d, Col %d: %s '%s