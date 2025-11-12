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
            int i = 0;
            while (isdigit(input[*pos]) && i < 63) {
                token->value[i++] = input[(*pos)++];
                (*column)++;
            }
            token->value[i] = '\0';
            token->type = TOKEN_NUMBER;
            token->line = start_line;
            token->column = start_column;
            return 1;
        }
        
        if (isalpha(c) || c == '_') {
            int i = 0;
            while ((isalnum(input[*pos]) || input[*pos] == '_') && i < 63) {
                token->value[i++] = input[(*pos)++];
                (*column)++;
            }
            token->value[i] = '\0';
            token->type = TOKEN_IDENTIFIER;
            token->line = start_line;
            token->column = start_column;
            return 1;
        }
        
        if (c == '"') {
            int i = 0;
            (*pos)++;
            (*column)++;
            
            while (input[*pos] != '\0' && input[*pos] != '"' && i < 63) {
                token->value[i++] = input[(*pos)++];
                (*column)++;
            }
            
            if (input[*pos] == '"') {
                (*pos)++;
                (*column)++;
            }
            
            token->value[i] = '\0';
            token->type = TOKEN_STRING;
            token->line = start_line;
            token->column = start_column;
            return 1;
        }
        
        if (is_operator_char(c)) {
            int op_len = parse_operator(input, *pos, token->value);
            if (op_len > 0) {
                *pos += op_len;
                *column += op_len;
                token->type = TOKEN_OPERATOR;
                token->line = start_line;
                token->column = start_column;
                return 1;
            }
        }
        
        token->value[0] = c;
        token->value[1] = '\0';
        token->type = TOKEN_UNKNOWN;
        token->line = start_line;
        token->column = start_column;
        (*pos)++;
        (*column)++;
        return 1;
    }
    
    token->type = TOKEN_EOF;
    token->value[0] = '\0';
    token->line = start_line;
    token->column = start_column;
    return 0;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_UNKNOWN: return "UNKNOWN";
        default: return "INVALID";
    }
}

int main(void) {
    char input[1024];
    printf("Enter text to tokenize (max