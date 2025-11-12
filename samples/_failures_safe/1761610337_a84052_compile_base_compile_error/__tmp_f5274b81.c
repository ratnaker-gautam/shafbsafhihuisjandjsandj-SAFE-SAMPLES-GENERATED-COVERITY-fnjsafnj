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
    TOKEN_EOF
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
    int start = pos;
    output[0] = '\0';
    
    while (is_operator_char(input[pos]) && (pos - start) < (MAX_TOKEN_LEN - 1)) {
        output[pos - start] = input[pos];
        pos++;
    }
    output[pos - start] = '\0';
    
    return pos - start;
}

int parse_number(const char *input, int pos, char *output) {
    int start = pos;
    output[0] = '\0';
    
    while (isdigit(input[pos]) && (pos - start) < (MAX_TOKEN_LEN - 1)) {
        output[pos - start] = input[pos];
        pos++;
    }
    output[pos - start] = '\0';
    
    return pos - start;
}

int parse_identifier(const char *input, int pos, char *output) {
    int start = pos;
    output[0] = '\0';
    
    if (!isalpha(input[pos]) && input[pos] != '_') {
        return 0;
    }
    
    while ((isalnum(input[pos]) || input[pos] == '_') && (pos - start) < (MAX_TOKEN_LEN - 1)) {
        output[pos - start] = input[pos];
        pos++;
    }
    output[pos - start] = '\0';
    
    return pos - start;
}

int parse_string(const char *input, int pos, char *output) {
    int start = pos;
    output[0] = '\0';
    
    if (input[pos] != '"') {
        return 0;
    }
    
    pos++;
    while (input[pos] != '\0' && input[pos] != '"' && (pos - start - 1) < (MAX_TOKEN_LEN - 1)) {
        output[pos - start - 1] = input[pos];
        pos++;
    }
    
    if (input[pos] == '"') {
        output[pos - start - 1] = '\0';
        return pos - start + 1;
    }
    
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int line = 1;
    int column = 1;
    int token_count = 0;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
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
        
        int advance = 0;
        struct Token *token = &tokens[token_count];
        token->line = line;
        token->column = column;
        
        if (isdigit(input[pos])) {
            advance = parse_number(input, pos, token->value);
            if (advance > 0) {
                token->type = TOKEN_NUMBER;
            }
        } else if (isalpha(input[pos]) || input[pos] == '_') {
            advance = parse_identifier(input, pos, token->value);
            if (advance > 0) {
                token->type = TOKEN_IDENTIFIER;
            }
        } else if (is_operator_char(input[pos])) {
            advance = parse_operator(input, pos, token->value);
            if (advance > 0) {
                token->type = TOKEN_OPERATOR;
            }
        } else if (input[pos] == '"') {
            advance = parse_string(input, pos, token->value);
            if (advance > 0) {
                token->type = TOKEN_STRING;
            }
        }
        
        if (advance > 0) {
            column += advance;
            pos += advance;
            token_count++;
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

void print_token_type(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
        default: printf("UNKNOWN"); break;
    }
}

int main(void) {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr,