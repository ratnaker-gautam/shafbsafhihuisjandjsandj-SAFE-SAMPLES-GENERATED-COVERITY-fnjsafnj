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
    TOKEN_ERROR
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

int parse_operator(const char *input, int pos, char *op) {
    int len = 0;
    char c = input[pos];
    
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!') {
        op[len++] = c;
        if (input[pos + 1] == '=') {
            op[len++] = '=';
        }
    } else if ((c == '&' && input[pos + 1] == '&') || (c == '|' && input[pos + 1] == '|')) {
        op[len++] = c;
        op[len++] = c;
    }
    op[len] = '\0';
    return len;
}

int get_next_token(const char *input, int *pos, int *line, int *column, struct Token *token) {
    int start_pos = *pos;
    int start_line = *line;
    int start_column = *column;
    
    while (input[*pos] != '\0') {
        char c = input[*pos];
        
        if (isspace(c)) {
            if (c == '\n') {
                (*line)++;
                *column = 1;
            } else {
                (*column)++;
            }
            (*pos)++;
            continue;
        }
        
        if (isdigit(c)) {
            int len = 0;
            while (isdigit(input[*pos])) {
                if (len < 63) {
                    token->value[len++] = input[*pos];
                }
                (*pos)++;
                (*column)++;
            }
            token->value[len] = '\0';
            token->type = TOKEN_NUMBER;
            token->line = start_line;
            token->column = start_column;
            return 1;
        }
        
        if (isalpha(c) || c == '_') {
            int len = 0;
            while (isalnum(input[*pos]) || input[*pos] == '_') {
                if (len < 63) {
                    token->value[len++] = input[*pos];
                }
                (*pos)++;
                (*column)++;
            }
            token->value[len] = '\0';
            token->type = TOKEN_IDENTIFIER;
            token->line = start_line;
            token->column = start_column;
            return 1;
        }
        
        if (c == '"') {
            int len = 0;
            (*pos)++;
            (*column)++;
            
            while (input[*pos] != '\0' && input[*pos] != '"') {
                if (len < 63) {
                    token->value[len++] = input[*pos];
                }
                (*pos)++;
                (*column)++;
                
                if (input[*pos] == '\0') {
                    token->type = TOKEN_ERROR;
                    strcpy(token->value, "Unterminated string");
                    token->line = start_line;
                    token->column = start_column;
                    return 1;
                }
            }
            
            if (input[*pos] == '"') {
                (*pos)++;
                (*column)++;
            }
            
            token->value[len] = '\0';
            token->type = TOKEN_STRING;
            token->line = start_line;
            token->column = start_column;
            return 1;
        }
        
        if (is_operator_char(c)) {
            char op[4];
            int op_len = parse_operator(input, *pos, op);
            
            if (op_len > 0) {
                strncpy(token->value, op, 63);
                token->value[63] = '\0';
                token->type = TOKEN_OPERATOR;
                token->line = start_line;
                token->column = start_column;
                *pos += op_len;
                *column += op_len;
                return 1;
            }
        }
        
        token->type = TOKEN_ERROR;
        snprintf(token->value, 63, "Unexpected character '%c'", c);
        token->line = start_line;
        token->column = start_column;
        (*pos)++;
        (*column)++;
        return 1;
    }
    
    token->type = TOKEN_EOF;
    strcpy(token->value, "EOF");
    token->line = *line;
    token->column = *column;
    return 1;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter text to tokenize (max 1023 characters):\n");
    
    if (