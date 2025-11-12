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

int is_valid_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

void init_token(struct Token *token, enum TokenType type, const char *value, int line, int column) {
    token->type = type;
    strncpy(token->value, value, sizeof(token->value) - 1);
    token->value[sizeof(token->value) - 1] = '\0';
    token->line = line;
    token->column = column;
}

struct Token get_next_token(const char **input, int *line, int *column) {
    struct Token token;
    const char *p = *input;
    
    while (*p && isspace(*p)) {
        if (*p == '\n') {
            (*line)++;
            *column = 1;
        } else {
            (*column)++;
        }
        p++;
    }
    
    if (*p == '\0') {
        init_token(&token, TOKEN_EOF, "", *line, *column);
        *input = p;
        return token;
    }
    
    int start_column = *column;
    
    if (is_valid_identifier_start(*p)) {
        char buffer[64] = {0};
        int i = 0;
        
        while (*p && is_valid_identifier_char(*p) && i < 63) {
            buffer[i++] = *p;
            p++;
            (*column)++;
        }
        
        buffer[i] = '\0';
        init_token(&token, TOKEN_IDENTIFIER, buffer, *line, start_column);
        *input = p;
        return token;
    }
    
    if (isdigit(*p)) {
        char buffer[64] = {0};
        int i = 0;
        int has_dot = 0;
        
        while (*p && (isdigit(*p) || (*p == '.' && !has_dot)) && i < 63) {
            if (*p == '.') has_dot = 1;
            buffer[i++] = *p;
            p++;
            (*column)++;
        }
        
        buffer[i] = '\0';
        init_token(&token, TOKEN_NUMBER, buffer, *line, start_column);
        *input = p;
        return token;
    }
    
    if (*p == '"') {
        char buffer[64] = {0};
        int i = 0;
        p++;
        (*column)++;
        
        while (*p && *p != '"' && i < 63) {
            buffer[i++] = *p;
            p++;
            (*column)++;
        }
        
        if (*p == '"') {
            p++;
            (*column)++;
        } else {
            init_token(&token, TOKEN_ERROR, "Unterminated string", *line, start_column);
            *input = p;
            return token;
        }
        
        buffer[i] = '\0';
        init_token(&token, TOKEN_STRING, buffer, *line, start_column);
        *input = p;
        return token;
    }
    
    if (is_operator_char(*p)) {
        char buffer[3] = {0};
        buffer[0] = *p;
        p++;
        (*column)++;
        
        if (is_operator_char(*p) && buffer[0] != '\0') {
            buffer[1] = *p;
            p++;
            (*column)++;
        }
        
        init_token(&token, TOKEN_OPERATOR, buffer, *line, start_column);
        *input = p;
        return token;
    }
    
    char error_buf[32];
    snprintf(error_buf, sizeof(error_buf), "Unexpected character '%c'", *p);
    init_token(&token, TOKEN_ERROR, error_buf, *line, start_column);
    p++;
    (*column)++;
    *input = p;
    return token;
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
    char input_buffer[1024];
    
    printf("Enter code to tokenize (max %zu characters):\n", sizeof(input_buffer) - 1);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1]