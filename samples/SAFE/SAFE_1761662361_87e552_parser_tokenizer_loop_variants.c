//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

int is_delimiter(char c) {
    return c == '(' || c == ')' || c == ',' || c == ';' || c == '{' || c == '}';
}

int parse_number(const char *input, int *pos, char *buffer) {
    int start = *pos;
    int i = 0;
    while (isdigit(input[*pos])) {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        buffer[i++] = input[(*pos)++];
    }
    buffer[i] = '\0';
    return i > 0;
}

int parse_identifier(const char *input, int *pos, char *buffer) {
    int start = *pos;
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

int main(void) {
    char input[512];
    struct Token tokens[MAX_TOKENS];
    int token_count = 0;
    
    printf("Enter expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    int len = strlen(input);
    if (len > 0 && input[len-1] == '\n') input[len-1] = '\0';
    
    int pos = 0;
    while (input[pos] != '\0' && token_count < MAX_TOKENS) {
        while (isspace(input[pos])) pos++;
        if (input[pos] == '\0') break;
        
        char buffer[MAX_TOKEN_LEN];
        int found = 0;
        
        if (parse_number(input, &pos, buffer)) {
            tokens[token_count].type = NUMBER;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN-1);
            tokens[token_count].value[MAX_TOKEN_LEN-1] = '\0';
            token_count++;
            found = 1;
        } else if (parse_identifier(input, &pos, buffer)) {
            tokens[token_count].type = IDENTIFIER;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN-1);
            tokens[token_count].value[MAX_TOKEN_LEN-1] = '\0';
            token_count++;
            found = 1;
        } else if (is_operator(input[pos])) {
            tokens[token_count].type = OPERATOR;
            tokens[token_count].value[0] = input[pos];
            tokens[token_count].value[1] = '\0';
            pos++;
            token_count++;
            found = 1;
        } else if (is_delimiter(input[pos])) {
            tokens[token_count].type = DELIMITER;
            tokens[token_count].value[0] = input[pos];
            tokens[token_count].value[1] = '\0';
            pos++;
            token_count++;
            found = 1;
        }
        
        if (!found) {
            tokens[token_count].type = UNKNOWN;
            tokens[token_count].value[0] = input[pos];
            tokens[token_count].value[1] = '\0';
            pos++;
            token_count++;
        }
    }
    
    printf("Tokens found: %d\n", token_count);
    const char *type_names[] = {"NUMBER", "IDENTIFIER", "OPERATOR", "DELIMITER", "UNKNOWN"};
    
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: %s -> %s\n", i, type_names[tokens[i].type], tokens[i].value);
    }
    
    return 0;
}