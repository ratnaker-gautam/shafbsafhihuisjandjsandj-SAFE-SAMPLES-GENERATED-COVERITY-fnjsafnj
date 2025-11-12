//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <ctype.h>
#include <string.h>

enum TokenType {
    TOK_NUMBER,
    TOK_IDENTIFIER,
    TOK_OPERATOR,
    TOK_EOF,
    TOK_INVALID
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

struct Token get_next_token(const char *input, int *pos, int line, int *col) {
    struct Token token = {TOK_INVALID, "", line, *col};
    int input_len = strlen(input);
    
    if (*pos >= input_len) {
        token.type = TOK_EOF;
        strcpy(token.value, "EOF");
        return token;
    }
    
    while (*pos < input_len && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            line++;
            *col = 1;
        } else {
            (*col)++;
        }
        (*pos)++;
    }
    
    if (*pos >= input_len) {
        token.type = TOK_EOF;
        strcpy(token.value, "EOF");
        return token;
    }
    
    token.line = line;
    token.column = *col;
    
    if (isdigit(input[*pos])) {
        token.type = TOK_NUMBER;
        int i = 0;
        while (*pos < input_len && isdigit(input[*pos]) && i < 31) {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token.value[i] = '\0';
    } else if (isalpha(input[*pos]) || input[*pos] == '_') {
        token.type = TOK_IDENTIFIER;
        int i = 0;
        while (*pos < input_len && (isalnum(input[*pos]) || input[*pos] == '_') && i < 31) {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token.value[i] = '\0';
    } else if (is_operator_char(input[*pos])) {
        token.type = TOK_OPERATOR;
        int i = 0;
        while (*pos < input_len && is_operator_char(input[*pos]) && i < 31) {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token.value[i] = '\0';
    } else {
        token.value[0] = input[*pos];
        token.value[1] = '\0';
        (*pos)++;
        (*col)++;
    }
    
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "EOF", "INVALID"
    };
    printf("Line %d, Col %d: %s '%s'\n", 
           token.line, token.column, type_names[token.type], token.value);
}

void parse_expression(const char *input, int *pos, int line, int *col);

void parse_primary(const char *input, int *pos, int line, int *col) {
    struct Token token = get_next_token(input, pos, line, col);
    
    if (token.type == TOK_NUMBER) {
        printf("Found number: %s\n", token.value);
    } else if (token.type == TOK_IDENTIFIER) {
        printf("Found identifier: %s\n", token.value);
    } else if (token.type == TOK_OPERATOR && token.value[0] == '(') {
        printf("Entering parenthesized expression\n");
        parse_expression(input, pos, line, col);
        token = get_next_token(input, pos, line, col);
        if (!(token.type == TOK_OPERATOR && token.value[0] == ')')) {
            printf("Error: Expected closing parenthesis at line %d, col %d\n", token.line, token.column);
        } else {
            printf("Exiting parenthesized expression\n");
        }
    } else {
        printf("Error: Unexpected token '%s' at line %d, col %d\n", token.value, token.line, token.column);
    }
}

void parse_expression(const char *input, int *pos, int line, int *col) {
    parse_primary(input, pos, line, col);
    
    struct Token token = get_next_token(input, pos, line, col);
    
    if (token.type == TOK_OPERATOR && 
        (strcmp(token.value, "+") == 0 || strcmp(token.value, "-") == 0 || 
         strcmp(token.value, "*") == 0 || strcmp(token.value, "/") == 0)) {
        printf("Found operator: %s\n", token.value);
        parse_expression(input, pos, line, col);
    } else {
        if (*pos >= (int)strlen(token.value)) {
            *pos -= strlen(token.value);
        }
        if (*col >= (int)strlen(token.value)) {
            *col -= strlen(token.value);
        }
    }
}

int main() {
    char input[1024];
    
    printf("Enter expression to parse: ");
    if (!fgets(input, sizeof(input), stdin)) {
        printf