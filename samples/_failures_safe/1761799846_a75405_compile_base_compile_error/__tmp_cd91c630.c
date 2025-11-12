//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
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
    int line;
    int column;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_operator(const char *input, int pos, char *output) {
    char c = input[pos];
    
    if (c == '&' && input[pos + 1] == '&') {
        strcpy(output, "&&");
        return 2;
    } else if (c == '|' && input[pos + 1] == '|') {
        strcpy(output, "||");
        return 2;
    } else if (c == '=' && input[pos + 1] == '=') {
        strcpy(output, "==");
        return 2;
    } else if (c == '!' && input[pos + 1] == '=') {
        strcpy(output, "!=");
        return 2;
    } else if (c == '<' && input[pos + 1] == '=') {
        strcpy(output, "<=");
        return 2;
    } else if (c == '>' && input[pos + 1] == '=') {
        strcpy(output, ">=");
        return 2;
    } else if (is_operator_char(c)) {
        output[0] = c;
        output[1] = '\0';
        return 1;
    }
    return 0;
}

int parse_number(const char *input, int pos, char *output) {
    int len = 0;
    int has_dot = 0;
    
    while (input[pos + len] && (isdigit(input[pos + len]) || input[pos + len] == '.')) {
        if (input[pos + len] == '.') {
            if (has_dot) break;
            has_dot = 1;
        }
        if (len < MAX_TOKEN_LEN - 1) {
            output[len] = input[pos + len];
            len++;
        } else {
            break;
        }
    }
    output[len] = '\0';
    return len > 0 ? len : 0;
}

int parse_identifier(const char *input, int pos, char *output) {
    int len = 0;
    
    if (isalpha(input[pos]) || input[pos] == '_') {
        while (input[pos + len] && (isalnum(input[pos + len]) || input[pos + len] == '_')) {
            if (len < MAX_TOKEN_LEN - 1) {
                output[len] = input[pos + len];
                len++;
            } else {
                break;
            }
        }
        output[len] = '\0';
        return len;
    }
    return 0;
}

int parse_string(const char *input, int pos, char *output) {
    int len = 0;
    
    if (input[pos] == '"') {
        output[len++] = '"';
        pos++;
        
        while (input[pos + len - 1] && input[pos + len - 1] != '"') {
            if (input[pos + len - 1] == '\\') {
                if (len < MAX_TOKEN_LEN - 2) {
                    output[len++] = input[pos + len - 1];
                    if (input[pos + len]) {
                        output[len++] = input[pos + len];
                    }
                } else {
                    break;
                }
            } else {
                if (len < MAX_TOKEN_LEN - 1) {
                    output[len++] = input[pos + len - 1];
                } else {
                    break;
                }
            }
        }
        
        if (input[pos + len - 1] == '"') {
            output[len++] = '"';
            output[len] = '\0';
            return len;
        }
    }
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int line = 1;
    int column = 1;
    int token_count = 0;
    
    while (input[pos] && token_count < max_tokens) {
        while (isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
        }
        
        if (!input[pos]) break;
        
        struct Token *token = &tokens[token_count];
        token->line = line;
        token->column = column;
        
        int advance = 0;
        
        if ((advance = parse_number(input, pos, token->value)) > 0) {
            token->type = TOKEN_NUMBER;
        } else if ((advance = parse_identifier(input, pos, token->value)) > 0) {
            token->type = TOKEN_IDENTIFIER;
        } else if ((advance = parse_operator(input, pos, token->value)) > 0) {
            token->type = TOKEN_OPERATOR;
        } else if ((advance = parse_string(input, pos, token->value)) > 0) {
            token->type = TO