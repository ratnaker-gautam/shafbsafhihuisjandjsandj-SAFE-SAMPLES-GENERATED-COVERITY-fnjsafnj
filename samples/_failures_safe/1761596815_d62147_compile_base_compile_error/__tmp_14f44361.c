//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
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
    TOKEN_EOF,
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_number(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    
    while (isdigit(input[*pos]) || input[*pos] == '.') {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*pos)++;
    }
    
    if (*pos - start > 0) {
        int len = *pos - start;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        return 1;
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        (*pos)++;
        while (isalnum(input[*pos]) || input[*pos] == '_') {
            (*pos)++;
        }
        
        int len = *pos - start;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    
    if (is_operator_char(input[*pos])) {
        (*pos)++;
        while (is_operator_char(input[*pos]) && (*pos - start) < (MAX_TOKEN_LEN - 1)) {
            (*pos)++;
        }
        
        int len = *pos - start;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_OPERATOR;
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    
    if (input[*pos] == '"') {
        (*pos)++;
        while (input[*pos] != '"' && input[*pos] != '\0') {
            if (input[*pos] == '\\' && input[*pos + 1] != '\0') {
                (*pos) += 2;
            } else {
                (*pos)++;
            }
        }
        
        if (input[*pos] == '"') {
            (*pos)++;
            int len = *pos - start;
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            strncpy(token->value, &input[start], len);
            token->value[len] = '\0';
            token->type = TOKEN_STRING;
            return 1;
        }
    }
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int line = 1;
    int column = 1;
    
    if (input == NULL || tokens == NULL || max_tokens <= 0) {
        return 0;
    }
    
    while (input[pos] != '\0' && token_count < max_tokens) {
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
        
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        
        if (parse_number(input, &pos, &tokens[token_count]) ||
            parse_identifier(input, &pos, &tokens[token_count]) ||
            parse_operator(input, &pos, &tokens[token_count]) ||
            parse_string(input, &pos, &tokens[token_count])) {
            
            int token_len = (int)strlen(tokens[token_count].value);
            if (token_len > 0) {
                column += token_len;
            }
            token_count++;
        } else {
            tokens[token_count].type = TOKEN_UNKNOWN;
            tokens[token_count].value[0] = input[pos];
            tokens[token_count].value[1] = '\0';
            pos++;
            column++;
            token_count++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count