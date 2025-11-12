//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *input, int *pos, char *buffer) {
    int len = 0;
    int start_pos = *pos;
    
    while (isdigit(input[*pos])) {
        if (len >= MAX_TOKEN_LEN - 1) {
            *pos = start_pos;
            return 0;
        }
        buffer[len++] = input[(*pos)++];
    }
    
    buffer[len] = '\0';
    return len > 0;
}

int parse_identifier(const char *input, int *pos, char *buffer) {
    int len = 0;
    int start_pos = *pos;
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        buffer[len++] = input[(*pos)++];
        
        while (isalnum(input[*pos]) || input[*pos] == '_') {
            if (len >= MAX_TOKEN_LEN - 1) break;
            buffer[len++] = input[(*pos)++];
        }
        
        buffer[len] = '\0';
        return 1;
    }
    
    *pos = start_pos;
    return 0;
}

int parse_operator(const char *input, int *pos, char *buffer) {
    int len = 0;
    int start_pos = *pos;
    
    if (is_operator_char(input[*pos])) {
        buffer[len++] = input[(*pos)++];
        
        if (is_operator_char(input[*pos])) {
            if (len >= MAX_TOKEN_LEN - 1) {
                buffer[len] = '\0';
                return 1;
            }
            buffer[len++] = input[(*pos)++];
        }
        
        buffer[len] = '\0';
        return 1;
    }
    
    *pos = start_pos;
    return 0;
}

int parse_string(const char *input, int *pos, char *buffer) {
    int len = 0;
    int start_pos = *pos;
    
    if (input[*pos] == '"') {
        buffer[len++] = input[(*pos)++];
        
        while (input[*pos] != '"' && input[*pos] != '\0') {
            if (len >= MAX_TOKEN_LEN - 1) break;
            buffer[len++] = input[(*pos)++];
        }
        
        if (input[*pos] == '"') {
            buffer[len++] = input[(*pos)++];
            buffer[len] = '\0';
            return 1;
        }
    }
    
    *pos = start_pos;
    return 0;
}

void tokenize(const char *input, struct Token *tokens, int *token_count) {
    int pos = 0;
    *token_count = 0;
    
    while (input[pos] != '\0' && *token_count < MAX_TOKENS) {
        while (isspace(input[pos])) pos++;
        
        if (input[pos] == '\0') break;
        
        struct Token *token = &tokens[*token_count];
        
        if (parse_number(input, &pos, token->value)) {
            token->type = TOKEN_NUMBER;
            (*token_count)++;
        } else if (parse_identifier(input, &pos, token->value)) {
            token->type = TOKEN_IDENTIFIER;
            (*token_count)++;
        } else if (parse_operator(input, &pos, token->value)) {
            token->type = TOKEN_OPERATOR;
            (*token_count)++;
        } else if (parse_string(input, &pos, token->value)) {
            token->type = TOKEN_STRING;
            (*token_count)++;
        } else {
            token->type = TOKEN_INVALID;
            token->value[0] = input[pos++];
            token->value[1] = '\0';
            (*token_count)++;
        }
    }
    
    if (*token_count < MAX_TOKENS) {
        tokens[*token_count].type = TOKEN_EOF;
        strncpy(tokens[*token_count].value, "EOF", MAX_TOKEN_LEN - 1);
        tokens[*token_count].value[MAX_TOKEN_LEN - 1] = '\0';
        (*token_count)++;
    }
}

const char* token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_INVALID: return "INVALID";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    int token_count;
    
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof