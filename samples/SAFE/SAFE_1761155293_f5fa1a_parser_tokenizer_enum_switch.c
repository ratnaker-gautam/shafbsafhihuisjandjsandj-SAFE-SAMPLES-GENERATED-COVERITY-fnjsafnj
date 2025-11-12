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

int get_token(struct Token *token, const char *input, int *pos, int line, int column) {
    int start = *pos;
    char c = input[*pos];
    
    if (c == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    if (isspace(c)) {
        while (isspace(input[*pos])) {
            (*pos)++;
        }
        return get_token(token, input, pos, line, column + (*pos - start));
    }
    
    if (isdigit(c)) {
        int len = 0;
        while (isdigit(input[*pos]) && len < 31) {
            token->value[len++] = input[(*pos)++];
        }
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        token->line = line;
        token->column = column;
        return 1;
    }
    
    if (isalpha(c) || c == '_') {
        int len = 0;
        while ((isalnum(input[*pos]) || input[*pos] == '_') && len < 31) {
            token->value[len++] = input[(*pos)++];
        }
        token->value[len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        token->line = line;
        token->column = column;
        return 1;
    }
    
    if (c == '"') {
        int len = 0;
        (*pos)++;
        while (input[*pos] != '"' && input[*pos] != '\0' && len < 31) {
            token->value[len++] = input[(*pos)++];
        }
        if (input[*pos] == '"') {
            (*pos)++;
        }
        token->value[len] = '\0';
        token->type = TOKEN_STRING;
        token->line = line;
        token->column = column;
        return 1;
    }
    
    if (is_operator_char(c)) {
        int len = 0;
        while (is_operator_char(input[*pos]) && len < 31) {
            token->value[len++] = input[(*pos)++];
        }
        token->value[len] = '\0';
        token->type = TOKEN_OPERATOR;
        token->line = line;
        token->column = column;
        return 1;
    }
    
    token->type = TOKEN_INVALID;
    token->value[0] = input[(*pos)++];
    token->value[1] = '\0';
    token->line = line;
    token->column = column;
    return 1;
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
    printf("Enter text to tokenize (max 1023 characters): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    printf("\nTokenizing: %s\n\n", input);
    
    int pos = 0;
    int line = 1;
    int column = 1;
    struct Token token;
    
    while (get_token(&token, input, &pos, line, column)) {
        print_token(&token);
        
        if (token.type == TOKEN_EOF) {
            break;
        }
        
        column += (int)strlen(token.value);
        
        if (token.type == TOKEN_INVALID && token.value[0] == '\n') {
            line++;
            column = 1;
        }
    }
    
    return 0;
}