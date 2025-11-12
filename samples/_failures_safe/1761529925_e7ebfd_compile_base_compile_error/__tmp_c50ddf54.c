//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_STRING, TOKEN_EOF, TOKEN_ERROR };

struct Token {
    enum TokenType type;
    char value[256];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_operator(const char *input, int pos, char *output) {
    int len = 0;
    char c = input[pos];
    
    if (c == '&' && input[pos + 1] == '&') {
        strcpy(output, "&&");
        len = 2;
    } else if (c == '|' && input[pos + 1] == '|') {
        strcpy(output, "||");
        len = 2;
    } else if (c == '=' && input[pos + 1] == '=') {
        strcpy(output, "==");
        len = 2;
    } else if (c == '!' && input[pos + 1] == '=') {
        strcpy(output, "!=");
        len = 2;
    } else if (c == '<' && input[pos + 1] == '=') {
        strcpy(output, "<=");
        len = 2;
    } else if (c == '>' && input[pos + 1] == '=') {
        strcpy(output, ">=");
        len = 2;
    } else if (is_operator_char(c)) {
        output[0] = c;
        output[1] = '\0';
        len = 1;
    }
    
    return len;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int line = 1;
    int column = 1;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        while (isspace(input[pos])) {
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
        
        if (isalpha(input[pos]) || input[pos] == '_') {
            int i = 0;
            while ((isalnum(input[pos]) || input[pos] == '_') && i < 255) {
                tokens[token_count].value[i++] = input[pos++];
                column++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
        } else if (isdigit(input[pos])) {
            int i = 0;
            int has_dot = 0;
            while ((isdigit(input[pos]) || input[pos] == '.') && i < 255) {
                if (input[pos] == '.') {
                    if (has_dot) break;
                    has_dot = 1;
                }
                tokens[token_count].value[i++] = input[pos++];
                column++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        } else if (input[pos] == '"') {
            int i = 0;
            pos++;
            column++;
            while (input[pos] != '"' && input[pos] != '\0' && i < 255) {
                tokens[token_count].value[i++] = input[pos++];
                column++;
            }
            if (input[pos] == '"') {
                pos++;
                column++;
            } else {
                tokens[token_count].type = TOKEN_ERROR;
                strcpy(tokens[token_count].value, "Unterminated string");
                token_count++;
                break;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
        } else if (is_operator_char(input[pos])) {
            int len = parse_operator(input, pos, tokens[token_count].value);
            if (len > 0) {
                pos += len;
                column += len;
                tokens[token_count].type = TOKEN_OPERATOR;
                token_count++;
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
            tokens[token_count].type = TOKEN_ERROR;
            token_count++;
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