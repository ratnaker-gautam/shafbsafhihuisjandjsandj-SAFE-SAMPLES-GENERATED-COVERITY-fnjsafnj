//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
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

int is_valid_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

struct Token get_next_token(const char *input, int *pos, int *line, int *col) {
    struct Token token = {TOKEN_UNKNOWN, "", *line, *col};
    int i = 0;
    
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
        token.type = TOKEN_EOF;
        return token;
    }
    
    token.line = *line;
    token.column = *col;
    
    if (is_valid_identifier_start(input[*pos])) {
        token.type = TOKEN_IDENTIFIER;
        while (input[*pos] != '\0' && is_valid_identifier_char(input[*pos]) && i < 63) {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token.value[i] = '\0';
    } else if (isdigit(input[*pos])) {
        token.type = TOKEN_NUMBER;
        while (input[*pos] != '\0' && (isdigit(input[*pos]) || input[*pos] == '.') && i < 63) {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token.value[i] = '\0';
    } else if (input[*pos] == '"') {
        token.type = TOKEN_STRING;
        (*pos)++;
        (*col)++;
        while (input[*pos] != '\0' && input[*pos] != '"' && i < 63) {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        if (input[*pos] == '"') {
            (*pos)++;
            (*col)++;
        }
        token.value[i] = '\0';
    } else if (is_operator_char(input[*pos])) {
        token.type = TOKEN_OPERATOR;
        token.value[i++] = input[*pos];
        (*pos)++;
        (*col)++;
        if (input[*pos] != '\0' && is_operator_char(input[*pos]) && i < 63) {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token.value[i] = '\0';
    } else {
        token.value[i++] = input[*pos];
        (*pos)++;
        (*col)++;
        token.value[i] = '\0';
    }
    
    return token;
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
    
    if (len == 1) {
        printf("Empty input\n");
        return 0;
    }
    
    int pos = 0;
    int line = 1;
    int col = 1;
    
    printf("\nTokens:\n");
    printf("%-12s %-15s %-6s %-6s\n", "TYPE", "VALUE", "LINE", "COL");
    printf("----------------------------------------\n");
    
    while (1) {
        struct Token token = get_next_token(input, &pos, &line, &col);
        
        if (token.type == TOKEN_EOF) {
            break;
        }
        
        printf("%-12s %-15s %-6d %-6d\n", 
               token_type_to_string(token.type),
               token.value,
               token.line,
               token.column);
        
        if (token.type == TOKEN_UNKNOWN) {
            printf("Warning: Unknown token encountered\n");
        }
    }
    
    return 0;
}