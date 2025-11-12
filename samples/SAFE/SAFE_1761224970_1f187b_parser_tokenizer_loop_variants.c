//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
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
    char value[32];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

struct Token get_next_token(const char *input, int *pos, int *line, int *col) {
    struct Token token = {TOKEN_ERROR, "", *line, *col};
    int i = 0;
    
    while (input[*pos] != '\0') {
        char c = input[*pos];
        
        if (isspace(c)) {
            if (c == '\n') {
                (*line)++;
                *col = 1;
            } else {
                (*col)++;
            }
            (*pos)++;
            continue;
        }
        
        token.line = *line;
        token.column = *col;
        
        if (isdigit(c)) {
            token.type = TOKEN_NUMBER;
            while (isdigit(input[*pos]) && i < 31) {
                token.value[i++] = input[*pos];
                (*pos)++;
                (*col)++;
            }
            token.value[i] = '\0';
            return token;
        }
        
        if (isalpha(c) || c == '_') {
            token.type = TOKEN_IDENTIFIER;
            while ((isalnum(input[*pos]) || input[*pos] == '_') && i < 31) {
                token.value[i++] = input[*pos];
                (*pos)++;
                (*col)++;
            }
            token.value[i] = '\0';
            return token;
        }
        
        if (is_operator_char(c)) {
            token.type = TOKEN_OPERATOR;
            token.value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
            if (is_operator_char(input[*pos]) && i < 31) {
                token.value[i++] = input[*pos];
                (*pos)++;
                (*col)++;
            }
            token.value[i] = '\0';
            return token;
        }
        
        if (c == '"') {
            token.type = TOKEN_STRING;
            token.value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
            
            while (input[*pos] != '\0' && input[*pos] != '"' && i < 31) {
                token.value[i++] = input[*pos];
                (*pos)++;
                (*col)++;
            }
            
            if (input[*pos] == '"') {
                token.value[i++] = input[*pos];
                (*pos)++;
                (*col)++;
                token.value[i] = '\0';
                return token;
            } else {
                token.type = TOKEN_ERROR;
                strcpy(token.value, "Unterminated string");
                return token;
            }
        }
        
        token.type = TOKEN_ERROR;
        snprintf(token.value, sizeof(token.value), "Unexpected character: %c", c);
        (*pos)++;
        (*col)++;
        return token;
    }
    
    token.type = TOKEN_EOF;
    strcpy(token.value, "EOF");
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    printf("Line %d, Col %d: %-12s '%s'\n", 
           token.line, token.column, type_names[token.type], token.value);
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (len == 1) {
        printf("No input provided\n");
        return 1;
    }
    
    int pos = 0;
    int line = 1;
    int col = 1;
    int token_count = 0;
    
    printf("\nTokens:\n");
    
    for (;;) {
        struct Token token = get_next_token(input, &pos, &line, &col);
        print_token(token);
        token_count++;
        
        if (token.type == TOKEN_EOF || token.type == TOKEN_ERROR) {
            break;
        }
        
        if (token_count > 100) {
            printf("Too many tokens, stopping\n");
            break;
        }
    }
    
    printf("Total tokens processed: %d\n", token_count);
    return 0;
}