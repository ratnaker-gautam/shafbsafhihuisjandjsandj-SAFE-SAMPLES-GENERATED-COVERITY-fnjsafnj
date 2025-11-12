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

struct Token get_next_token(const char *input, int *pos, int *line, int *column) {
    struct Token token = {TOKEN_ERROR, "", *line, *column};
    int start_pos = *pos;
    
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
                token.value[i++] = input[(*pos)++];
                (*column)++;
            }
            token.value[i] = '\0';
            token.type = TOKEN_NUMBER;
            return token;
        }
        
        if (isalpha(c) || c == '_') {
            int i = 0;
            while ((isalnum(input[*pos]) || input[*pos] == '_') && i < 63) {
                token.value[i++] = input[(*pos)++];
                (*column)++;
            }
            token.value[i] = '\0';
            token.type = TOKEN_IDENTIFIER;
            return token;
        }
        
        if (c == '"') {
            int i = 0;
            (*pos)++;
            (*column)++;
            
            while (input[*pos] != '\0' && input[*pos] != '"' && i < 63) {
                if (input[*pos] == '\\' && input[*pos + 1] != '\0') {
                    token.value[i++] = input[(*pos)++];
                    (*column)++;
                }
                token.value[i++] = input[(*pos)++];
                (*column)++;
            }
            
            if (input[*pos] == '"') {
                (*pos)++;
                (*column)++;
                token.value[i] = '\0';
                token.type = TOKEN_STRING;
                return token;
            } else {
                token.type = TOKEN_ERROR;
                strcpy(token.value, "Unterminated string");
                return token;
            }
        }
        
        if (is_operator_char(c)) {
            int op_len = parse_operator(input, *pos, token.value);
            if (op_len > 0) {
                *pos += op_len;
                *column += op_len;
                token.type = TOKEN_OPERATOR;
                return token;
            }
        }
        
        token.type = TOKEN_ERROR;
        snprintf(token.value, sizeof(token.value), "Unexpected character '%c'", c);
        (*pos)++;
        (*column)++;
        return token;
    }
    
    token.type = TOKEN_EOF;
    strcpy(token.value, "EOF");
    return token;
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
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if