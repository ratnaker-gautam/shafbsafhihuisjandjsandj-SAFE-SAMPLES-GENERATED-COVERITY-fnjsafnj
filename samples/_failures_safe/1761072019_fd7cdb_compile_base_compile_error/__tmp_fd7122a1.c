//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
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

struct Token get_next_token(const char *input, int *pos, int *line, int *col) {
    struct Token token = {TOKEN_ERROR, "", *line, *col};
    int value_pos = 0;
    
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
        
        if (isalpha(c) || c == '_') {
            token.type = TOKEN_IDENTIFIER;
            token.line = *line;
            token.column = *col;
            
            while ((isalnum(input[*pos]) || input[*pos] == '_') && value_pos < 63) {
                token.value[value_pos++] = input[*pos];
                (*pos)++;
                (*col)++;
            }
            token.value[value_pos] = '\0';
            return token;
        }
        
        if (isdigit(c)) {
            token.type = TOKEN_NUMBER;
            token.line = *line;
            token.column = *col;
            
            while (isdigit(input[*pos]) && value_pos < 63) {
                token.value[value_pos++] = input[*pos];
                (*pos)++;
                (*col)++;
            }
            token.value[value_pos] = '\0';
            return token;
        }
        
        if (c == '"') {
            token.type = TOKEN_STRING;
            token.line = *line;
            token.column = *col;
            
            (*pos)++;
            (*col)++;
            
            while (input[*pos] != '"' && input[*pos] != '\0' && value_pos < 63) {
                token.value[value_pos++] = input[*pos];
                (*pos)++;
                (*col)++;
            }
            
            if (input[*pos] == '"') {
                (*pos)++;
                (*col)++;
            } else {
                token.type = TOKEN_ERROR;
            }
            token.value[value_pos] = '\0';
            return token;
        }
        
        if (is_operator_char(c)) {
            token.type = TOKEN_OPERATOR;
            token.line = *line;
            token.column = *col;
            
            while (is_operator_char(input[*pos]) && value_pos < 63) {
                token.value[value_pos++] = input[*pos];
                (*pos)++;
                (*col)++;
            }
            token.value[value_pos] = '\0';
            return token;
        }
        
        token.type = TOKEN_ERROR;
        token.value[0] = c;
        token.value[1] = '\0';
        (*pos)++;
        (*col)++;
        return token;
    }
    
    token.type = TOKEN_EOF;
    token.value[0] = '\0';
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
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("Empty input\n");
        return 1;
    }
    
    int pos = 0;
    int line = 1;
    int col = 1;
    int token_count = 0;
    
    printf("\nToken stream:\n");
    printf("Line:Col Type        Value\n");
    printf("-------- ----------- --------------------\n");
    
    while (1) {
        struct Token token = get_next_token(input, &pos, &line, &col);
        
        printf("%4d:%-3d %-11s %s\n", 
               token.line, token.column, 
               token_type_to_string(token.type),
               token.value);
        
        token_count++;
        
        if (token.type == TOKEN_EOF || token.type == TOKEN_ERROR) {
            break;
        }
        
        if (token_count > 100) {
            printf("Too many tokens, stopping\n");
            break;
        }
    }
    
    printf("Total tokens