//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
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
    
    if (is_operator_char(c)) {
        output[len++] = c;
        if (is_operator_char(input[pos + 1])) {
            output[len++] = input[pos + 1];
        }
    }
    output[len] = '\0';
    return len > 0 ? len : 0;
}

int parse_number(const char *input, int pos, char *output) {
    int len = 0;
    int has_dot = 0;
    
    while (input[pos] != '\0') {
        if (isdigit(input[pos])) {
            output[len++] = input[pos];
        } else if (input[pos] == '.' && !has_dot) {
            output[len++] = input[pos];
            has_dot = 1;
        } else {
            break;
        }
        pos++;
    }
    output[len] = '\0';
    return len > 0 ? len : 0;
}

int parse_identifier(const char *input, int pos, char *output) {
    int len = 0;
    
    if (isalpha(input[pos]) || input[pos] == '_') {
        output[len++] = input[pos];
        pos++;
        
        while (input[pos] != '\0' && (isalnum(input[pos]) || input[pos] == '_')) {
            output[len++] = input[pos];
            pos++;
        }
    }
    output[len] = '\0';
    return len > 0 ? len : 0;
}

int parse_string(const char *input, int pos, char *output) {
    int len = 0;
    
    if (input[pos] == '"') {
        output[len++] = input[pos];
        pos++;
        
        while (input[pos] != '\0' && input[pos] != '"') {
            if (input[pos] == '\\' && input[pos + 1] != '\0') {
                output[len++] = input[pos++];
                output[len++] = input[pos++];
            } else {
                output[len++] = input[pos++];
            }
        }
        
        if (input[pos] == '"') {
            output[len++] = input[pos];
        }
    }
    output[len] = '\0';
    return len > 0 ? len : 0;
}

int get_next_token(const char *input, int *pos, int line, int *column, struct Token *token) {
    if (input == NULL || pos == NULL || token == NULL) return 0;
    
    int start_pos = *pos;
    int start_col = *column;
    
    while (input[*pos] != '\0' && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            line++;
            *column = 1;
        } else {
            (*column)++;
        }
        (*pos)++;
    }
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        strcpy(token->value, "");
        token->line = line;
        token->column = *column;
        return 1;
    }
    
    token->line = line;
    token->column = *column;
    
    int len = parse_string(input, *pos, token->value);
    if (len > 0) {
        token->type = TOKEN_STRING;
        *pos += len;
        *column += len;
        return 1;
    }
    
    len = parse_number(input, *pos, token->value);
    if (len > 0) {
        token->type = TOKEN_NUMBER;
        *pos += len;
        *column += len;
        return 1;
    }
    
    len = parse_operator(input, *pos, token->value);
    if (len > 0) {
        token->type = TOKEN_OPERATOR;
        *pos += len;
        *column += len;
        return 1;
    }
    
    len = parse_identifier(input, *pos, token->value);
    if (len > 0) {
        token->type = TOKEN_IDENTIFIER;
        *pos += len;
        *column += len;
        return 1;
    }
    
    token->type = TOKEN_UNKNOWN;
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    (*pos)++;
    (*column)++;
    return 1;
}

void print_token(struct Token *token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "UNKNOWN"
    };
    
    printf("Line %d, Column %d: %s", token->line, token->column, type_names[token->type]);
    if (token->type != TOKEN_EOF) {
        printf(" ('%s')", token->value);
    }
    printf("\n");
}

int main(void) {
    char input[102