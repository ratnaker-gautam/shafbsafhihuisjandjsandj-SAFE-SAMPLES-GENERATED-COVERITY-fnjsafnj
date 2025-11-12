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
    int len = 0;
    char c = input[pos];
    
    if (is_operator_char(c)) {
        output[len++] = c;
        if (is_operator_char(input[pos + 1])) {
            output[len++] = input[pos + 1];
        }
    }
    output[len] = '\0';
    return len;
}

int parse_number(const char *input, int pos, char *output) {
    int len = 0;
    int has_dot = 0;
    
    while (input[pos] != '\0') {
        if (isdigit(input[pos])) {
            output[len++] = input[pos];
        } else if (input[pos] == '.' && !has_dot) {
            output[len++] = input[pos];
            has_dot = 1;
        } else {
            break;
        }
        pos++;
    }
    output[len] = '\0';
    return len;
}

int parse_identifier(const char *input, int pos, char *output) {
    int len = 0;
    
    if (isalpha(input[pos]) || input[pos] == '_') {
        output[len++] = input[pos];
        pos++;
        
        while (input[pos] != '\0' && (isalnum(input[pos]) || input[pos] == '_')) {
            output[len++] = input[pos];
            pos++;
        }
    }
    output[len] = '\0';
    return len;
}

int parse_string(const char *input, int pos, char *output) {
    int len = 0;
    
    if (input[pos] == '"') {
        output[len++] = input[pos];
        pos++;
        
        while (input[pos] != '\0' && input[pos] != '"') {
            if (input[pos] == '\\' && input[pos + 1] != '\0') {
                output[len++] = input[pos++];
                output[len++] = input[pos++];
            } else {
                output[len++] = input[pos++];
            }
        }
        
        if (input[pos] == '"') {
            output[len++] = input[pos];
        }
    }
    output[len] = '\0';
    return len;
}

void skip_whitespace(const char *input, int *pos) {
    while (input[*pos] != '\0' && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            (*pos)++;
            break;
        }
        (*pos)++;
    }
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int line = 1;
    int column = 1;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        skip_whitespace(input, &pos);
        if (input[pos] == '\0') break;
        
        int current_column = column;
        int token_len = 0;
        struct Token *token = &tokens[token_count];
        
        if (isdigit(input[pos])) {
            token_len = parse_number(input, pos, token->value);
            token->type = TOKEN_NUMBER;
        } else if (isalpha(input[pos]) || input[pos] == '_') {
            token_len = parse_identifier(input, pos, token->value);
            token->type = TOKEN_IDENTIFIER;
        } else if (input[pos] == '"') {
            token_len = parse_string(input, pos, token->value);
            token->type = TOKEN_STRING;
        } else if (is_operator_char(input[pos])) {
            token_len = parse_operator(input, pos, token->value);
            token->type = TOKEN_OPERATOR;
        } else {
            token->value[0] = input[pos];
            token->value[1] = '\0';
            token->type = TOKEN_UNKNOWN;
            token_len = 1;
        }
        
        if (token_len > 0) {
            token->line = line;
            token->column = current_column;
            token_count++;
            pos += token_len;
            column += token_len;
            
            for (int i = 0; i < token_len; i++) {
                if (token->value[i] == '\n') {
                    line++;
                    column = 1;
                }
            }
        } else {
            pos++;
            column++;
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

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "UNKNOWN"
    };
    printf("Line %d, Column