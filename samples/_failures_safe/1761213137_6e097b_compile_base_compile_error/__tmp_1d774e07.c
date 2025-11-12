//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_STRING, TOKEN_EOF, TOKEN_ERROR };

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int get_operator_length(const char* str) {
    if (str[0] == '\0') return 0;
    if (str[1] == '\0') return 1;
    if ((str[0] == '&' && str[1] == '&') || (str[0] == '|' && str[1] == '|') || 
        (str[0] == '=' && str[1] == '=') || (str[0] == '!' && str[1] == '=') ||
        (str[0] == '<' && str[1] == '=') || (str[0] == '>' && str[1] == '=')) {
        return 2;
    }
    return 1;
}

const char* token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int tokenize(const char* input, struct Token* tokens, int max_tokens) {
    int pos = 0;
    int line = 1;
    int column = 1;
    int token_count = 0;
    
    if (input == NULL || tokens == NULL || max_tokens <= 0) {
        return 0;
    }
    
    while (input[pos] != '\0' && token_count < max_tokens) {
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
        
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        
        if (isalpha((unsigned char)input[pos]) || input[pos] == '_') {
            int len = 0;
            while ((isalnum((unsigned char)input[pos]) || input[pos] == '_') && len < 31) {
                tokens[token_count].value[len++] = input[pos++];
                column++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
        } else if (isdigit((unsigned char)input[pos])) {
            int len = 0;
            int has_dot = 0;
            while ((isdigit((unsigned char)input[pos]) || input[pos] == '.') && len < 31) {
                if (input[pos] == '.') {
                    if (has_dot) break;
                    has_dot = 1;
                }
                tokens[token_count].value[len++] = input[pos++];
                column++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        } else if (input[pos] == '"') {
            int len = 0;
            tokens[token_count].value[len++] = input[pos++];
            column++;
            while (input[pos] != '"' && input[pos] != '\0' && len < 31) {
                if (input[pos] == '\\' && input[pos + 1] != '\0') {
                    tokens[token_count].value[len++] = input[pos++];
                    column++;
                }
                tokens[token_count].value[len++] = input[pos++];
                column++;
            }
            if (input[pos] == '"') {
                tokens[token_count].value[len++] = input[pos++];
                column++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
        } else if (is_operator_char(input[pos])) {
            int op_len = get_operator_length(input + pos);
            if (op_len > 0 && op_len < 32) {
                strncpy(tokens[token_count].value, input + pos, op_len);
                tokens[token_count].value[op_len] = '\0';
                tokens[token_count].type = TOKEN_OPERATOR;
                token_count++;
                pos += op_len;
                column += op_len;
            } else {
                tokens[token_count].value[0] = input[pos];
                tokens[token_count].value[1] = '\0';
                tokens[token_count].type = TOKEN_ERROR;
                token_count++;
                pos++;
                column++;
            }
        } else {
            tokens[token_count].value[0] = input[pos];
            tokens[token_count].value[1] = '\0';