//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
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
    int i = *pos;
    int len = strlen(input);
    
    if (i >= len) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    while (i < len && isspace(input[i])) {
        if (input[i] == '\n') {
            (*line)++;
            *column = 1;
        } else {
            (*column)++;
        }
        i++;
    }
    
    if (i >= len) {
        token.type = TOKEN_EOF;
        *pos = i;
        return token;
    }
    
    token.line = *line;
    token.column = *column;
    
    if (isalpha(input[i]) || input[i] == '_') {
        token.type = TOKEN_IDENTIFIER;
        int j = 0;
        while (i < len && j < 31 && (isalnum(input[i]) || input[i] == '_')) {
            token.value[j++] = input[i++];
            (*column)++;
        }
        token.value[j] = '\0';
    } else if (isdigit(input[i])) {
        token.type = TOKEN_NUMBER;
        int j = 0;
        while (i < len && j < 31 && isdigit(input[i])) {
            token.value[j++] = input[i++];
            (*column)++;
        }
        token.value[j] = '\0';
    } else if (input[i] == '"') {
        token.type = TOKEN_STRING;
        int j = 0;
        i++;
        (*column)++;
        while (i < len && j < 31 && input[i] != '"') {
            token.value[j++] = input[i++];
            (*column)++;
        }
        token.value[j] = '\0';
        if (i < len && input[i] == '"') {
            i++;
            (*column)++;
        }
    } else if (is_operator_char(input[i])) {
        token.type = TOKEN_OPERATOR;
        int j = 0;
        while (i < len && j < 31 && is_operator_char(input[i])) {
            token.value[j++] = input[i++];
            (*column)++;
        }
        token.value[j] = '\0';
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = input[i++];
        token.value[1] = '\0';
        (*column)++;
    }
    
    *pos = i;
    return token;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_INVALID: return "INVALID";
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
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int pos = 0;
    int line = 1;
    int column = 1;
    
    printf("\nToken stream:\n");
    printf("%-12s %-15s %-6s %-6s\n", "Type", "Value", "Line", "Column");
    printf("----------------------------------------\n");
    
    while (1) {
        struct Token token = get_next_token(input, &pos, &line, &column);
        
        switch (token.type) {
            case TOKEN_IDENTIFIER:
            case TOKEN_NUMBER:
            case TOKEN_OPERATOR:
            case TOKEN_STRING:
            case TOKEN_INVALID:
                printf("%-12s %-15s %-6d %-6d\n", 
                       token_type_to_string(token.type), 
                       token.value, 
                       token.line, 
                       token.column);
                break;
                
            case TOKEN_EOF:
                printf("%-12s %-15s %-6d %-6d\n", 
                       token_type_to_string(token.type), 
                       "", 
                       token.line, 
                       token.column);
                return 0;
                
            default:
                fprintf(stderr, "Unexpected token type\n");
                return 1;
        }
    }
    
    return 0;
}