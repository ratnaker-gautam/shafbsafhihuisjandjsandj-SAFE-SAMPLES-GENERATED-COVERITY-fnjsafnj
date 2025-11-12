//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_TOKENS 256
#define MAX_TOKEN_LENGTH 64

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LENGTH];
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_number(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int has_decimal = 0;
    
    while (input[*pos] && (isdigit(input[*pos]) || input[*pos] == '.')) {
        if (input[*pos] == '.') {
            if (has_decimal) return 0;
            has_decimal = 1;
        }
        (*pos)++;
    }
    
    int length = *pos - start;
    if (length > 0 && length < MAX_TOKEN_LENGTH) {
        strncpy(token->value, &input[start], length);
        token->value[length] = '\0';
        token->type = TOKEN_NUMBER;
        return 1;
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    
    if (input[*pos] && (isalpha(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
        while (input[*pos] && (isalnum(input[*pos]) || input[*pos] == '_')) {
            (*pos)++;
        }
    }
    
    int length = *pos - start;
    if (length > 0 && length < MAX_TOKEN_LENGTH) {
        strncpy(token->value, &input[start], length);
        token->value[length] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    
    if (input[*pos] && is_operator_char(input[*pos])) {
        (*pos)++;
        if (input[*pos] && is_operator_char(input[*pos])) {
            (*pos)++;
        }
    }
    
    int length = *pos - start;
    if (length > 0 && length < MAX_TOKEN_LENGTH) {
        strncpy(token->value, &input[start], length);
        token->value[length] = '\0';
        token->type = TOKEN_OPERATOR;
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, struct Token *token) {
    if (input[*pos] != '"') return 0;
    
    int start = *pos;
    (*pos)++;
    
    while (input[*pos] && input[*pos] != '"') {
        if (input[*pos] == '\\' && input[*pos + 1]) {
            (*pos) += 2;
        } else {
            (*pos)++;
        }
    }
    
    if (input[*pos] == '"') {
        (*pos)++;
        int length = *pos - start;
        if (length < MAX_TOKEN_LENGTH) {
            strncpy(token->value, &input[start], length);
            token->value[length] = '\0';
            token->type = TOKEN_STRING;
            return 1;
        }
    }
    return 0;
}

void skip_whitespace(const char *input, int *pos) {
    while (input[*pos] && isspace(input[*pos])) {
        (*pos)++;
    }
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    
    while (input[pos] && token_count < max_tokens - 1) {
        skip_whitespace(input, &pos);
        if (!input[pos]) break;
        
        struct Token token;
        int success = 0;
        
        if (parse_number(input, &pos, &token)) success = 1;
        else if (parse_string(input, &pos, &token)) success = 1;
        else if (parse_operator(input, &pos, &token)) success = 1;
        else if (parse_identifier(input, &pos, &token)) success = 1;
        
        if (success) {
            tokens[token_count++] = token;
        } else {
            break;
        }
    }
    
    tokens[token_count].type = TOKEN_EOF;
    tokens[token_count].value[0] = '\0';
    return token_count;
}

const char* token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

int main(void