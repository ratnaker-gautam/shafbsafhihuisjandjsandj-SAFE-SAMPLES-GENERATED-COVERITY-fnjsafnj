//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
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
    const char* operators = "+-*/=<>!&|";
    for (const char* op = operators; *op != '\0'; op++) {
        if (c == *op) return 1;
    }
    return 0;
}

int parse_number(const char* input, int* pos, struct Token* token) {
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
    if (length > 63) length = 63;
    
    strncpy(token->value, input + start, length);
    token->value[length] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(const char* input, int* pos, struct Token* token) {
    int start = *pos;
    
    if (!isalpha(input[*pos]) && input[*pos] != '_') {
        return 0;
    }
    
    (*pos)++;
    while (input[*pos] != '\0' && (isalnum(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
    }
    
    int length = *pos - start;
    if (length > 63) length = 63;
    
    strncpy(token->value, input + start, length);
    token->value[length] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_string(const char* input, int* pos, struct Token* token) {
    if (input[*pos] != '"') return 0;
    
    int start = *pos;
    (*pos)++;
    
    while (input[*pos] != '\0' && input[*pos] != '"') {
        if (input[*pos] == '\\' && input[*pos + 1] != '\0') {
            (*pos) += 2;
        } else {
            (*pos)++;
        }
    }
    
    if (input[*pos] == '"') {
        (*pos)++;
        int length = *pos - start;
        if (length > 63) length = 63;
        
        strncpy(token->value, input + start, length);
        token->value[length] = '\0';
        token->type = TOKEN_STRING;
        return 1;
    }
    
    return 0;
}

int parse_operator(const char* input, int* pos, struct Token* token) {
    if (!is_operator_char(input[*pos])) return 0;
    
    int start = *pos;
    (*pos)++;
    
    while (is_operator_char(input[*pos]) && (*pos - start) < 2) {
        (*pos)++;
    }
    
    int length = *pos - start;
    if (length > 63) length = 63;
    
    strncpy(token->value, input + start, length);
    token->value[length] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

void tokenize(const char* input, struct Token* tokens, int max_tokens, int* token_count) {
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
        
        struct Token token;
        token.line = line;
        token.column = column;
        
        int found = 0;
        int start_pos = pos;
        
        if (parse_number(input, &pos, &token)) found = 1;
        else if (parse_string(input, &pos, &token)) found = 1;
        else if (parse_identifier(input, &pos, &token)) found = 1;
        else if (parse_operator(input, &pos, &token)) found = 1;
        
        if (!found) {
            token.type = TOKEN_UNKNOWN;
            token.value[0] = input[pos];
            token.value[1] = '\0';
            pos++;
        }
        
        if (found) {
            column += (pos - start_pos);
        } else {
            column++;
        }
        
        tokens[(*token_count)++] = token;
    }
    
    if (*token_count < max_tokens) {
        tokens[*token_count].type = TOKEN_EOF;
        tokens[*token_count].value[0] = '\0';
        tokens[*token_count].line = line;
        tokens