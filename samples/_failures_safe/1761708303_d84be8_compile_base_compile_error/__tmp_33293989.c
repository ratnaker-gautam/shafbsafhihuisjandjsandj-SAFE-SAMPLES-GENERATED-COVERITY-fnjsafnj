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
        strcpy(token.value, "");
        *pos = i;
        return token;
    }
    
    token.line = *line;
    token.column = *col;
    
    if (isalpha(input[i]) || input[i] == '_') {
        int j = 0;
        while ((i < len) && (isalnum(input[i]) || input[i] == '_') && j < 63) {
            token.value[j++] = input[i++];
            (*col)++;
        }
        token.value[j] = '\0';
        token.type = TOKEN_IDENTIFIER;
    } else if (isdigit(input[i])) {
        int j = 0;
        while ((i < len) && (isdigit(input[i]) || input[i] == '.') && j < 63) {
            token.value[j++] = input[i++];
            (*col)++;
        }
        token.value[j] = '\0';
        token.type = TOKEN_NUMBER;
    } else if (input[i] == '"') {
        int j = 0;
        token.value[j++] = input[i++];
        (*col)++;
        while (i < len && input[i] != '"' && j < 63) {
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
            strcpy(token.value, "Unterminated string");
        }
    } else if (is_operator_char(input[i])) {
        int j = 0;
        while (i < len && is_operator_char(input[i]) && j < 63) {
            token.value[j++] = input[i++];
            (*col)++;
        }
        token.value[j] = '\0';
        token.type = TOKEN_OPERATOR;
    } else {
        token.value[0] = input[i++];
        token.value[1] = '\0';
        (*col)++;
        token.type = TOKEN_ERROR;
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
    printf("Enter code to tokenize (max 1023 chars): ");
    
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
    int col = 1;
    int token_count = 0;
    
    printf("\nTokens:\n");
    printf("Line Col Type        Value\n");
    printf("---- --- ----------  --------------------\n");
    
    while (1) {
        struct Token token = get_next_token(input, &pos, &line, &col);
        
        if (token.type == TOKEN_EOF) {
            printf("%-4d %-3d %-10s  %s\n", token.line, token.column, 
                   token_type_to_string(token.type), token.value);
            break;
        }
        
        if (token.type == TOKEN_ERROR) {
            printf("%-4d %-3d %-10s  %s\n", token.line, token.column, 
                   token_type_to_string(token.type), token.value);
            if (strcmp(token.value, "Unterminated string") == 0) {
                break;
            }
        } else {
            printf("%-4d %-3d %-10s  %s\n", token.line, token.