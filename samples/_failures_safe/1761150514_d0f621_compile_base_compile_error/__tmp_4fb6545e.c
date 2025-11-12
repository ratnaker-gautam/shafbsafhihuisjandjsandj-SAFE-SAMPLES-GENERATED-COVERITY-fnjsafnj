//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[64];
    int line;
    int column;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_number(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    
    while (input[*pos] && (isdigit(input[*pos]) || input[*pos] == '.')) {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*pos)++;
    }
    
    if (*pos - start > 63) return 0;
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    
    (*pos)++;
    while (input[*pos] && (isalnum(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
    }
    
    if (*pos - start > 63) return 0;
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    
    if (!is_operator_char(input[*pos])) return 0;
    
    (*pos)++;
    while (input[*pos] && is_operator_char(input[*pos])) {
        (*pos)++;
    }
    
    if (*pos - start > 63) return 0;
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int parse_string(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    
    if (input[*pos] != '"') return 0;
    
    (*pos)++;
    while (input[*pos] && input[*pos] != '"') {
        if (input[*pos] == '\\' && input[*pos + 1]) {
            (*pos) += 2;
        } else {
            (*pos)++;
        }
    }
    
    if (!input[*pos]) return 0;
    
    (*pos)++;
    
    if (*pos - start > 63) return 0;
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_STRING;
    return 1;
}

int get_next_token(const char *input, int *pos, int *line, int *column, struct Token *token) {
    while (input[*pos]) {
        if (isspace(input[*pos])) {
            if (input[*pos] == '\n') {
                (*line)++;
                *column = 1;
            } else {
                (*column)++;
            }
            (*pos)++;
            continue;
        }
        
        token->line = *line;
        token->column = *column;
        
        if (parse_number(input, pos, token)) {
            *column += (int)strlen(token->value);
            return 1;
        }
        
        if (parse_identifier(input, pos, token)) {
            *column += (int)strlen(token->value);
            return 1;
        }
        
        if (parse_operator(input, pos, token)) {
            *column += (int)strlen(token->value);
            return 1;
        }
        
        if (parse_string(input, pos, token)) {
            *column += (int)strlen(token->value);
            return 1;
        }
        
        token->type = TOKEN_UNKNOWN;
        token->value[0] = input[*pos];
        token->value[1] = '\0';
        (*pos)++;
        (*column)++;
        return 1;
    }
    
    token->type = TOKEN_EOF;
    token->value[0] = '\0';
    return 1;
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

int main(void) {
    char input[1024];
    printf("Enter text to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading