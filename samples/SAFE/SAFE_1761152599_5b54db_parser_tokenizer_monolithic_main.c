//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_STRING, TOKEN_EOF, TOKEN_ERROR };

struct Token {
    enum TokenType type;
    char value[64];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int get_token(struct Token *token, const char *input, int *pos, int *line, int *col) {
    if (input == NULL || token == NULL || pos == NULL || line == NULL || col == NULL) return -1;
    
    while (input[*pos] != '\0') {
        if (isspace(input[*pos])) {
            if (input[*pos] == '\n') {
                (*line)++;
                *col = 1;
            } else {
                (*col)++;
            }
            (*pos)++;
            continue;
        }
        
        token->line = *line;
        token->column = *col;
        
        if (isalpha(input[*pos]) || input[*pos] == '_') {
            int i = 0;
            while ((isalnum(input[*pos]) || input[*pos] == '_') && i < 63) {
                token->value[i++] = input[*pos];
                (*pos)++;
                (*col)++;
            }
            token->value[i] = '\0';
            token->type = TOKEN_IDENTIFIER;
            return 0;
        }
        
        if (isdigit(input[*pos])) {
            int i = 0;
            while (isdigit(input[*pos]) && i < 63) {
                token->value[i++] = input[*pos];
                (*pos)++;
                (*col)++;
            }
            token->value[i] = '\0';
            token->type = TOKEN_NUMBER;
            return 0;
        }
        
        if (input[*pos] == '"') {
            int i = 0;
            (*pos)++;
            (*col)++;
            while (input[*pos] != '"' && input[*pos] != '\0' && i < 63) {
                token->value[i++] = input[*pos];
                (*pos)++;
                (*col)++;
            }
            if (input[*pos] == '"') {
                (*pos)++;
                (*col)++;
            } else {
                token->type = TOKEN_ERROR;
                return -1;
            }
            token->value[i] = '\0';
            token->type = TOKEN_STRING;
            return 0;
        }
        
        if (is_operator_char(input[*pos])) {
            int i = 0;
            while (is_operator_char(input[*pos]) && i < 63) {
                token->value[i++] = input[*pos];
                (*pos)++;
                (*col)++;
            }
            token->value[i] = '\0';
            token->type = TOKEN_OPERATOR;
            return 0;
        }
        
        token->type = TOKEN_ERROR;
        return -1;
    }
    
    token->type = TOKEN_EOF;
    token->value[0] = '\0';
    return 0;
}

void print_token(const struct Token *token) {
    if (token == NULL) return;
    
    const char *type_names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    
    printf("Line %d, Col %d: %s", token->line, token->column, type_names[token->type]);
    if (token->type != TOKEN_EOF && token->type != TOKEN_ERROR) {
        printf(" '%s'", token->value);
    }
    printf("\n");
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize (max 1023 chars): ");
    
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
    int col = 1;
    
    printf("Tokens:\n");
    
    while (1) {
        struct Token token;
        if (get_token(&token, input, &pos, &line, &col) != 0) {
            fprintf(stderr, "Error tokenizing at position %d\n", pos);
            return 1;
        }
        
        print_token(&token);
        
        if (token.type == TOKEN_EOF || token.type == TOKEN_ERROR) {
            break;
        }
    }
    
    return 0;
}