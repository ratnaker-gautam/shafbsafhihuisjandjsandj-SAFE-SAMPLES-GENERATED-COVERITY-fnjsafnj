//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_STRING, TOKEN_EOF, TOKEN_UNKNOWN };

struct Token {
    enum TokenType type;
    char value[64];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int get_token(struct Token *token, const char *input, int *pos, int *line, int *column) {
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = *line;
        token->column = *column;
        return 1;
    }
    
    while (isspace((unsigned char)input[*pos])) {
        if (input[*pos] == '\n') {
            (*line)++;
            *column = 1;
        } else {
            (*column)++;
        }
        (*pos)++;
        if (input[*pos] == '\0') {
            token->type = TOKEN_EOF;
            token->value[0] = '\0';
            token->line = *line;
            token->column = *column;
            return 1;
        }
    }
    
    token->line = *line;
    token->column = *column;
    
    if (isalpha((unsigned char)input[*pos]) || input[*pos] == '_') {
        token->type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum((unsigned char)input[*pos]) || input[*pos] == '_') && i < 63) {
            token->value[i++] = input[*pos];
            (*pos)++;
            (*column)++;
        }
        token->value[i] = '\0';
        return 1;
    }
    
    if (isdigit((unsigned char)input[*pos])) {
        token->type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit((unsigned char)input[*pos]) && i < 63) {
            token->value[i++] = input[*pos];
            (*pos)++;
            (*column)++;
        }
        token->value[i] = '\0';
        return 1;
    }
    
    if (input[*pos] == '"') {
        token->type = TOKEN_STRING;
        int i = 0;
        token->value[i++] = input[*pos];
        (*pos)++;
        (*column)++;
        while (input[*pos] != '"' && input[*pos] != '\0' && i < 63) {
            token->value[i++] = input[*pos];
            (*pos)++;
            (*column)++;
        }
        if (input[*pos] == '"') {
            token->value[i++] = input[*pos];
            (*pos)++;
            (*column)++;
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
            (*column)++;
        }
        token->value[i] = '\0';
        return 1;
    }
    
    token->type = TOKEN_UNKNOWN;
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    (*pos)++;
    (*column)++;
    return 1;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input_buffer[1024];
    printf("Enter code to tokenize (max 1023 characters):\n");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input_buffer[0] == '\n')) {
        printf("No input provided\n");
        return 1;
    }
    
    int pos = 0;
    int line = 1;
    int column = 1;
    struct Token token;
    
    printf("\nTokens:\n");
    printf("Line:Column  Type         Value\n");
    printf("-----------  -----------  -----\n");
    
    while (get_token(&token, input_buffer, &pos, &line, &column)) {
        printf("%3d:%-7d  %-11s  %s\n", 
               token.line, token.column,