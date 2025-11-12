//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 1000
#define MAX_TOKEN_LEN 256

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
} Token;

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_operator(const char *input, int pos, char *buffer) {
    int len = 0;
    char c = input[pos];
    
    if (c == '&' && input[pos + 1] == '&') {
        buffer[len++] = '&';
        buffer[len++] = '&';
    } else if (c == '|' && input[pos + 1] == '|') {
        buffer[len++] = '|';
        buffer[len++] = '|';
    } else if (c == '=' && input[pos + 1] == '=') {
        buffer[len++] = '=';
        buffer[len++] = '=';
    } else if (c == '!' && input[pos + 1] == '=') {
        buffer[len++] = '!';
        buffer[len++] = '=';
    } else if (c == '<' && input[pos + 1] == '=') {
        buffer[len++] = '<';
        buffer[len++] = '=';
    } else if (c == '>' && input[pos + 1] == '=') {
        buffer[len++] = '>';
        buffer[len++] = '=';
    } else {
        buffer[len++] = c;
    }
    
    buffer[len] = '\0';
    return len;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    int pos = 0;
    int line = 1;
    int column = 1;
    int token_count = 0;
    
    while (input[pos] != '\0' && token_count < max_tokens - 1) {
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
        
        Token *token = &tokens[token_count];
        token->line = line;
        token->column = column;
        
        if (isalpha((unsigned char)input[pos]) || input[pos] == '_') {
            int len = 0;
            while ((isalnum((unsigned char)input[pos]) || input[pos] == '_') && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = input[pos++];
                column++;
            }
            token->value[len] = '\0';
            token->type = TOKEN_IDENTIFIER;
            token_count++;
        } else if (isdigit((unsigned char)input[pos])) {
            int len = 0;
            while (isdigit((unsigned char)input[pos]) && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = input[pos++];
                column++;
            }
            token->value[len] = '\0';
            token->type = TOKEN_NUMBER;
            token_count++;
        } else if (input[pos] == '"') {
            int len = 0;
            token->value[len++] = input[pos++];
            column++;
            
            while (input[pos] != '"' && input[pos] != '\0' && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = input[pos++];
                column++;
            }
            
            if (input[pos] == '"') {
                token->value[len++] = input[pos++];
                column++;
            }
            token->value[len] = '\0';
            token->type = TOKEN_STRING;
            token_count++;
        } else if (is_operator_char(input[pos])) {
            int len = parse_operator(input, pos, token->value);
            pos += len;
            column += len;
            token->type = TOKEN_OPERATOR;
            token_count++;
        } else {
            token->value[0] = input[pos];
            token->value[1] = '\0';
            token->type = TOKEN_UNKNOWN;
            token_count++;
            pos++;
            column++;
        }
    }
    
    tokens[token_count].type = TOKEN_EOF;
    tokens[token_count].value[0] = '\0';
    tokens[token_count].line = line;
    tokens[token_count].column = column;
    
    return token_count + 1;
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input_buffer[4096];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return