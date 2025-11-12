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

int parse_operator(const char *input, int pos, char *buffer) {
    char c = input[pos];
    
    if (c == '&' && input[pos + 1] == '&') {
        buffer[0] = '&'; buffer[1] = '&'; buffer[2] = '\0';
        return 2;
    } else if (c == '|' && input[pos + 1] == '|') {
        buffer[0] = '|'; buffer[1] = '|'; buffer[2] = '\0';
        return 2;
    } else if (c == '=' && input[pos + 1] == '=') {
        buffer[0] = '='; buffer[1] = '='; buffer[2] = '\0';
        return 2;
    } else if (c == '!' && input[pos + 1] == '=') {
        buffer[0] = '!'; buffer[1] = '='; buffer[2] = '\0';
        return 2;
    } else if (c == '<' && input[pos + 1] == '=') {
        buffer[0] = '<'; buffer[1] = '='; buffer[2] = '\0';
        return 2;
    } else if (c == '>' && input[pos + 1] == '=') {
        buffer[0] = '>'; buffer[1] = '='; buffer[2] = '\0';
        return 2;
    } else {
        buffer[0] = c;
        buffer[1] = '\0';
        return 1;
    }
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "UNKNOWN"
    };
    
    printf("Line %d, Column %d: %s (%s)\n", token.line, token.column, token.value, type_names[token.type]);
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int line = 1;
    int column = 1;
    
    if (input == NULL || tokens == NULL || max_tokens <= 0) {
        return 0;
    }
    
    while (input[pos] != '\0' && token_count < max_tokens - 1) {
        if (isspace((unsigned char)input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
            continue;
        }
        
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        
        if (isdigit((unsigned char)input[pos])) {
            tokens[token_count].type = TOKEN_NUMBER;
            int i = 0;
            while (isdigit((unsigned char)input[pos]) && i < 63) {
                tokens[token_count].value[i++] = input[pos++];
                column++;
            }
            tokens[token_count].value[i] = '\0';
            token_count++;
        } else if (isalpha((unsigned char)input[pos]) || input[pos] == '_') {
            tokens[token_count].type = TOKEN_IDENTIFIER;
            int i = 0;
            while ((isalnum((unsigned char)input[pos]) || input[pos] == '_') && i < 63) {
                tokens[token_count].value[i++] = input[pos++];
                column++;
            }
            tokens[token_count].value[i] = '\0';
            token_count++;
        } else if (is_operator_char(input[pos])) {
            tokens[token_count].type = TOKEN_OPERATOR;
            int advance = parse_operator(input, pos, tokens[token_count].value);
            pos += advance;
            column += advance;
            token_count++;
        } else if (input[pos] == '"') {
            tokens[token_count].type = TOKEN_STRING;
            int i = 0;
            tokens[token_count].value[i++] = input[pos++];
            column++;
            
            while (input[pos] != '"' && input[pos] != '\0' && i < 63) {
                tokens[token_count].value[i++] = input[pos++];
                column++;
            }
            
            if (input[pos] == '"') {
                tokens[token_count].value[i++] = input[pos++];
                column++;
            }
            tokens[token_count].value[i] = '\0';
            token_count++;
        } else {
            tokens[token_count].type = TOKEN_UNKNOWN;
            tokens[token_count].value[0] = input[pos];
            tokens[token_count].value[1] = '\0';
            pos++;
            column++;
            token_count++;
        }
    }
    
    tokens[token_count].type = TOKEN_EOF;
    strcpy(tokens[token_count