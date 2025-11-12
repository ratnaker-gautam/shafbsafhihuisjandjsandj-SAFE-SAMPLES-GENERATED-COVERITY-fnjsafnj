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

int parse_operator(const char *input, int pos, char *output) {
    output[0] = '\0';
    
    if (input[pos] == '\0') return 0;
    
    if (is_operator_char(input[pos])) {
        int len = 1;
        if (input[pos] == '=' && input[pos + 1] == '=') len = 2;
        else if (input[pos] == '!' && input[pos + 1] == '=') len = 2;
        else if (input[pos] == '<' && input[pos + 1] == '=') len = 2;
        else if (input[pos] == '>' && input[pos + 1] == '=') len = 2;
        else if (input[pos] == '&' && input[pos + 1] == '&') len = 2;
        else if (input[pos] == '|' && input[pos + 1] == '|') len = 2;
        
        if (len > 63) len = 63;
        strncpy(output, &input[pos], len);
        output[len] = '\0';
        return len;
    }
    
    return 0;
}

int parse_number(const char *input, int pos, char *output) {
    output[0] = '\0';
    
    if (input[pos] == '\0' || !isdigit((unsigned char)input[pos])) return 0;
    
    int len = 0;
    while (input[pos] != '\0' && isdigit((unsigned char)input[pos])) {
        if (len < 63) {
            output[len++] = input[pos];
        }
        pos++;
    }
    output[len] = '\0';
    return len;
}

int parse_identifier(const char *input, int pos, char *output) {
    output[0] = '\0';
    
    if (input[pos] == '\0' || (!isalpha((unsigned char)input[pos]) && input[pos] != '_')) return 0;
    
    int len = 0;
    while (input[pos] != '\0' && (isalnum((unsigned char)input[pos]) || input[pos] == '_')) {
        if (len < 63) {
            output[len++] = input[pos];
        }
        pos++;
    }
    output[len] = '\0';
    return len;
}

int parse_string(const char *input, int pos, char *output) {
    output[0] = '\0';
    
    if (input[pos] != '"') return 0;
    
    pos++;
    int len = 0;
    while (input[pos] != '\0' && input[pos] != '"') {
        if (len < 63) {
            output[len++] = input[pos];
        }
        pos++;
    }
    output[len] = '\0';
    
    if (input[pos] == '"') {
        return len + 2;
    }
    return 0;
}

void tokenize(const char *input, struct Token *tokens, int max_tokens, int *token_count) {
    int pos = 0;
    int line = 1;
    int column = 1;
    *token_count = 0;
    
    while (input[pos] != '\0' && *token_count < max_tokens - 1) {
        while (isspace((unsigned char)input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
        }
        
        if (input[pos] == '\0') break;
        
        struct Token *token = &tokens[*token_count];
        token->line = line;
        token->column = column;
        
        int advance = 0;
        char buffer[64];
        
        if ((advance = parse_string(input, pos, buffer)) > 0) {
            token->type = TOKEN_STRING;
            strncpy(token->value, buffer, 63);
            token->value[63] = '\0';
        } else if ((advance = parse_number(input, pos, buffer)) > 0) {
            token->type = TOKEN_NUMBER;
            strncpy(token->value, buffer, 63);
            token->value[63] = '\0';
        } else if ((advance = parse_identifier(input, pos, buffer)) > 0) {
            token->type = TOKEN_IDENTIFIER;
            strncpy(token->value, buffer, 63);
            token->value[63] = '\0';
        } else if ((advance = parse_operator(input, pos, buffer)) > 0) {
            token->type = TOKEN_OPERATOR;
            strncpy(token->value, buffer, 63);
            token->value[63] = '\0';
        } else {
            token->type = TOKEN_UNKNOWN;
            token->value[0] = input[pos];
            token->value[1] = '\0';
            advance =