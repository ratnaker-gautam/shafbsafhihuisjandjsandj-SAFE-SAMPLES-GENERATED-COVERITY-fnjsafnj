//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
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
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

struct Token get_next_token(const char *input, int *pos, int *line, int *column) {
    struct Token token = {TOKEN_INVALID, "", *line, *column};
    int i = 0;
    
    while (input[*pos] != '\0' && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            (*line)++;
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
    
    token.line = *line;
    token.column = *column;
    
    if (isdigit(input[*pos])) {
        token.type = TOKEN_NUMBER;
        while (input[*pos] != '\0' && isdigit(input[*pos]) && i < 31) {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*column)++;
        }
        token.value[i] = '\0';
    } else if (isalpha(input[*pos]) || input[*pos] == '_') {
        token.type = TOKEN_IDENTIFIER;
        while (input[*pos] != '\0' && (isalnum(input[*pos]) || input[*pos] == '_') && i < 31) {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*column)++;
        }
        token.value[i] = '\0';
    } else if (is_operator_char(input[*pos])) {
        token.type = TOKEN_OPERATOR;
        while (input[*pos] != '\0' && is_operator_char(input[*pos]) && i < 31) {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*column)++;
        }
        token.value[i] = '\0';
    } else if (input[*pos] == '"') {
        token.type = TOKEN_STRING;
        token.value[i++] = input[*pos];
        (*pos)++;
        (*column)++;
        
        while (input[*pos] != '\0' && input[*pos] != '"' && i < 31) {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*column)++;
        }
        
        if (input[*pos] == '"') {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*column)++;
            token.value[i] = '\0';
        } else {
            token.type = TOKEN_INVALID;
        }
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = input[*pos];
        token.value[1] = '\0';
        (*pos)++;
        (*column)++;
    }
    
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "INVALID"
    };
    
    printf("Line %d, Column %d: %s", token.line, token.column, type_names[token.type]);
    
    if (token.type != TOKEN_EOF) {
        printf(" ('%s')", token.value);
    }
    printf("\n");
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
        input[len - 1] = '\0';
    }
    
    int pos = 0;
    int line = 1;
    int column = 1;
    
    printf("Tokens:\n");
    
    while (1) {
        struct Token token = get_next_token(input, &pos, &line, &column);
        
        switch (token.type) {
            case TOKEN_NUMBER:
            case TOKEN_IDENTIFIER:
            case TOKEN_OPERATOR:
            case TOKEN_STRING:
            case TOKEN_INVALID:
                print_token(token);
                break;
            case TOKEN_EOF:
                print_token(token);
                return 0;
        }
        
        if (pos >= (int)strlen(input)) {
            struct Token eof_token = {TOKEN_EOF, "", line, column};
            print_token(eof_token);
            break;
        }
    }
    
    return 0;
}