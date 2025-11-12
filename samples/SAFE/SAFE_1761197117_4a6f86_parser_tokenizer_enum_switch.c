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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int get_token(struct Token *token, const char *input, int *pos, int line, int column) {
    int start = *pos;
    char current = input[*pos];
    
    if (current == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    while (isspace((unsigned char)current)) {
        if (current == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
        (*pos)++;
        current = input[*pos];
    }
    
    token->line = line;
    token->column = column;
    
    if (isdigit((unsigned char)current)) {
        int i = 0;
        while (isdigit((unsigned char)input[*pos]) && i < 31) {
            token->value[i++] = input[(*pos)++];
        }
        token->value[i] = '\0';
        token->type = TOKEN_NUMBER;
        return 1;
    }
    
    if (isalpha((unsigned char)current) || current == '_') {
        int i = 0;
        while ((isalnum((unsigned char)input[*pos]) || input[*pos] == '_') && i < 31) {
            token->value[i++] = input[(*pos)++];
        }
        token->value[i] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    
    if (is_operator_char(current)) {
        int i = 0;
        while (is_operator_char(input[*pos]) && i < 31) {
            token->value[i++] = input[(*pos)++];
        }
        token->value[i] = '\0';
        token->type = TOKEN_OPERATOR;
        return 1;
    }
    
    if (current == '"') {
        int i = 0;
        (*pos)++;
        while (input[*pos] != '"' && input[*pos] != '\0' && i < 31) {
            token->value[i++] = input[(*pos)++];
        }
        if (input[*pos] == '"') {
            (*pos)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_STRING;
        return 1;
    }
    
    token->type = TOKEN_INVALID;
    token->value[0] = current;
    token->value[1] = '\0';
    (*pos)++;
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
    printf("Enter text to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    printf("\nTokens:\n");
    printf("--------\n");
    
    int pos = 0;
    int line = 1;
    int column = 1;
    
    while (1) {
        struct Token token;
        if (!get_token(&token, input, &pos, line, column)) {
            fprintf(stderr, "Error tokenizing input\n");
            return 1;
        }
        
        print_token(&token);
        
        line = token.line;
        column = token.column + (int)strlen(token.value);
        
        if (token.type == TOKEN_EOF || token.type == TOKEN_INVALID) {
            break;
        }
    }
    
    return 0;
}