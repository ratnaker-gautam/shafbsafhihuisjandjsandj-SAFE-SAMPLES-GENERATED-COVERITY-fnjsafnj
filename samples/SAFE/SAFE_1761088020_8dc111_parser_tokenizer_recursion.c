//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

struct Token get_next_token(const char **input, int *line, int *column) {
    struct Token token = {TOKEN_INVALID, "", *line, *column};
    
    if (**input == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        return token;
    }
    
    while (isspace((unsigned char)**input)) {
        if (**input == '\n') {
            (*line)++;
            *column = 1;
        } else {
            (*column)++;
        }
        (*input)++;
    }
    
    if (**input == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        return token;
    }
    
    token.line = *line;
    token.column = *column;
    
    if (isdigit((unsigned char)**input)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit((unsigned char)**input) && i < 31) {
            token.value[i++] = **input;
            (*input)++;
            (*column)++;
        }
        token.value[i] = '\0';
    } else if (isalpha((unsigned char)**input) || **input == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum((unsigned char)**input) || **input == '_') && i < 31) {
            token.value[i++] = **input;
            (*input)++;
            (*column)++;
        }
        token.value[i] = '\0';
    } else if (is_operator_char(**input)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (is_operator_char(**input) && i < 31) {
            token.value[i++] = **input;
            (*input)++;
            (*column)++;
        }
        token.value[i] = '\0';
    } else {
        token.value[0] = **input;
        token.value[1] = '\0';
        (*input)++;
        (*column)++;
    }
    
    return token;
}

void print_token(const struct Token *token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "EOF", "INVALID"
    };
    printf("Line %d, Column %d: %s '%s'\n", 
           token->line, token->column, type_names[token->type], token->value);
}

void parse_expression(const char **input, int *line, int *column, int depth) {
    if (depth > 100) {
        printf("Error: Expression too complex\n");
        return;
    }
    
    struct Token token = get_next_token(input, line, column);
    
    if (token.type == TOKEN_EOF) {
        return;
    }
    
    if (token.type == TOKEN_INVALID) {
        printf("Error: Invalid token ");
        print_token(&token);
        return;
    }
    
    print_token(&token);
    
    if (token.type == TOKEN_OPERATOR) {
        parse_expression(input, line, column, depth + 1);
        parse_expression(input, line, column, depth + 1);
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to parse: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    const char *input_ptr = input;
    int line = 1;
    int column = 1;
    
    printf("Parsing expression: %s\n", input);
    printf("Token stream:\n");
    
    parse_expression(&input_ptr, &line, &column, 0);
    
    struct Token final_token = get_next_token(&input_ptr, &line, &column);
    if (final_token.type != TOKEN_EOF) {
        printf("Unexpected token after expression: ");
        print_token(&final_token);
    } else {
        printf("Parsing completed successfully\n");
    }
    
    return 0;
}