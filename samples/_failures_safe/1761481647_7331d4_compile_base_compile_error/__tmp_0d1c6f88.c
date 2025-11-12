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

int parse_operator(const char *input, int pos, char *op) {
    int len = 0;
    char c = input[pos];
    
    if (is_operator_char(c)) {
        op[len++] = c;
        char next = input[pos + 1];
        
        if ((c == '=' && next == '=') || (c == '!' && next == '=') ||
            (c == '<' && next == '=') || (c == '>' && next == '=') ||
            (c == '&' && next == '&') || (c == '|' && next == '|')) {
            op[len++] = next;
        }
    }
    
    op[len] = '\0';
    return len > 0 ? len : 0;
}

struct Token get_next_token(const char *input, int *pos, int *line, int *column) {
    struct Token token = {TOKEN_ERROR, "", *line, *column};
    int start_pos = *pos;
    
    while (input[*pos] != '\0') {
        char c = input[*pos];
        
        if (isspace(c)) {
            if (c == '\n') {
                (*line)++;
                *column = 1;
            } else {
                (*column)++;
            }
            (*pos)++;
            continue;
        }
        
        if (isdigit(c)) {
            int len = 0;
            while (isdigit(input[*pos])) {
                if (len < 63) {
                    token.value[len++] = input[*pos];
                }
                (*pos)++;
                (*column)++;
            }
            token.value[len] = '\0';
            token.type = TOKEN_NUMBER;
            return token;
        }
        
        if (isalpha(c) || c == '_') {
            int len = 0;
            while (isalnum(input[*pos]) || input[*pos] == '_') {
                if (len < 63) {
                    token.value[len++] = input[*pos];
                }
                (*pos)++;
                (*column)++;
            }
            token.value[len] = '\0';
            token.type = TOKEN_IDENTIFIER;
            return token;
        }
        
        if (c == '"') {
            int len = 0;
            (*pos)++;
            (*column)++;
            
            while (input[*pos] != '\0' && input[*pos] != '"') {
                if (len < 63) {
                    token.value[len++] = input[*pos];
                }
                (*pos)++;
                (*column)++;
                
                if (input[*pos] == '\0') {
                    token.type = TOKEN_ERROR;
                    return token;
                }
            }
            
            if (input[*pos] == '"') {
                (*pos)++;
                (*column)++;
            }
            
            token.value[len] = '\0';
            token.type = TOKEN_STRING;
            return token;
        }
        
        if (is_operator_char(c)) {
            char op[3];
            int op_len = parse_operator(input, *pos, op);
            if (op_len > 0) {
                strncpy(token.value, op, 63);
                token.value[63] = '\0';
                token.type = TOKEN_OPERATOR;
                *pos += op_len;
                *column += op_len;
                return token;
            }
        }
        
        token.type = TOKEN_ERROR;
        strncpy(token.value, &input[*pos], 1);
        token.value[1] = '\0';
        (*pos)++;
        (*column)++;
        return token;
    }
    
    token.type = TOKEN_EOF;
    strcpy(token.value, "");
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    
    printf("Line %d, Column %d: %s", token.line, token.column, type_names[token.type]);
    
    if (token.type != TOKEN_EOF) {
        printf(" ('%s')", token.value);
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
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int pos = 0;
    int line = 1;
    int column = 1;
    int token_count = 0;
    
    printf("\nTokens:\n");
    
    while (1) {
        struct Token token =