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

struct Token get_next_token(const char* input, int* pos, int* line, int* column) {
    struct Token token = {TOKEN_ERROR, "", *line, *column};
    int i = *pos;
    int len = strlen(input);
    
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
        strcpy(token.value, "");
        *pos = i;
        return token;
    }
    
    token.line = *line;
    token.column = *column;
    
    if (is_valid_identifier_start(input[i])) {
        token.type = TOKEN_IDENTIFIER;
        int j = 0;
        while (i < len && j < 63 && is_valid_identifier_char(input[i])) {
            token.value[j++] = input[i++];
            (*column)++;
        }
        token.value[j] = '\0';
    } else if (isdigit(input[i])) {
        token.type = TOKEN_NUMBER;
        int j = 0;
        while (i < len && j < 63 && (isdigit(input[i]) || input[i] == '.')) {
            token.value[j++] = input[i++];
            (*column)++;
        }
        token.value[j] = '\0';
    } else if (input[i] == '"') {
        token.type = TOKEN_STRING;
        int j = 0;
        i++;
        (*column)++;
        while (i < len && j < 63 && input[i] != '"') {
            if (input[i] == '\\' && i + 1 < len) {
                token.value[j++] = input[i++];
                (*column)++;
            }
            token.value[j++] = input[i++];
            (*column)++;
        }
        if (i < len && input[i] == '"') {
            i++;
            (*column)++;
        } else {
            token.type = TOKEN_ERROR;
        }
        token.value[j] = '\0';
    } else if (is_operator_char(input[i])) {
        token.type = TOKEN_OPERATOR;
        int j = 0;
        while (i < len && j < 63 && is_operator_char(input[i])) {
            token.value[j++] = input[i++];
            (*column)++;
        }
        token.value[j] = '\0';
    } else {
        token.type = TOKEN_ERROR;
        token.value[0] = input[i];
        token.value[1] = '\0';
        i++;
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
        case TOKEN_ERROR: return "ERROR";
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
    
    printf("\nTokens:\n");
    printf("%-12s %-15s %-6s %-6s\n", "Type", "Value", "Line", "Column");
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