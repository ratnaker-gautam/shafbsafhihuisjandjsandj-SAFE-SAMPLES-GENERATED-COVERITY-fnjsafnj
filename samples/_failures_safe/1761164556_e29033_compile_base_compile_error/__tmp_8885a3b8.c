//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_STRING, TOKEN_EOF, TOKEN_UNKNOWN };

struct Token {
    enum TokenType type;
    char value[64];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_operator(const char *input, int pos, char *buffer) {
    int len = 0;
    char c = input[pos];
    if (is_operator_char(c)) {
        buffer[len++] = c;
        if (is_operator_char(input[pos + 1]) && ((c == '&' && input[pos + 1] == '&') || (c == '|' && input[pos + 1] == '|') || (c == '=' && input[pos + 1] == '=') || (c == '!' && input[pos + 1] == '=') || (c == '<' && input[pos + 1] == '=') || (c == '>' && input[pos + 1] == '='))) {
            buffer[len++] = input[pos + 1];
        }
        buffer[len] = '\0';
    }
    return len;
}

int parse_number(const char *input, int pos, char *buffer) {
    int len = 0;
    int has_dot = 0;
    while (input[pos] != '\0' && (isdigit(input[pos]) || (input[pos] == '.' && !has_dot))) {
        if (input[pos] == '.') has_dot = 1;
        if (len < 63) buffer[len++] = input[pos++];
        else break;
    }
    buffer[len] = '\0';
    return len;
}

int parse_string(const char *input, int pos, char *buffer) {
    int len = 0;
    if (input[pos] == '"') {
        pos++;
        while (input[pos] != '\0' && input[pos] != '"') {
            if (input[pos] == '\\' && input[pos + 1] != '\0') {
                if (len < 62) buffer[len++] = input[pos++];
                if (len < 62) buffer[len++] = input[pos++];
            } else {
                if (len < 63) buffer[len++] = input[pos++];
                else break;
            }
        }
        if (input[pos] == '"') pos++;
        buffer[len] = '\0';
    }
    return len > 0 ? len + 2 : 0;
}

int parse_identifier(const char *input, int pos, char *buffer) {
    int len = 0;
    if (isalpha(input[pos]) || input[pos] == '_') {
        while (input[pos] != '\0' && (isalnum(input[pos]) || input[pos] == '_')) {
            if (len < 63) buffer[len++] = input[pos++];
            else break;
        }
        buffer[len] = '\0';
    }
    return len;
}

void tokenize(const char *input, struct Token *tokens, int max_tokens, int *token_count) {
    int pos = 0;
    int line = 1;
    int column = 1;
    *token_count = 0;
    
    while (input[pos] != '\0' && *token_count < max_tokens - 1) {
        if (isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
            continue;
        }
        
        struct Token *token = &tokens[*token_count];
        token->line = line;
        token->column = column;
        int advance = 0;
        
        if (is_operator_char(input[pos])) {
            advance = parse_operator(input, pos, token->value);
            if (advance > 0) {
                token->type = TOKEN_OPERATOR;
            }
        }
        
        if (advance == 0 && isdigit(input[pos])) {
            advance = parse_number(input, pos, token->value);
            if (advance > 0) {
                token->type = TOKEN_NUMBER;
            }
        }
        
        if (advance == 0 && input[pos] == '"') {
            advance = parse_string(input, pos, token->value);
            if (advance > 0) {
                token->type = TOKEN_STRING;
            }
        }
        
        if (advance == 0 && (isalpha(input[pos]) || input[pos] == '_')) {
            advance = parse_identifier(input, pos, token->value);
            if (advance > 0) {
                token->type = TOKEN_IDENTIFIER;
            }
        }
        
        if (advance == 0) {
            token->type = TOKEN_UNKNOWN;
            if (pos < 63) {
                token->value[0] = input[pos];
                token->value[1] = '\0';
            } else {
                token->value[0] = '\0';
            }
            advance = 1;
        }
        
        column += advance;
        pos += advance;
        (*token_count)++;