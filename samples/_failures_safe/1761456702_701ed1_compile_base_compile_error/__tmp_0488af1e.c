//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
#define DELIMITERS " \t\n\r"
#define IS_OPERATOR(c) (strchr("+-*/=<>!&|", c) != NULL)
#define IS_PUNCTUATOR(c) (strchr("(),;{}[]", c) != NULL)

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_PUNCTUATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

Token tokens[MAX_TOKENS];
size_t token_count = 0;
size_t current_token = 0;

void add_token(TokenType type, const char* value) {
    if (token_count >= MAX_TOKENS) return;
    Token* token = &tokens[token_count++];
    token->type = type;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
}

int is_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

int parse_number(const char* input, size_t* pos) {
    size_t start = *pos;
    while (isdigit(input[*pos])) (*pos)++;
    if (input[*pos] == '.') {
        (*pos)++;
        while (isdigit(input[*pos])) (*pos)++;
    }
    return *pos > start;
}

int parse_string(const char* input, size_t* pos) {
    if (input[*pos] != '"') return 0;
    size_t start = *pos;
    (*pos)++;
    while (input[*pos] && input[*pos] != '"') {
        if (input[*pos] == '\\' && input[*pos + 1]) (*pos) += 2;
        else (*pos)++;
    }
    if (input[*pos] == '"') {
        (*pos)++;
        return 1;
    }
    *pos = start;
    return 0;
}

int parse_identifier(const char* input, size_t* pos) {
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    size_t start = *pos;
    while (is_identifier_char(input[*pos])) (*pos)++;
    return *pos > start;
}

int parse_operator(const char* input, size_t* pos) {
    if (!IS_OPERATOR(input[*pos])) return 0;
    size_t start = *pos;
    if (input[*pos + 1] && IS_OPERATOR(input[*pos + 1])) (*pos) += 2;
    else (*pos) += 1;
    return *pos > start;
}

void tokenize(const char* input) {
    token_count = 0;
    size_t pos = 0;
    size_t len = strlen(input);
    
    while (pos < len && token_count < MAX_TOKENS) {
        while (pos < len && strchr(DELIMITERS, input[pos])) pos++;
        if (pos >= len) break;
        
        size_t start = pos;
        
        if (parse_string(input, &pos)) {
            char str[MAX_TOKEN_LEN];
            size_t str_len = pos - start;
            if (str_len >= MAX_TOKEN_LEN) str_len = MAX_TOKEN_LEN - 1;
            strncpy(str, input + start, str_len);
            str[str_len] = '\0';
            add_token(TOKEN_STRING, str);
            continue;
        }
        
        if (parse_number(input, &pos)) {
            char num[MAX_TOKEN_LEN];
            size_t num_len = pos - start;
            if (num_len >= MAX_TOKEN_LEN) num_len = MAX_TOKEN_LEN - 1;
            strncpy(num, input + start, num_len);
            num[num_len] = '\0';
            add_token(TOKEN_NUMBER, num);
            continue;
        }
        
        if (parse_identifier(input, &pos)) {
            char ident[MAX_TOKEN_LEN];
            size_t ident_len = pos - start;
            if (ident_len >= MAX_TOKEN_LEN) ident_len = MAX_TOKEN_LEN - 1;
            strncpy(ident, input + start, ident_len);
            ident[ident_len] = '\0';
            add_token(TOKEN_IDENTIFIER, ident);
            continue;
        }
        
        if (parse_operator(input, &pos)) {
            char op[MAX_TOKEN_LEN];
            size_t op_len = pos - start;
            if (op_len >= MAX_TOKEN_LEN) op_len = MAX_TOKEN_LEN - 1;
            strncpy(op, input + start, op_len);
            op[op_len] = '\0';
            add_token(TOKEN_OPERATOR, op);
            continue;
        }
        
        if (IS_PUNCTUATOR(input[pos])) {
            char punct[2] = {input[pos++], '\0'};
            add_token(TOKEN_PUNCTUATOR, punct);
            continue;
        }
        
        char error[2] = {input[pos++], '\0'};
        add_token(TOKEN_ERROR, error);
    }
    
    add_token(TOKEN_EOF, "");
}

const char* token_type_name(TokenType type)