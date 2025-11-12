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
    char value[32];
    int line;
    int column;
};

int is_operator_char(char c) {
    const char operators[] = "+-*/=<>!&|";
    for (size_t i = 0; i < strlen(operators); i++) {
        if (c == operators[i]) return 1;
    }
    return 0;
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
        token->value[length++] = input[(*pos)++];
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
    
    do {
        token->value[length++] = input[(*pos)++];
    } while ((isalnum(input[*pos]) || input[*pos] == '_') && length < 31);
    
    token->value[length] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    if (!is_operator_char(input[*pos])) return 0;
    
    int start = *pos;
    int length = 0;
    
    while (is_operator_char(input[*pos]) && length < 31) {
        token->value[length++] = input[(*pos)++];
    }
    
    token->value[length] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

void skip_whitespace(const char *input, int *pos) {
    while (input[*pos] != '\0' && isspace(input[*pos])) {
        (*pos)++;
    }
}

int get_next_token(const char *input, int *pos, int line, struct Token *token) {
    skip_whitespace(input, pos);
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    token->line = line;
    token->column = *pos;
    
    if (parse_number(input, pos, token)) return 1;
    if (parse_string(input, pos, token)) return 1;
    if (parse_identifier(input, pos, token)) return 1;
    if (parse_operator(input, pos, token)) return 1;
    
    token->type = TOKEN_UNKNOWN;
    token->value[0] = input[(*pos)++];
    token->value[1] = '\0';
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
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    printf("\nTokenizing: %s\n\n", input);
    printf("%-12s %-15s %-6s %-6