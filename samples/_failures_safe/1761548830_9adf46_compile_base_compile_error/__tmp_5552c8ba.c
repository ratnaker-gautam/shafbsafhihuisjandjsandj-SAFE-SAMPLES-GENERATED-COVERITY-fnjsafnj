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
    int value_pos = 0;
    
    while (input[*pos] != '\0') {
        char c = input[*pos];
        
        if (isspace(c)) {
            if (c == '\n') {
                (*line)++;
                *col = 1;
            } else {
                (*col)++;
            }
            (*pos)++;
            continue;
        }
        
        if (isdigit(c)) {
            token.type = TOKEN_NUMBER;
            token.line = *line;
            token.column = *col;
            
            while (isdigit(input[*pos]) && value_pos < 31) {
                token.value[value_pos++] = input[*pos];
                (*pos)++;
                (*col)++;
            }
            token.value[value_pos] = '\0';
            return token;
        }
        
        if (isalpha(c) || c == '_') {
            token.type = TOKEN_IDENTIFIER;
            token.line = *line;
            token.column = *col;
            
            while ((isalnum(input[*pos]) || input[*pos] == '_') && value_pos < 31) {
                token.value[value_pos++] = input[*pos];
                (*pos)++;
                (*col)++;
            }
            token.value[value_pos] = '\0';
            return token;
        }
        
        if (c == '"') {
            token.type = TOKEN_STRING;
            token.line = *line;
            token.column = *col;
            
            token.value[value_pos++] = input[*pos];
            (*pos)++;
            (*col)++;
            
            while (input[*pos] != '"' && input[*pos] != '\0' && value_pos < 31) {
                token.value[value_pos++] = input[*pos];
                (*pos)++;
                (*col)++;
            }
            
            if (input[*pos] == '"') {
                token.value[value_pos++] = input[*pos];
                (*pos)++;
                (*col)++;
            }
            token.value[value_pos] = '\0';
            return token;
        }
        
        if (is_operator_char(c)) {
            token.type = TOKEN_OPERATOR;
            token.line = *line;
            token.column = *col;
            
            while (is_operator_char(input[*pos]) && value_pos < 31) {
                token.value[value_pos++] = input[*pos];
                (*pos)++;
                (*col)++;
            }
            token.value[value_pos] = '\0';
            return token;
        }
        
        token.type = TOKEN_INVALID;
        token.value[0] = c;
        token.value[1] = '\0';
        token.line = *line;
        token.column = *col;
        (*pos)++;
        (*col)++;
        return token;
    }
    
    token.type = TOKEN_EOF;
    token.value[0] = '\0';
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

void parse_expression(struct Token token) {
    switch (token.type) {
        case TOKEN_NUMBER:
            printf("Parsing number: %s\n", token.value);
            break;
        case TOKEN_IDENTIFIER:
            printf("Parsing identifier: %s\n", token.value);
            break;
        case TOKEN_STRING:
            printf("Parsing string: %s\n", token.value);
            break;
        case TOKEN_OPERATOR:
            printf("Parsing operator: %s\n", token.value);
            break;
        case TOKEN_INVALID:
            printf("Error: Invalid token '%s' at line %d, column %d\n", 
                   token.value, token.line, token.column);
            break;
        default:
            break;
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to tokenize (max 1023 chars): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int