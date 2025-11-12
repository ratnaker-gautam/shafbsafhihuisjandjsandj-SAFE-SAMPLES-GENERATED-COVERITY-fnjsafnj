//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

enum TokenType { NUMBER, IDENTIFIER, OPERATOR, DELIMITER, UNKNOWN };

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
};

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_delimiter(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',' || c == ';';
}

int parse_number(const char *input, int *pos, char *buffer) {
    int len = 0;
    int input_len = strlen(input);
    
    while (*pos < input_len && isdigit(input[*pos])) {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        buffer[len++] = input[(*pos)++];
    }
    
    buffer[len] = '\0';
    return len > 0;
}

int parse_identifier(const char *input, int *pos, char *buffer) {
    int len = 0;
    int input_len = strlen(input);
    
    if (*pos >= input_len || (!isalpha(input[*pos]) && input[*pos] != '_')) return 0;
    
    while (*pos < input_len && (isalnum(input[*pos]) || input[*pos] == '_')) {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        buffer[len++] = input[(*pos)++];
    }
    
    buffer[len] = '\0';
    return len > 0;
}

int parse_operator(const char *input, int *pos, char *buffer) {
    int input_len = strlen(input);
    if (*pos >= input_len || !is_operator(input[*pos])) return 0;
    
    buffer[0] = input[(*pos)++];
    buffer[1] = '\0';
    return 1;
}

int parse_delimiter(const char *input, int *pos, char *buffer) {
    int input_len = strlen(input);
    if (*pos >= input_len || !is_delimiter(input[*pos])) return 0;
    
    buffer[0] = input[(*pos)++];
    buffer[1] = '\0';
    return 1;
}

void tokenize(const char *input, struct Token *tokens, int *token_count) {
    int pos = 0;
    int input_len = strlen(input);
    *token_count = 0;
    
    while (pos < input_len && *token_count < MAX_TOKENS) {
        while (pos < input_len && isspace(input[pos])) pos++;
        if (pos >= input_len) break;
        
        char buffer[MAX_TOKEN_LEN];
        struct Token *current = &tokens[*token_count];
        
        if (parse_number(input, &pos, buffer)) {
            current->type = NUMBER;
            strncpy(current->value, buffer, MAX_TOKEN_LEN - 1);
            current->value[MAX_TOKEN_LEN - 1] = '\0';
            (*token_count)++;
        } else if (parse_identifier(input, &pos, buffer)) {
            current->type = IDENTIFIER;
            strncpy(current->value, buffer, MAX_TOKEN_LEN - 1);
            current->value[MAX_TOKEN_LEN - 1] = '\0';
            (*token_count)++;
        } else if (parse_operator(input, &pos, buffer)) {
            current->type = OPERATOR;
            strncpy(current->value, buffer, MAX_TOKEN_LEN - 1);
            current->value[MAX_TOKEN_LEN - 1] = '\0';
            (*token_count)++;
        } else if (parse_delimiter(input, &pos, buffer)) {
            current->type = DELIMITER;
            strncpy(current->value, buffer, MAX_TOKEN_LEN - 1);
            current->value[MAX_TOKEN_LEN - 1] = '\0';
            (*token_count)++;
        } else {
            current->type = UNKNOWN;
            current->value[0] = input[pos++];
            current->value[1] = '\0';
            (*token_count)++;
        }
    }
}

const char *token_type_name(enum TokenType type) {
    switch (type) {
        case NUMBER: return "NUMBER";
        case IDENTIFIER: return "IDENTIFIER";
        case OPERATOR: return "OPERATOR";
        case DELIMITER: return "DELIMITER";
        case UNKNOWN: return "UNKNOWN";
        default: return "INVALID";
    }
}

int main(void) {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    int token_count;
    
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
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    tokenize(input, tokens, &token_count);