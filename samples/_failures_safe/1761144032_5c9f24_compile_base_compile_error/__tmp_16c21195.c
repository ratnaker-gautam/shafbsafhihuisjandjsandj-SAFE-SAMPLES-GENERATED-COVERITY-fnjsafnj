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
    TOKEN_ERROR
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
    
    int len = *pos - start;
    if (len > 31) len = 31;
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_string(const char *input, int *pos, struct Token *token) {
    if (input[*pos] != '"') return 0;
    
    (*pos)++;
    int start = *pos;
    int len = 0;
    
    while (input[*pos] != '\0' && input[*pos] != '"' && len < 31) {
        token->value[len++] = input[*pos];
        (*pos)++;
    }
    
    if (input[*pos] == '"') {
        token->value[len] = '\0';
        token->type = TOKEN_STRING;
        (*pos)++;
        return 1;
    }
    
    return 0;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    
    int start = *pos;
    int len = 0;
    
    while (input[*pos] != '\0' && (isalnum(input[*pos]) || input[*pos] == '_') && len < 31) {
        token->value[len++] = input[*pos];
        (*pos)++;
    }
    
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    if (!is_operator_char(input[*pos])) return 0;
    
    int start = *pos;
    int len = 0;
    
    while (input[*pos] != '\0' && is_operator_char(input[*pos]) && len < 31) {
        token->value[len++] = input[*pos];
        (*pos)++;
    }
    
    token->value[len] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

struct Token get_next_token(const char *input, int *pos, int line, int column) {
    struct Token token = {TOKEN_ERROR, "", line, column};
    
    if (input[*pos] == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        return token;
    }
    
    while (isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
        (*pos)++;
    }
    
    token.line = line;
    token.column = column;
    
    if (parse_number(input, pos, &token)) return token;
    if (parse_string(input, pos, &token)) return token;
    if (parse_identifier(input, pos, &token)) return token;
    if (parse_operator(input, pos, &token)) return token;
    
    if (input[*pos] != '\0') {
        token.value[0] = input[*pos];
        token.value[1] = '\0';
        (*pos)++;
    }
    
    return token;
}

const char* token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "ERROR";
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int pos = 0;
    int line = 1;
    int