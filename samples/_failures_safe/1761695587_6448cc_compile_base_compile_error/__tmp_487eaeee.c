//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50
#define DELIMITERS " \t\n\r"
#define IS_OPERATOR(c) (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '(' || c == ')')

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_END
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

Token tokens[MAX_TOKENS];
size_t token_count = 0;
size_t current_token = 0;

void add_token(TokenType type, const char* value) {
    if (token_count >= MAX_TOKENS) return;
    tokens[token_count].type = type;
    strncpy(tokens[token_count].value, value, MAX_TOKEN_LEN - 1);
    tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
    token_count++;
}

int is_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

void tokenize(const char* input) {
    const char* p = input;
    char buffer[MAX_TOKEN_LEN];
    size_t buf_index = 0;

    while (*p && token_count < MAX_TOKENS) {
        while (isspace(*p)) p++;

        if (!*p) break;

        if (IS_OPERATOR(*p)) {
            buffer[0] = *p;
            buffer[1] = '\0';
            add_token(TOKEN_OPERATOR, buffer);
            p++;
            continue;
        }

        if (isdigit(*p)) {
            buf_index = 0;
            while (isdigit(*p) && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *p++;
            }
            buffer[buf_index] = '\0';
            add_token(TOKEN_NUMBER, buffer);
            continue;
        }

        if (is_identifier_char(*p)) {
            buf_index = 0;
            while (is_identifier_char(*p) && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *p++;
            }
            buffer[buf_index] = '\0';
            add_token(TOKEN_IDENTIFIER, buffer);
            continue;
        }

        p++;
    }
    add_token(TOKEN_END, "");
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_END: return "END";
        default: return "UNKNOWN";
    }
}

void print_tokens() {
    for (size_t i = 0; i < token_count; i++) {
        printf("Token %zu: Type=%s, Value='%s'\n", 
               i, token_type_name(tokens[i].type), tokens[i].value);
    }
}

int parse_expression();

int parse_factor() {
    if (current_token >= token_count) return 0;
    
    Token token = tokens[current_token];
    if (token.type == TOKEN_NUMBER) {
        printf("Factor: Number %s\n", token.value);
        current_token++;
        return 1;
    } else if (token.type == TOKEN_IDENTIFIER) {
        printf("Factor: Identifier %s\n", token.value);
        current_token++;
        return 1;
    } else if (strcmp(token.value, "(") == 0) {
        current_token++;
        if (!parse_expression()) return 0;
        if (current_token >= token_count || strcmp(tokens[current_token].value, ")") != 0) {
            return 0;
        }
        current_token++;
        return 1;
    }
    return 0;
}

int parse_term() {
    if (!parse_factor()) return 0;
    
    while (current_token < token_count) {
        Token token = tokens[current_token];
        if (token.type == TOKEN_OPERATOR && 
            (strcmp(token.value, "*") == 0 || strcmp(token.value, "/") == 0)) {
            current_token++;
            if (!parse_factor()) return 0;
            printf("Term: Operator %s\n", token.value);
        } else {
            break;
        }
    }
    return 1;
}

int parse_expression() {
    if (!parse_term()) return 0;
    
    while (current_token < token_count) {
        Token token = tokens[current_token];
        if (token.type == TOKEN_OPERATOR && 
            (strcmp(token.value, "+") == 0 || strcmp(token.value, "-") == 0)) {
            current_token++;
            if (!parse_term()) return 0;
            printf("Expression: Operator %s\n", token.value);
        } else {
            break;
        }
    }
    return 1;
}

int parse_assignment() {
    if (current_token >= token_count - 2) return 0;
    
    if (tokens[current_token].type != TOKEN_IDENTIFIER) return 0;
    printf("Assignment: Identifier %s\n", tokens[current_token].value);
    current_token++;
    
    if (current_token >= token_count || strcmp(tokens[current_token].value, "=")