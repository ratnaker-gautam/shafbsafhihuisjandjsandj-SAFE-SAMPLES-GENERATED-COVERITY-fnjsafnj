//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <ctype.h>
#include <string.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '(' || c == ')';
}

struct Token get_next_token(const char *input, int *pos, int line, int *column) {
    struct Token token = {TOKEN_INVALID, "", line, *column};
    
    if (input[*pos] == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        return token;
    }
    
    while (isspace((unsigned char)input[*pos])) {
        if (input[*pos] == '\n') {
            line++;
            *column = 1;
        } else {
            (*column)++;
        }
        (*pos)++;
    }
    
    token.line = line;
    token.column = *column;
    
    if (input[*pos] == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        return token;
    }
    
    if (isdigit((unsigned char)input[*pos])) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit((unsigned char)input[*pos]) && i < 31) {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*column)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (isalpha((unsigned char)input[*pos])) {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum((unsigned char)input[*pos]) || input[*pos] == '_') && i < 31) {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*column)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (is_operator_char(input[*pos])) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (is_operator_char(input[*pos]) && i < 31) {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*column)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    token.value[0] = input[*pos];
    token.value[1] = '\0';
    (*pos)++;
    (*column)++;
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "EOF", "INVALID"
    };
    printf("Line %d, Column %d: %s '%s'\n", 
           token.line, token.column, type_names[token.type], token.value);
}

void parse_expression(const char *input, int *pos, int line, int *column);

void parse_primary(const char *input, int *pos, int line, int *column) {
    struct Token token = get_next_token(input, pos, line, column);
    
    if (token.type == TOKEN_NUMBER) {
        printf("Found number: %s\n", token.value);
    } else if (token.type == TOKEN_IDENTIFIER) {
        printf("Found identifier: %s\n", token.value);
    } else if (token.type == TOKEN_OPERATOR && token.value[0] == '(') {
        printf("Entering parenthesized expression\n");
        parse_expression(input, pos, line, column);
        struct Token close_paren = get_next_token(input, pos, line, column);
        if (close_paren.type != TOKEN_OPERATOR || close_paren.value[0] != ')') {
            printf("Error: Expected closing parenthesis at line %d, column %d\n", 
                   close_paren.line, close_paren.column);
        } else {
            printf("Exiting parenthesized expression\n");
        }
    } else {
        printf("Error: Unexpected token '%s' at line %d, column %d\n", 
               token.value, token.line, token.column);
    }
}

void parse_expression(const char *input, int *pos, int line, int *column) {
    parse_primary(input, pos, line, column);
    
    struct Token token = get_next_token(input, pos, line, column);
    
    if (token.type == TOKEN_OPERATOR && 
        (token.value[0] == '+' || token.value[0] == '-' || 
         token.value[0] == '*' || token.value[0] == '/')) {
        printf("Found operator: %s\n", token.value);
        parse_expression(input, pos, line, column);
    } else {
        *pos -= (int)strlen(token.value);
        *column -= (int)strlen(token.value);
    }
}

int main() {
    char input[1024];
    
    printf("Enter an expression to parse: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len