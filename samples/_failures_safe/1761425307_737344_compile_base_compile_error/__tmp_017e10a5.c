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
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *input, int *pos, char *buffer) {
    int i = 0;
    while (isdigit(input[*pos])) {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        buffer[i++] = input[(*pos)++];
    }
    buffer[i] = '\0';
    return i > 0;
}

int parse_identifier(const char *input, int *pos, char *buffer) {
    int i = 0;
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        buffer[i++] = input[(*pos)++];
        while (isalnum(input[*pos]) || input[*pos] == '_') {
            if (i >= MAX_TOKEN_LEN - 1) break;
            buffer[i++] = input[(*pos)++];
        }
        buffer[i] = '\0';
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, char *buffer) {
    int i = 0;
    if (is_operator_char(input[*pos])) {
        buffer[i++] = input[(*pos)++];
        if (is_operator_char(input[*pos])) {
            if (i >= MAX_TOKEN_LEN - 1) {
                buffer[i] = '\0';
                return 1;
            }
            buffer[i++] = input[(*pos)++];
        }
        buffer[i] = '\0';
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, char *buffer) {
    int i = 0;
    if (input[*pos] == '"') {
        buffer[i++] = input[(*pos)++];
        while (input[*pos] != '"' && input[*pos] != '\0') {
            if (i >= MAX_TOKEN_LEN - 1) break;
            buffer[i++] = input[(*pos)++];
        }
        if (input[*pos] == '"') {
            buffer[i++] = input[(*pos)++];
            buffer[i] = '\0';
            return 1;
        }
    }
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int line = 1;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        while (isspace(input[pos])) {
            if (input[pos] == '\n') line++;
            pos++;
        }
        if (input[pos] == '\0') break;
        
        struct Token *token = &tokens[token_count];
        token->line = line;
        
        int found = 0;
        char buffer[MAX_TOKEN_LEN];
        
        if (parse_number(input, &pos, buffer)) {
            token->type = TOKEN_NUMBER;
            strncpy(token->value, buffer, MAX_TOKEN_LEN - 1);
            token->value[MAX_TOKEN_LEN - 1] = '\0';
            found = 1;
        } else if (parse_identifier(input, &pos, buffer)) {
            token->type = TOKEN_IDENTIFIER;
            strncpy(token->value, buffer, MAX_TOKEN_LEN - 1);
            token->value[MAX_TOKEN_LEN - 1] = '\0';
            found = 1;
        } else if (parse_operator(input, &pos, buffer)) {
            token->type = TOKEN_OPERATOR;
            strncpy(token->value, buffer, MAX_TOKEN_LEN - 1);
            token->value[MAX_TOKEN_LEN - 1] = '\0';
            found = 1;
        } else if (parse_string(input, &pos, buffer)) {
            token->type = TOKEN_STRING;
            strncpy(token->value, buffer, MAX_TOKEN_LEN - 1);
            token->value[MAX_TOKEN_LEN - 1] = '\0';
            found = 1;
        }
        
        if (found) {
            token_count++;
        } else {
            pos++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        tokens[token_count].line = line;
        token_count++;
    }
    
    return token_count;
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

int main() {