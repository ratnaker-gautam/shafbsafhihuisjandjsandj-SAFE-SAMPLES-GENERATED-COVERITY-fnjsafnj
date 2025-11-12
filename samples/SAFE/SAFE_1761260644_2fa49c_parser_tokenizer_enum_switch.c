//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
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
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

struct Token get_next_token(const char *input, int *pos, int *line, int *column) {
    struct Token token = {TOKEN_INVALID, "", *line, *column};
    int value_pos = 0;
    
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
            token.type = TOKEN_NUMBER;
            token.line = *line;
            token.column = *column;
            
            while (isdigit(input[*pos]) && value_pos < 31) {
                token.value[value_pos++] = input[*pos];
                (*pos)++;
                (*column)++;
            }
            token.value[value_pos] = '\0';
            return token;
        }
        
        if (isalpha(c) || c == '_') {
            token.type = TOKEN_IDENTIFIER;
            token.line = *line;
            token.column = *column;
            
            while ((isalnum(input[*pos]) || input[*pos] == '_') && value_pos < 31) {
                token.value[value_pos++] = input[*pos];
                (*pos)++;
                (*column)++;
            }
            token.value[value_pos] = '\0';
            return token;
        }
        
        if (c == '"') {
            token.type = TOKEN_STRING;
            token.line = *line;
            token.column = *column;
            token.value[value_pos++] = c;
            (*pos)++;
            (*column)++;
            
            while (input[*pos] != '\0' && input[*pos] != '"' && value_pos < 31) {
                token.value[value_pos++] = input[*pos];
                (*pos)++;
                (*column)++;
            }
            
            if (input[*pos] == '"') {
                token.value[value_pos++] = input[*pos];
                (*pos)++;
                (*column)++;
            }
            token.value[value_pos] = '\0';
            return token;
        }
        
        if (is_operator_char(c)) {
            token.type = TOKEN_OPERATOR;
            token.line = *line;
            token.column = *column;
            token.value[value_pos++] = c;
            (*pos)++;
            (*column)++;
            
            if (is_operator_char(input[*pos]) && value_pos < 31) {
                token.value[value_pos++] = input[*pos];
                (*pos)++;
                (*column)++;
            }
            token.value[value_pos] = '\0';
            return token;
        }
        
        token.value[value_pos++] = c;
        token.value[value_pos] = '\0';
        (*pos)++;
        (*column)++;
        return token;
    }
    
    token.type = TOKEN_EOF;
    strcpy(token.value, "EOF");
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "INVALID"
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
    
    int pos = 0;
    int line = 1;
    int column = 1;
    
    printf("\nTokens:\n");
    printf("--------\n");
    
    while (1) {
        struct Token token = get_next_token(input, &pos, &line, &column);
        print_token(token);
        
        if (token.type == TOKEN_EOF || token.type == TOKEN_INVALID) {
            break;
        }
    }
    
    return 0;
}