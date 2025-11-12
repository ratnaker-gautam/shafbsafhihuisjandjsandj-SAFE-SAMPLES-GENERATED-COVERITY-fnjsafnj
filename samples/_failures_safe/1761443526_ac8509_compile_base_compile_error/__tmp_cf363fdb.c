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
    TOKEN_PUNCTUATION,
    TOKEN_KEYWORD,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
} Token;

const char *keywords[] = {
    "if", "else", "while", "for", "return", "int", "float", "char", "void"
};
const int num_keywords = 9;

int is_keyword(const char *str) {
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_operator(const char *input, int pos, char *op) {
    int len = 0;
    char c = input[pos];
    
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!') {
        op[len++] = c;
        if (input[pos + 1] == '=') {
            op[len++] = '=';
        }
    } else if (c == '&' && input[pos + 1] == '&') {
        op[len++] = '&';
        op[len++] = '&';
    } else if (c == '|' && input[pos + 1] == '|') {
        op[len++] = '|';
        op[len++] = '|';
    } else {
        op[len++] = c;
    }
    op[len] = '\0';
    return len;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    int pos = 0;
    int line = 1;
    int column = 1;
    int token_count = 0;
    int len = strlen(input);
    
    while (pos < len && token_count < max_tokens - 1) {
        char c = input[pos];
        
        if (isspace(c)) {
            if (c == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
            continue;
        }
        
        if (isalpha(c) || c == '_') {
            int start = pos;
            while (pos < len && (isalnum(input[pos]) || input[pos] == '_')) {
                pos++;
                column++;
            }
            
            int token_len = pos - start;
            if (token_len >= MAX_TOKEN_LEN) {
                token_len = MAX_TOKEN_LEN - 1;
            }
            
            strncpy(tokens[token_count].value, &input[start], token_len);
            tokens[token_count].value[token_len] = '\0';
            
            if (is_keyword(tokens[token_count].value)) {
                tokens[token_count].type = TOKEN_KEYWORD;
            } else {
                tokens[token_count].type = TOKEN_IDENTIFIER;
            }
            
            tokens[token_count].line = line;
            tokens[token_count].column = column - token_len;
            token_count++;
            continue;
        }
        
        if (isdigit(c)) {
            int start = pos;
            while (pos < len && (isdigit(input[pos]) || input[pos] == '.')) {
                pos++;
                column++;
            }
            
            int token_len = pos - start;
            if (token_len >= MAX_TOKEN_LEN) {
                token_len = MAX_TOKEN_LEN - 1;
            }
            
            strncpy(tokens[token_count].value, &input[start], token_len);
            tokens[token_count].value[token_len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            tokens[token_count].line = line;
            tokens[token_count].column = column - token_len;
            token_count++;
            continue;
        }
        
        if (c == '"') {
            int start = pos;
            pos++;
            column++;
            
            while (pos < len && input[pos] != '"') {
                if (input[pos] == '\\' && pos + 1 < len) {
                    pos += 2;
                    column += 2;
                } else {
                    pos++;
                    column++;
                }
            }
            
            if (pos < len && input[pos] == '"') {
                pos++;
                column++;
            }
            
            int token_len = pos - start;
            if (token_len >= MAX_TOKEN_LEN) {
                token_len = MAX_TOKEN_LEN - 1;
            }
            
            strncpy(tokens[token_count].value, &input[start], token_len);
            tokens[token_count].value[token_len] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            tokens[token_count].line = line;
            tokens[token_count].column = column - token_len;
            token_count++;
            continue;
        }
        
        if (is_operator_char(c)) {
            char op[3];
            int op_len = parse_operator(input, pos, op);
            
            strncpy