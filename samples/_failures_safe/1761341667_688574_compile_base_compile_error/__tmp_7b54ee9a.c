//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
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
    char value[64];
    int line;
    int column;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int get_operator_length(const char *str) {
    if (str[0] == '\0') return 0;
    if (str[0] == '&' && str[1] == '&') return 2;
    if (str[0] == '|' && str[1] == '|') return 2;
    if (str[0] == '=' && str[1] == '=') return 2;
    if (str[0] == '!' && str[1] == '=') return 2;
    if (str[0] == '<' && str[1] == '=') return 2;
    if (str[0] == '>' && str[1] == '=') return 2;
    return 1;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int token_count = 0;
    int line = 1;
    int column = 1;
    const char *ptr = input;
    
    while (*ptr != '\0' && token_count < max_tokens - 1) {
        while (isspace((unsigned char)*ptr)) {
            if (*ptr == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            ptr++;
        }
        
        if (*ptr == '\0') break;
        
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        
        if (isdigit((unsigned char)*ptr)) {
            tokens[token_count].type = TOKEN_NUMBER;
            int i = 0;
            while (isdigit((unsigned char)*ptr) && i < 63) {
                tokens[token_count].value[i++] = *ptr++;
                column++;
            }
            tokens[token_count].value[i] = '\0';
            token_count++;
        } else if (isalpha((unsigned char)*ptr) || *ptr == '_') {
            tokens[token_count].type = TOKEN_IDENTIFIER;
            int i = 0;
            while ((isalnum((unsigned char)*ptr) || *ptr == '_') && i < 63) {
                tokens[token_count].value[i++] = *ptr++;
                column++;
            }
            tokens[token_count].value[i] = '\0';
            token_count++;
        } else if (*ptr == '"') {
            tokens[token_count].type = TOKEN_STRING;
            int i = 0;
            ptr++;
            column++;
            while (*ptr != '"' && *ptr != '\0' && i < 63) {
                tokens[token_count].value[i++] = *ptr++;
                column++;
            }
            tokens[token_count].value[i] = '\0';
            if (*ptr == '"') {
                ptr++;
                column++;
            }
            token_count++;
        } else if (is_operator_char(*ptr)) {
            tokens[token_count].type = TOKEN_OPERATOR;
            int op_len = get_operator_length(ptr);
            if (op_len > 0 && op_len < 64) {
                strncpy(tokens[token_count].value, ptr, op_len);
                tokens[token_count].value[op_len] = '\0';
                ptr += op_len;
                column += op_len;
                token_count++;
            } else {
                tokens[token_count].type = TOKEN_UNKNOWN;
                tokens[token_count].value[0] = *ptr;
                tokens[token_count].value[1] = '\0';
                ptr++;
                column++;
                token_count++;
            }
        } else {
            tokens[token_count].type = TOKEN_UNKNOWN;
            tokens[token_count].value[0] = *ptr;
            tokens[token_count].value[1] = '\0';
            ptr++;
            column++;
            token_count++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        token_count++;
    }
    
    return token_count;
}

const char *token_type_name(enum TokenType type) {
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