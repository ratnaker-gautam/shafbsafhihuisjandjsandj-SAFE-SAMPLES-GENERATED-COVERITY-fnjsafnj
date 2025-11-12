//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_STRING, TOKEN_EOF, TOKEN_ERROR };

struct Token {
    enum TokenType type;
    char value[256];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

struct Token get_next_token(const char* input, int* pos, int* line, int* col) {
    struct Token token = {TOKEN_ERROR, "", *line, *col};
    int i = *pos;
    int len = strlen(input);
    
    while (i < len && isspace(input[i])) {
        if (input[i] == '\n') {
            (*line)++;
            *col = 1;
        } else {
            (*col)++;
        }
        i++;
    }
    
    if (i >= len) {
        token.type = TOKEN_EOF;
        *pos = i;
        return token;
    }
    
    token.line = *line;
    token.column = *col;
    
    if (isalpha(input[i]) || input[i] == '_') {
        int j = 0;
        while ((i < len) && (isalnum(input[i]) || input[i] == '_') && j < 255) {
            token.value[j++] = input[i++];
            (*col)++;
        }
        token.value[j] = '\0';
        token.type = TOKEN_IDENTIFIER;
    } else if (isdigit(input[i])) {
        int j = 0;
        while ((i < len) && (isdigit(input[i]) || input[i] == '.') && j < 255) {
            token.value[j++] = input[i++];
            (*col)++;
        }
        token.value[j] = '\0';
        token.type = TOKEN_NUMBER;
    } else if (input[i] == '"') {
        int j = 0;
        token.value[j++] = input[i++];
        (*col)++;
        while ((i < len) && (input[i] != '"') && j < 254) {
            token.value[j++] = input[i++];
            (*col)++;
        }
        if (i < len && input[i] == '"') {
            token.value[j++] = input[i++];
            (*col)++;
            token.value[j] = '\0';
            token.type = TOKEN_STRING;
        } else {
            token.type = TOKEN_ERROR;
        }
    } else if (is_operator_char(input[i])) {
        int j = 0;
        while ((i < len) && is_operator_char(input[i]) && j < 255) {
            token.value[j++] = input[i++];
            (*col)++;
        }
        token.value[j] = '\0';
        token.type = TOKEN_OPERATOR;
    } else {
        token.value[0] = input[i];
        token.value[1] = '\0';
        token.type = TOKEN_ERROR;
        i++;
        (*col)++;
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
        case TOKEN_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[1024];
    printf("Enter code to tokenize (max 1023 characters):\n");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int pos = 0;
    int line = 1;
    int column = 1;
    
    printf("\nTokens:\n");
    printf("%-12s %-15s %-6s %-6s\n", "TYPE", "VALUE", "LINE", "COL");
    printf("----------------------------------------\n");
    
    while (1) {
        struct Token token = get_next_token(input, &pos, &line, &column);
        
        printf("%-12s %-15s %-6d %-6d\n", 
               token_type_to_string(token.type), 
               token.value, 
               token.line, 
               token.column);
        
        if (token.type == TOKEN_EOF || token.type == TOKEN_ERROR) {
            break;
        }
    }
    
    return 0;
}