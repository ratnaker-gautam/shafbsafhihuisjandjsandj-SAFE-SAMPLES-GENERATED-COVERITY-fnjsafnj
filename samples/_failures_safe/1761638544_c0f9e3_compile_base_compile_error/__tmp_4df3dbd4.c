//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_operator(const char *input, int pos, char *output) {
    int len = 0;
    if (input == NULL || output == NULL || pos < 0) return 0;
    
    char c = input[pos];
    if (c == '\0') return 0;
    
    if (is_operator_char(c)) {
        output[len++] = c;
        char next = input[pos + 1];
        
        if ((c == '&' && next == '&') || (c == '|' && next == '|') ||
            (c == '=' && next == '=') || (c == '!' && next == '=') ||
            (c == '<' && next == '=') || (c == '>' && next == '=')) {
            output[len++] = next;
        }
    }
    
    output[len] = '\0';
    return len;
}

int parse_string(const char *input, int pos, char *output) {
    int len = 0;
    if (input == NULL || output == NULL || pos < 0) return 0;
    
    if (input[pos] == '"') {
        output[len++] = '"';
        pos++;
        
        while (input[pos] != '\0' && input[pos] != '"') {
            if (len < 31) {
                output[len++] = input[pos];
            }
            pos++;
        }
        
        if (input[pos] == '"') {
            output[len++] = '"';
        }
    }
    
    output[len] = '\0';
    return len;
}

int parse_number(const char *input, int pos, char *output) {
    int len = 0;
    if (input == NULL || output == NULL || pos < 0) return 0;
    
    while (isdigit((unsigned char)input[pos])) {
        if (len < 31) {
            output[len++] = input[pos];
        }
        pos++;
    }
    
    output[len] = '\0';
    return len;
}

int parse_identifier(const char *input, int pos, char *output) {
    int len = 0;
    if (input == NULL || output == NULL || pos < 0) return 0;
    
    if (isalpha((unsigned char)input[pos]) || input[pos] == '_') {
        while (isalnum((unsigned char)input[pos]) || input[pos] == '_') {
            if (len < 31) {
                output[len++] = input[pos];
            }
            pos++;
        }
    }
    
    output[len] = '\0';
    return len;
}

struct Token get_next_token(const char *input, int *pos, int line, int *column) {
    struct Token token = {TOKEN_INVALID, "", line, *column};
    if (input == NULL || pos == NULL || column == NULL) return token;
    
    while (input[*pos] != '\0' && isspace((unsigned char)input[*pos])) {
        if (input[*pos] == '\n') {
            line++;
            *column = 1;
        } else {
            (*column)++;
        }
        (*pos)++;
    }
    
    if (input[*pos] == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    token.line = line;
    token.column = *column;
    
    int len;
    
    if ((len = parse_operator(input, *pos, token.value)) > 0) {
        token.type = TOKEN_OPERATOR;
    } else if ((len = parse_string(input, *pos, token.value)) > 0) {
        token.type = TOKEN_STRING;
    } else if ((len = parse_number(input, *pos, token.value)) > 0) {
        token.type = TOKEN_NUMBER;
    } else if ((len = parse_identifier(input, *pos, token.value)) > 0) {
        token.type = TOKEN_IDENTIFIER;
    } else {
        if (len < 31) {
            token.value[0] = input[*pos];
            token.value[1] = '\0';
            len = 1;
        }
    }
    
    *pos += len;
    *column += len;
    
    return token;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_INVALID: return "INVALID";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
    if (