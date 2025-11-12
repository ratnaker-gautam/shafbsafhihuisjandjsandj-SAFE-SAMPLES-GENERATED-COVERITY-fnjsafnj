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

int get_operator_length(const char *str) {
    if (str[0] == '&' && str[1] == '&') return 2;
    if (str[0] == '|' && str[1] == '|') return 2;
    if (str[0] == '=' && str[1] == '=') return 2;
    if (str[0] == '!' && str[1] == '=') return 2;
    if (str[0] == '<' && str[1] == '=') return 2;
    if (str[0] == '>' && str[1] == '=') return 2;
    if (is_operator_char(str[0])) return 1;
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int line = 1;
    int column = 1;
    int token_count = 0;
    
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
            int len = 0;
            while (isalnum(input[pos]) || input[pos] == '_') {
                if (len < 63) {
                    tokens[token_count].value[len++] = input[pos];
                }
                pos++;
                column++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
        } else if (isdigit(input[pos])) {
            int len = 0;
            while (isdigit(input[pos])) {
                if (len < 63) {
                    tokens[token_count].value[len++] = input[pos];
                }
                pos++;
                column++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        } else if (input[pos] == '"') {
            int len = 0;
            pos++;
            column++;
            while (input[pos] != '"' && input[pos] != '\0') {
                if (len < 63) {
                    tokens[token_count].value[len++] = input[pos];
                }
                pos++;
                column++;
            }
            if (input[pos] == '"') {
                pos++;
                column++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
        } else {
            int op_len = get_operator_length(&input[pos]);
            if (op_len > 0) {
                for (int i = 0; i < op_len && i < 63; i++) {
                    tokens[token_count].value[i] = input[pos + i];
                }
                tokens[token_count].value[op_len] = '\0';
                tokens[token_count].type = TOKEN_OPERATOR;
                pos += op_len;
                column += op_len;
                token_count++;
            } else {
                tokens[token_count].value[0] = input[pos];
                tokens[token_count].value[1] = '\0';
                tokens[token_count].type = TOKEN_UNKNOWN;
                pos++;
                column++;
                token_count++;
            }
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

const char *token_type_to_string(enum TokenType type) {
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
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len