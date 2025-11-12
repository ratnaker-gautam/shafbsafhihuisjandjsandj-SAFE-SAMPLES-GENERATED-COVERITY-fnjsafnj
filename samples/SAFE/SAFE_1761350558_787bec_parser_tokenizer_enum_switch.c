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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int get_token(struct Token *token, const char *input, int *pos, int *line, int *col) {
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
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
            token->type = TOKEN_EOF;
            token->value[0] = '\0';
            return 1;
        }
    }

    token->line = *line;
    token->column = *col;
    int value_pos = 0;

    if (isdigit(input[*pos])) {
        token->type = TOKEN_NUMBER;
        while (isdigit(input[*pos]) && value_pos < 31) {
            token->value[value_pos++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token->value[value_pos] = '\0';
    } else if (isalpha(input[*pos]) || input[*pos] == '_') {
        token->type = TOKEN_IDENTIFIER;
        while ((isalnum(input[*pos]) || input[*pos] == '_') && value_pos < 31) {
            token->value[value_pos++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token->value[value_pos] = '\0';
    } else if (is_operator_char(input[*pos])) {
        token->type = TOKEN_OPERATOR;
        while (is_operator_char(input[*pos]) && value_pos < 31) {
            token->value[value_pos++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token->value[value_pos] = '\0';
    } else if (input[*pos] == '"') {
        token->type = TOKEN_STRING;
        token->value[value_pos++] = input[*pos];
        (*pos)++;
        (*col)++;
        while (input[*pos] != '"' && input[*pos] != '\0' && value_pos < 31) {
            token->value[value_pos++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        if (input[*pos] == '"') {
            token->value[value_pos++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token->value[value_pos] = '\0';
    } else {
        token->type = TOKEN_INVALID;
        token->value[0] = input[*pos];
        token->value[1] = '\0';
        (*pos)++;
        (*col)++;
    }

    return 0;
}

void print_token(const struct Token *token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "INVALID"
    };
    
    printf("Line %d, Column %d: %s", token->line, token->column, type_names[token->type]);
    
    if (token->type != TOKEN_EOF) {
        printf(" ('%s')", token->value);
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
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }

    if (len == 0 || (len == 1 && input[0] == '\n')) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }

    int pos = 0;
    int line = 1;
    int column = 1;
    struct Token token;

    printf("Tokens:\n");
    while (1) {
        if (get_token(&token, input, &pos, &line, &column) != 0) {
            break;
        }
        
        print_token(&token);
        
        switch (token.type) {
            case TOKEN_EOF:
                return 0;
            case TOKEN_INVALID:
                fprintf(stderr, "Error: Invalid token at line %d, column %d\n", token.line, token.column);
                return 1;
            default:
                break;
        }
    }

    return 0;
}