//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
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
    char value[32];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_number(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    
    while (input[*pos] != '\0') {
        if (isdigit(input[*pos])) {
            (*pos)++;
        } else if (input[*pos] == '.' && !has_dot) {
            has_dot = 1;
            (*pos)++;
        } else {
            break;
        }
    }
    
    int length = *pos - start;
    if (length > 31) length = 31;
    
    strncpy(token->value, &input[start], length);
    token->value[length] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_string(const char *input, int *pos, struct Token *token) {
    if (input[*pos] != '"') return 0;
    
    (*pos)++;
    int start = *pos;
    int length = 0;
    
    while (input[*pos] != '\0' && input[*pos] != '"' && length < 31) {
        token->value[length++] = input[*pos];
        (*pos)++;
    }
    
    if (input[*pos] == '"') {
        (*pos)++;
        token->value[length] = '\0';
        token->type = TOKEN_STRING;
        return 1;
    }
    
    return 0;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    
    int start = *pos;
    int length = 0;
    
    while (input[*pos] != '\0' && (isalnum(input[*pos]) || input[*pos] == '_') && length < 31) {
        token->value[length++] = input[*pos];
        (*pos)++;
    }
    
    token->value[length] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    if (!is_operator_char(input[*pos])) return 0;
    
    int start = *pos;
    int length = 0;
    
    while (input[*pos] != '\0' && is_operator_char(input[*pos]) && length < 31) {
        token->value[length++] = input[*pos];
        (*pos)++;
    }
    
    token->value[length] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

void tokenize(const char *input, struct Token *tokens, int max_tokens, int *token_count) {
    int pos = 0;
    int line = 1;
    int column = 1;
    *token_count = 0;
    
    while (input[pos] != '\0' && *token_count < max_tokens - 1) {
        while (isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
        }
        
        if (input[pos] == '\0') break;
        
        tokens[*token_count].line = line;
        tokens[*token_count].column = column;
        
        int found = 0;
        found = parse_number(input, &pos, &tokens[*token_count]);
        if (!found) found = parse_string(input, &pos, &tokens[*token_count]);
        if (!found) found = parse_identifier(input, &pos, &tokens[*token_count]);
        if (!found) found = parse_operator(input, &pos, &tokens[*token_count]);
        
        if (!found) {
            tokens[*token_count].value[0] = input[pos];
            tokens[*token_count].value[1] = '\0';
            tokens[*token_count].type = TOKEN_UNKNOWN;
            pos++;
        }
        
        column += (int)strlen(tokens[*token_count].value);
        (*token_count)++;
    }
    
    tokens[*token_count].type = TOKEN_EOF;
    tokens[*token_count].value[0] = '\0';
    tokens[*token_count].line = line;
    tokens[*token_count].column = column;
    (*token_count)++;
}

const char* token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";