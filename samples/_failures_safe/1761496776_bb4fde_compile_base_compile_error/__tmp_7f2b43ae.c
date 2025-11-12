//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
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
    TOKEN_ERROR
};

struct Token {
    enum TokenType type;
    char value[64];
    int line;
    int column;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_operator(const char *input, int pos, char *buffer) {
    int len = 0;
    char c = input[pos];
    
    if (is_operator_char(c)) {
        buffer[len++] = c;
        char next = input[pos + 1];
        
        if ((c == '=' && next == '=') ||
            (c == '!' && next == '=') ||
            (c == '<' && next == '=') ||
            (c == '>' && next == '=') ||
            (c == '&' && next == '&') ||
            (c == '|' && next == '|')) {
            buffer[len++] = next;
        }
    }
    
    buffer[len] = '\0';
    return len > 0 ? len : 0;
}

struct Token get_next_token(const char *input, int *pos, int *line, int *column) {
    struct Token token = {TOKEN_ERROR, "", *line, *column};
    int start_pos = *pos;
    
    while (input[*pos] != '\0' && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            (*line)++;
            *column = 1;
        } else {
            (*column)++;
        }
        (*pos)++;
        start_pos = *pos;
    }
    
    if (input[*pos] == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        return token;
    }
    
    token.line = *line;
    token.column = *column;
    
    if (isdigit(input[*pos])) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (input[*pos] != '\0' && (isdigit(input[*pos]) || input[*pos] == '.')) {
            if (i < 63) {
                token.value[i++] = input[*pos];
            }
            (*pos)++;
            (*column)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (input[*pos] != '\0' && (isalnum(input[*pos]) || input[*pos] == '_')) {
            if (i < 63) {
                token.value[i++] = input[*pos];
            }
            (*pos)++;
            (*column)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (input[*pos] == '"') {
        token.type = TOKEN_STRING;
        int i = 0;
        (*pos)++;
        (*column)++;
        
        while (input[*pos] != '\0' && input[*pos] != '"') {
            if (i < 63) {
                token.value[i++] = input[*pos];
            }
            (*pos)++;
            (*column)++;
        }
        
        if (input[*pos] == '"') {
            (*pos)++;
            (*column)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    int op_len = parse_operator(input, *pos, token.value);
    if (op_len > 0) {
        token.type = TOKEN_OPERATOR;
        *pos += op_len;
        *column += op_len;
        return token;
    }
    
    token.value[0] = input[*pos];
    token.value[1] = '\0';
    (*pos)++;
    (*column)++;
    return token;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
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
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("Empty input\n");
        return 1;
    }
    
    int pos = 0;
    int line = 1;
    int column = 1;
    
    printf("\nTokens:\n");
    printf("%-12s %-15s %-6s %-6s\n", "TYPE", "VALUE", "LINE", "COL