//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LINE 1024
#define MAX_TOKENS 256
#define MAX_TOKEN_LEN 128

enum token_type {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_PUNCTUATION,
    TOKEN_UNKNOWN
};

struct token {
    enum token_type type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_punctuation_char(char c) {
    return strchr("(),;{}[]", c) != NULL;
}

int parse_number(const char *input, int *pos, struct token *token, int line, int column) {
    int start = *pos;
    int has_dot = 0;
    
    while (isdigit(input[*pos]) || input[*pos] == '.') {
        if (input[*pos] == '.') {
            if (has_dot) break;
            has_dot = 1;
        }
        (*pos)++;
    }
    
    int len = *pos - start;
    if (len > 0 && len < MAX_TOKEN_LEN) {
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        token->line = line;
        token->column = column;
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, struct token *token, int line, int column) {
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
        int len = *pos - start;
        if (len < MAX_TOKEN_LEN) {
            strncpy(token->value, &input[start], len);
            token->value[len] = '\0';
            token->type = TOKEN_STRING;
            token->line = line;
            token->column = column;
            return 1;
        }
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, struct token *token, int line, int column) {
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    
    int start = *pos;
    while (isalnum(input[*pos]) || input[*pos] == '_') {
        (*pos)++;
    }
    
    int len = *pos - start;
    if (len > 0 && len < MAX_TOKEN_LEN) {
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        token->line = line;
        token->column = column;
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, struct token *token, int line, int column) {
    if (!is_operator_char(input[*pos])) return 0;
    
    int start = *pos;
    while (is_operator_char(input[*pos])) {
        (*pos)++;
    }
    
    int len = *pos - start;
    if (len > 0 && len < MAX_TOKEN_LEN) {
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_OPERATOR;
        token->line = line;
        token->column = column;
        return 1;
    }
    return 0;
}

int parse_punctuation(const char *input, int *pos, struct token *token, int line, int column) {
    if (!is_punctuation_char(input[*pos])) return 0;
    
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    token->type = TOKEN_PUNCTUATION;
    token->line = line;
    token->column = column;
    (*pos)++;
    return 1;
}

int tokenize_line(const char *line, int line_num, struct token *tokens, int *token_count) {
    int pos = 0;
    int column = 1;
    
    while (line[pos] != '\0' && *token_count < MAX_TOKENS - 1) {
        while (isspace(line[pos])) {
            pos++;
            column++;
        }
        
        if (line[pos] == '\0') break;
        
        struct token token;
        int parsed = 0;
        
        if (parse_number(line, &pos, &token, line_num, column)) parsed = 1;
        else if (parse_string(line, &pos, &token, line_num, column)) parsed = 1;
        else if (parse_identifier(line, &pos, &token, line_num, column)) parsed = 1;
        else if (parse_operator(line, &pos, &token, line_num, column