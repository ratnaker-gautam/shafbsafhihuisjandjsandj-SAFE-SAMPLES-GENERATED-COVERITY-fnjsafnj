//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
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
    char c = input[pos];
    
    if (is_operator_char(c)) {
        output[len++] = c;
        if (is_operator_char(input[pos + 1])) {
            output[len++] = input[pos + 1];
        }
        output[len] = '\0';
    }
    return len;
}

int parse_number(const char *input, int pos, char *output) {
    int len = 0;
    while (isdigit(input[pos])) {
        if (len < 31) {
            output[len++] = input[pos++];
        } else {
            break;
        }
    }
    output[len] = '\0';
    return len;
}

int parse_string(const char *input, int pos, char *output) {
    int len = 0;
    if (input[pos] == '"') {
        pos++;
        while (input[pos] != '"' && input[pos] != '\0') {
            if (len < 31) {
                output[len++] = input[pos++];
            } else {
                break;
            }
        }
        output[len] = '\0';
        return len + 2;
    }
    return 0;
}

int parse_identifier(const char *input, int pos, char *output) {
    int len = 0;
    if (isalpha(input[pos]) || input[pos] == '_') {
        while (isalnum(input[pos]) || input[pos] == '_') {
            if (len < 31) {
                output[len++] = input[pos++];
            } else {
                break;
            }
        }
        output[len] = '\0';
    }
    return len;
}

struct Token get_next_token(const char *input, int *pos, int line, int *column) {
    struct Token token = {TOKEN_INVALID, "", line, *column};
    
    while (input[*pos] != '\0') {
        char c = input[*pos];
        
        if (isspace(c)) {
            if (c == '\n') {
                line++;
                *column = 1;
            } else {
                (*column)++;
            }
            (*pos)++;
            continue;
        }
        
        int advance = 0;
        
        if (is_operator_char(c)) {
            advance = parse_operator(input, *pos, token.value);
            if (advance > 0) {
                token.type = TOKEN_OPERATOR;
            }
        } else if (isdigit(c)) {
            advance = parse_number(input, *pos, token.value);
            if (advance > 0) {
                token.type = TOKEN_NUMBER;
            }
        } else if (c == '"') {
            advance = parse_string(input, *pos, token.value);
            if (advance > 0) {
                token.type = TOKEN_STRING;
                advance--;
            }
        } else if (isalpha(c) || c == '_') {
            advance = parse_identifier(input, *pos, token.value);
            if (advance > 0) {
                token.type = TOKEN_IDENTIFIER;
            }
        }
        
        if (advance > 0) {
            *pos += advance;
            *column += advance;
            break;
        } else {
            token.value[0] = c;
            token.value[1] = '\0';
            (*pos)++;
            (*column)++;
            break;
        }
    }
    
    if (input[*pos] == '\0' && token.type == TOKEN_INVALID) {
        token.type = TOKEN_EOF;
    }
    
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF", "INVALID"
    };
    
    printf("Line %d, Column %d: %s", token.line, token.column, type_names[token.type]);
    
    if (token.type != TOKEN_EOF) {
        printf(" ('%s')", token.value);
    }
    printf("\n");
}

int main(void) {
    char input[1024];
    
    printf("Enter input to tokenize (max 1023 characters): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    int pos = 0;
    int line = 1;
    int column = 1;
    
    printf("\nTokens:\n");
    
    while (1) {
        struct Token token = get_next_token(input, &pos, line, &column);