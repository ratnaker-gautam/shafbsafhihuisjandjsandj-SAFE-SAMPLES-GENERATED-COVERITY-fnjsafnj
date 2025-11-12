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
    if (input == NULL || pos == NULL || line == NULL || col == NULL) return -1;
    
    int start_pos = *pos;
    int start_line = *line;
    int start_col = *col;
    
    while (input[*pos] != '\0' && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            (*line)++;
            *col = 1;
        } else {
            (*col)++;
        }
        (*pos)++;
    }
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = *line;
        token->column = *col;
        return 0;
    }
    
    start_pos = *pos;
    start_line = *line;
    start_col = *col;
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        int i = 0;
        while ((isalnum(input[*pos]) || input[*pos] == '_') && i < 63) {
            token->value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_IDENTIFIER;
        token->line = start_line;
        token->column = start_col;
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
        token->line = start_line;
        token->column = start_col;
        return 0;
    }
    
    if (input[*pos] == '"') {
        int i = 0;
        token->value[i++] = input[*pos];
        (*pos)++;
        (*col)++;
        
        while (input[*pos] != '\0' && input[*pos] != '"' && i < 63) {
            token->value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        
        if (input[*pos] == '"') {
            token->value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
            token->value[i] = '\0';
            token->type = TOKEN_STRING;
            token->line = start_line;
            token->column = start_col;
            return 0;
        } else {
            token->type = TOKEN_ERROR;
            token->value[0] = '\0';
            token->line = start_line;
            token->column = start_col;
            return -1;
        }
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
        token->line = start_line;
        token->column = start_col;
        return 0;
    }
    
    token->type = TOKEN_ERROR;
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    (*pos)++;
    (*col)++;
    token->line = start_line;
    token->column = start_col;
    return -1;
}

void print_token(const struct Token *token) {
    if (token == NULL) return;
    
    const char *type_names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    
    printf("Line %d, Column %d: %s", token->line, token->column, type_names[token->type]);
    if (token->type != TOKEN_EOF && token->type != TOKEN_ERROR) {
        printf(" '%s'", token->value);
    } else if (token->type == TOKEN_ERROR) {
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
        input[len-1