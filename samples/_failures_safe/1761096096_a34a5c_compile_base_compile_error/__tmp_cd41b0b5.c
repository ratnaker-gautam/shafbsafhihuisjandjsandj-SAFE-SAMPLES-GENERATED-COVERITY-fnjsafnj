//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
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
    TOKEN_UNKNOWN
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
    if (token == NULL || input == NULL || pos == NULL) {
        return 0;
    }
    
    int start = *pos;
    char current = input[*pos];
    
    if (current == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = line;
        token->column = column;
        return 1;
    }
    
    if (isspace(current)) {
        while (isspace(input[*pos])) {
            (*pos)++;
        }
        return get_token(token, input, pos, line, column + (*pos - start));
    }
    
    if (isdigit(current)) {
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
    
    if (isalpha(current) || current == '_') {
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
    
    if (current == '"') {
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
    
    if (is_operator_char(current)) {
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
    
    token->value[0] = current;
    token->value[1] = '\0';
    token->type = TOKEN_UNKNOWN;
    token->line = line;
    token->column = column;
    (*pos)++;
    return 1;
}

void print_token(const struct Token *token) {
    if (token == NULL) {
        return;
    }
    
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "UNKNOWN"
    };
    
    printf("Line %d, Column %d: %s", token->line, token->column, type_names[token->type]);
    
    if (token->type != TOKEN_EOF) {
        printf(" ('%s')", token->value);
    }
    printf("\n");
}

void process_tokens(const char *input) {
    if (input == NULL) {
        return;
    }
    
    int pos = 0;
    int line = 1;
    int column = 1;
    struct Token token;
    
    printf("Input: \"%s\"\n", input);
    printf("Tokens:\n");
    
    while (1) {
        int start_pos = pos;
        int start_column = column;
        
        if (!get_token(&token, input, &pos, line, start_column)) {
            break;
        }
        
        switch (token.type) {
            case TOKEN_NUMBER:
            case TOKEN_IDENTIFIER:
            case TOKEN_OPERATOR:
            case TOKEN_STRING:
            case TOKEN_UNKNOWN:
                print_token(&token);
                break;
            case TOKEN_EOF:
                print_token(&token);
                return;
            default:
                break;
        }
        
        for (int i = start_pos; i < pos && input[i] != '\0'; i++) {
            if (input[i] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
        }
        
        if (pos >= 1024) {
            break;
        }
    }
}

int main() {
    char input[1024];
    
    printf("Enter an expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf