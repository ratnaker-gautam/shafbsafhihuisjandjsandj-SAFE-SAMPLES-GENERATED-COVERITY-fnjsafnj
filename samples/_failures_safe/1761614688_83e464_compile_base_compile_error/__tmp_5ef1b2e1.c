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

int get_next_token(const char *input, int *pos, int *line, int *column, struct Token *token) {
    int start_pos = *pos;
    int start_line = *line;
    int start_column = *column;
    
    while (input[*pos] != '\0' && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            (*line)++;
            *column = 1;
        } else {
            (*column)++;
        }
        (*pos)++;
    }
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = *line;
        token->column = *column;
        return 1;
    }
    
    start_pos = *pos;
    start_line = *line;
    start_column = *column;
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        int i = 0;
        while ((isalnum(input[*pos]) || input[*pos] == '_') && i < 63) {
            token->value[i++] = input[*pos];
            (*pos)++;
            (*column)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_IDENTIFIER;
        token->line = start_line;
        token->column = start_column;
        return 1;
    }
    
    if (isdigit(input[*pos])) {
        int i = 0;
        while (isdigit(input[*pos]) && i < 63) {
            token->value[i++] = input[*pos];
            (*pos)++;
            (*column)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_NUMBER;
        token->line = start_line;
        token->column = start_column;
        return 1;
    }
    
    if (input[*pos] == '"') {
        int i = 0;
        (*pos)++;
        (*column)++;
        while (input[*pos] != '\0' && input[*pos] != '"' && i < 63) {
            token->value[i++] = input[*pos];
            (*pos)++;
            (*column)++;
        }
        if (input[*pos] == '"') {
            (*pos)++;
            (*column)++;
        } else {
            token->type = TOKEN_ERROR;
            strcpy(token->value, "Unterminated string");
            token->line = start_line;
            token->column = start_column;
            return 0;
        }
        token->value[i] = '\0';
        token->type = TOKEN_STRING;
        token->line = start_line;
        token->column = start_column;
        return 1;
    }
    
    if (is_operator_char(input[*pos])) {
        int i = 0;
        while (is_operator_char(input[*pos]) && i < 63) {
            token->value[i++] = input[*pos];
            (*pos)++;
            (*column)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_OPERATOR;
        token->line = start_line;
        token->column = start_column;
        return 1;
    }
    
    token->type = TOKEN_ERROR;
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    token->line = start_line;
    token->column = start_column;
    (*pos)++;
    (*column)++;
    return 0;
}

const char* token_type_to_string(enum TokenType type) {
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
    int column = 1;
    struct Token token;
    
    printf("\nToken stream:\n");
    printf("%-12s %-15s %-6s %-6s\n", "Type", "Value", "Line", "Column");
    printf("------------------------------------------------\n");
    
    while (1) {
        if (!get_next