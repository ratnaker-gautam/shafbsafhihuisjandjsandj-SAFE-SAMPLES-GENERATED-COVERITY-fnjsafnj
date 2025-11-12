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
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        token->type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum(input[*pos]) || input[*pos] == '_') && i < 63) {
            token->value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token->value[i] = '\0';
        return 1;
    }
    
    if (isdigit(input[*pos])) {
        token->type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit(input[*pos]) && i < 63) {
            token->value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token->value[i] = '\0';
        return 1;
    }
    
    if (input[*pos] == '"') {
        token->type = TOKEN_STRING;
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
        }
        token->value[i] = '\0';
        return 1;
    }
    
    if (is_operator_char(input[*pos])) {
        token->type = TOKEN_OPERATOR;
        int i = 0;
        while (is_operator_char(input[*pos]) && i < 63) {
            token->value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token->value[i] = '\0';
        return 1;
    }
    
    token->type = TOKEN_ERROR;
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    (*pos)++;
    (*col)++;
    return 1;
}

const char* token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    int pos = 0;
    int line = 1;
    int col = 1;
    struct Token token;
    
    printf("\nToken stream:\n");
    printf("%-12s %-15s %-6s %-6s\n", "Type", "Value", "Line", "Column");
    printf("------------------------------------------------\n");
    
    while (1) {
        if (!get_token(&token, input, &pos, &line, &col)) {
            fprintf(stderr, "Error getting token\n");
            return 1;
        }
        
        printf("%-12s %-15s %-6d %-6d\n", 
               token_type_name(token.type), 
               token.value, 
               token.line, 
               token.column);
        
        if (token.type == TOKEN_EOF || token.type == TOKEN_ERROR) {
            break;
        }
    }
    
    return 0;
}