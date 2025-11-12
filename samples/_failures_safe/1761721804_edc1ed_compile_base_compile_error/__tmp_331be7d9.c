//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(const char *input, int *pos) {
    while (input[*pos] != '\0' && isspace(input[*pos])) {
        (*pos)++;
    }
}

int parse_number(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    
    while (input[*pos] != '\0' && (isdigit(input[*pos]) || input[*pos] == '.')) {
        if (input[*pos] == '.') {
            if (has_dot) {
                return 0;
            }
            has_dot = 1;
        }
        (*pos)++;
    }
    
    int len = *pos - start;
    if (len > 0 && len < MAX_TOKEN_LEN) {
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, struct Token *token) {
    if (input[*pos] != '"') {
        return 0;
    }
    
    (*pos)++;
    int start = *pos;
    
    while (input[*pos] != '\0' && input[*pos] != '"') {
        (*pos)++;
    }
    
    if (input[*pos] == '"') {
        int len = *pos - start;
        if (len >= 0 && len < MAX_TOKEN_LEN) {
            strncpy(token->value, &input[start], len);
            token->value[len] = '\0';
            token->type = TOKEN_STRING;
            (*pos)++;
            return 1;
        }
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    if (input[*pos] == '\0' || !isalpha(input[*pos])) {
        return 0;
    }
    
    int start = *pos;
    while (input[*pos] != '\0' && (isalnum(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
    }
    
    int len = *pos - start;
    if (len > 0 && len < MAX_TOKEN_LEN) {
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    if (input[*pos] == '\0' || !is_operator_char(input[*pos])) {
        return 0;
    }
    
    int start = *pos;
    while (input[*pos] != '\0' && is_operator_char(input[*pos])) {
        (*pos)++;
    }
    
    int len = *pos - start;
    if (len > 0 && len < MAX_TOKEN_LEN) {
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_OPERATOR;
        return 1;
    }
    return 0;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int line = 1;
    int column = 1;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        skip_whitespace(input, &pos);
        if (input[pos] == '\0') {
            break;
        }
        
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        
        int old_pos = pos;
        
        if (parse_number(input, &pos, &tokens[token_count]) ||
            parse_string(input, &pos, &tokens[token_count]) ||
            parse_identifier(input, &pos, &tokens[token_count]) ||
            parse_operator(input, &pos, &tokens[token_count])) {
            
            token_count++;
            column += (pos - old_pos);
            continue;
        }
        
        pos++;
        column++;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens