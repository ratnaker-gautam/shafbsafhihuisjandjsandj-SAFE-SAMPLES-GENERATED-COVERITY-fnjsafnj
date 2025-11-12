//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

enum TokenType { NUMBER, IDENTIFIER, OPERATOR, PUNCTUATION, END };

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_punctuation_char(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ',';
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
        if (is_operator_char(input[*pos]) && i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = input[(*pos)++];
        }
        buffer[i] = '\0';
        return 1;
    }
    return 0;
}

int parse_punctuation(const char *input, int *pos, char *buffer) {
    if (is_punctuation_char(input[*pos])) {
        buffer[0] = input[(*pos)++];
        buffer[1] = '\0';
        return 1;
    }
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    char buffer[MAX_TOKEN_LEN];
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        while (isspace(input[pos])) pos++;
        if (input[pos] == '\0') break;
        
        if (parse_number(input, &pos, buffer)) {
            tokens[token_count].type = NUMBER;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        } else if (parse_identifier(input, &pos, buffer)) {
            tokens[token_count].type = IDENTIFIER;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        } else if (parse_operator(input, &pos, buffer)) {
            tokens[token_count].type = OPERATOR;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        } else if (parse_punctuation(input, &pos, buffer)) {
            tokens[token_count].type = PUNCTUATION;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        } else {
            break;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = END;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

const char* token_type_name(enum TokenType type) {
    switch (type) {
        case NUMBER: return "NUMBER";
        case IDENTIFIER: return "IDENTIFIER";
        case OPERATOR: return "OPERATOR";
        case PUNCTUATION: return "PUNCTUATION";
        case END: return "END";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[512];
    struct Token tokens[MAX_TOKENS];
    int token_count;
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    token_count