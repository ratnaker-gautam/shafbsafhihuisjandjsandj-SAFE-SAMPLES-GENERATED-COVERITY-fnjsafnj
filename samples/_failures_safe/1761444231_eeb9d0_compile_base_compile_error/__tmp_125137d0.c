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

struct Token get_next_token(const char *input, int *pos, int *line, int *col) {
    struct Token token = {TOKEN_INVALID, "", *line, *col};
    int i = 0;
    
    if (input[*pos] == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    while (isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            (*line)++;
            *col = 1;
        } else {
            (*col)++;
        }
        (*pos)++;
        if (input[*pos] == '\0') {
            token.type = TOKEN_EOF;
            return token;
        }
    }
    
    token.line = *line;
    token.column = *col;
    
    if (isdigit(input[*pos])) {
        token.type = TOKEN_NUMBER;
        while (isdigit(input[*pos]) && i < 31) {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token.value[i] = '\0';
    } else if (isalpha(input[*pos]) || input[*pos] == '_') {
        token.type = TOKEN_IDENTIFIER;
        while ((isalnum(input[*pos]) || input[*pos] == '_') && i < 31) {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token.value[i] = '\0';
    } else if (is_operator_char(input[*pos])) {
        token.type = TOKEN_OPERATOR;
        while (is_operator_char(input[*pos]) && i < 31) {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token.value[i] = '\0';
    } else if (input[*pos] == '"') {
        token.type = TOKEN_STRING;
        token.value[i++] = input[*pos];
        (*pos)++;
        (*col)++;
        while (input[*pos] != '"' && input[*pos] != '\0' && i < 31) {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        if (input[*pos] == '"') {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token.value[i] = '\0';
    } else {
        token.value[i++] = input[*pos];
        token.value[i] = '\0';
        (*pos)++;
        (*col)++;
    }
    
    return token;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
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
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len <= 1) {
        printf("No input provided\n");
        return 1;
    }
    
    int pos = 0;
    int line = 1;
    int col = 1;
    
    printf("\nToken stream:\n");
    printf("%-12s %-12s %-8s %-8s\n", "TYPE", "VALUE", "LINE", "COLUMN");
    printf("----------------------------------------\n");
    
    while (1) {
        struct Token token = get_next_token(input, &pos, &line, &col);
        
        switch (token.type) {
            case TOKEN_NUMBER:
            case TOKEN_IDENTIFIER:
            case TOKEN_OPERATOR:
            case TOKEN_STRING:
            case TOKEN_INVALID:
                printf("%-12s %-12s %-8d %-8d\n", 
                       token_type_to_string(token.type), 
                       token.value, 
                       token.line, 
                       token.column);
                break;
            case TOKEN_EOF:
                printf("%-12s %-12s %-8d %-8d\n", 
                       token_type_to_string(token.type), 
                       "", 
                       token.line, 
                       token.column);
                return 0;
            default:
                break;
        }
        
        if (pos >= (int)strlen(input))