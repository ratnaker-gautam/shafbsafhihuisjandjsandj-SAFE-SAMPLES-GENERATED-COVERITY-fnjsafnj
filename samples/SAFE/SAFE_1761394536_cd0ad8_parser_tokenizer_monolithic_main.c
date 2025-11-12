//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_STRING, TOKEN_SYMBOL, TOKEN_EOF };

struct Token {
    enum TokenType type;
    char value[64];
    int line;
    int column;
};

int is_symbol_char(char c) {
    return isalnum(c) || c == '_' || c == '-';
}

int parse_token(const char *input, int *pos, int line, int col, struct Token *token) {
    int start = *pos;
    int length = 0;
    
    while (input[*pos] == ' ' || input[*pos] == '\t') {
        (*pos)++;
        col++;
    }
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = line;
        token->column = col;
        return 1;
    }
    
    if (input[*pos] == '\n') {
        (*pos)++;
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = line;
        token->column = col;
        return 1;
    }
    
    if (isdigit(input[*pos])) {
        token->type = TOKEN_NUMBER;
        while (isdigit(input[*pos]) && length < 63) {
            token->value[length++] = input[*pos];
            (*pos)++;
            col++;
        }
        token->value[length] = '\0';
        token->line = line;
        token->column = col - length;
        return 1;
    }
    
    if (input[*pos] == '"') {
        token->type = TOKEN_STRING;
        (*pos)++;
        col++;
        while (input[*pos] != '"' && input[*pos] != '\0' && input[*pos] != '\n' && length < 63) {
            token->value[length++] = input[*pos];
            (*pos)++;
            col++;
        }
        if (input[*pos] == '"') {
            (*pos)++;
            col++;
        }
        token->value[length] = '\0';
        token->line = line;
        token->column = col - length - 1;
        return 1;
    }
    
    if (is_symbol_char(input[*pos])) {
        token->type = TOKEN_SYMBOL;
        while (is_symbol_char(input[*pos]) && length < 63) {
            token->value[length++] = input[*pos];
            (*pos)++;
            col++;
        }
        token->value[length] = '\0';
        token->line = line;
        token->column = col - length;
        return 1;
    }
    
    token->type = TOKEN_SYMBOL;
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    (*pos)++;
    token->line = line;
    token->column = col;
    return 1;
}

void print_token(const struct Token *token) {
    const char *type_names[] = {"NUMBER", "STRING", "SYMBOL", "EOF"};
    printf("Line %d, Column %d: %s", token->line, token->column, type_names[token->type]);
    if (token->type != TOKEN_EOF) {
        printf(" '%s'", token->value);
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
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    int pos = 0;
    int line = 1;
    int column = 1;
    struct Token token;
    
    printf("\nTokens:\n");
    printf("--------\n");
    
    while (parse_token(input, &pos, line, column, &token)) {
        print_token(&token);
        
        if (token.type == TOKEN_EOF) {
            break;
        }
        
        column = token.column + (int)strlen(token.value);
        if (token.type == TOKEN_STRING) {
            column += 2;
        }
    }
    
    return 0;
}