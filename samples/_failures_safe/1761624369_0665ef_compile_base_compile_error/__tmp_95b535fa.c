//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int read_next_char(FILE *file, int *line, int *column) {
    int c = fgetc(file);
    if (c == '\n') {
        (*line)++;
        *column = 1;
    } else {
        (*column)++;
    }
    return c;
}

void unread_char(int c, FILE *file, int *column) {
    if (c != EOF) {
        ungetc(c, file);
        (*column)--;
    }
}

int tokenize_file(FILE *file, struct Token *tokens, int max_tokens) {
    int line = 1, column = 1;
    int token_count = 0;
    int c;
    
    while (token_count < max_tokens - 1 && (c = read_next_char(file, &line, &column)) != EOF) {
        if (isspace(c)) {
            continue;
        }
        
        struct Token *token = &tokens[token_count];
        token->line = line;
        token->column = column - 1;
        
        if (isdigit(c)) {
            token->type = TOKEN_NUMBER;
            int pos = 0;
            token->value[pos++] = c;
            
            while (pos < MAX_TOKEN_LEN - 1 && (c = read_next_char(file, &line, &column)) != EOF) {
                if (!isdigit(c)) {
                    unread_char(c, file, &column);
                    break;
                }
                token->value[pos++] = c;
            }
            token->value[pos] = '\0';
            token_count++;
        }
        else if (isalpha(c) || c == '_') {
            token->type = TOKEN_IDENTIFIER;
            int pos = 0;
            token->value[pos++] = c;
            
            while (pos < MAX_TOKEN_LEN - 1 && (c = read_next_char(file, &line, &column)) != EOF) {
                if (!isalnum(c) && c != '_') {
                    unread_char(c, file, &column);
                    break;
                }
                token->value[pos++] = c;
            }
            token->value[pos] = '\0';
            token_count++;
        }
        else if (c == '"') {
            token->type = TOKEN_STRING;
            int pos = 0;
            
            while (pos < MAX_TOKEN_LEN - 1 && (c = read_next_char(file, &line, &column)) != EOF) {
                if (c == '"') {
                    break;
                }
                if (c == '\\') {
                    c = read_next_char(file, &line, &column);
                    if (c == EOF) break;
                }
                token->value[pos++] = c;
            }
            token->value[pos] = '\0';
            token_count++;
        }
        else if (is_operator_char(c)) {
            token->type = TOKEN_OPERATOR;
            int pos = 0;
            token->value[pos++] = c;
            
            if (pos < MAX_TOKEN_LEN - 1) {
                int next_c = read_next_char(file, &line, &column);
                if (is_operator_char(next_c)) {
                    token->value[pos++] = next_c;
                } else {
                    unread_char(next_c, file, &column);
                }
            }
            token->value[pos] = '\0';
            token_count++;
        }
        else {
            token->type = TOKEN_INVALID;
            token->value[0] = c;
            token->value[1] = '\0';
            token_count++;
        }
    }
    
    tokens[token_count].type = TOKEN_EOF;
    tokens[token_count].value[0] = '\0';
    tokens[token_count].line = line;
    tokens[token_count].column = column;
    
    return token_count + 1;
}

void print_token_type(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
        case TOKEN_INVALID: printf("INVALID"); break;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", argv[1]);
        return 1;
    }