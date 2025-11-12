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

struct Token get_next_token(const char *input, int *pos, int *line, int *column) {
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
    
    if (isalpha(input[i]) || input[i] == '_') {
        int j = 0;
        while ((i < len) && (isalnum(input[i]) || input[i] == '_') && j < 63) {
            token.value[j++] = input[i++];
            (*column)++;
        }
        token.value[j] = '\0';
        token.type = TOKEN_IDENTIFIER;
    } else if (isdigit(input[i])) {
        int j = 0;
        while ((i < len) && (isdigit(input[i]) || input[i] == '.') && j < 63) {
            token.value[j++] = input[i++];
            (*column)++;
        }
        token.value[j] = '\0';
        token.type = TOKEN_NUMBER;
    } else if (input[i] == '"') {
        int j = 0;
        token.value[j++] = input[i++];
        (*column)++;
        while (i < len && input[i] != '"' && j < 63) {
            token.value[j++] = input[i++];
            (*column)++;
        }
        if (i < len && input[i] == '"') {
            token.value[j++] = input[i++];
            (*column)++;
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
            (*column)++;
        }
        token.value[j] = '\0';
        token.type = TOKEN_OPERATOR;
    } else {
        token.value[0] = input[i++];
        token.value[1] = '\0';
        (*column)++;
        token.type = TOKEN_ERROR;
        strcpy(token.value, "Unknown character");
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
    printf("Enter code to tokenize (max 1023 characters): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    int pos = 0;
    int line = 1;
    int column = 1;
    
    printf("\nTokens:\n");
    printf("Line:Column  Type        Value\n");
    printf("-----------  ----------  --------\n");
    
    while (1) {
        struct Token token = get_next_token(input, &pos, &line, &column);
        
        printf("%3d:%-7d  %-10s  %s\n", 
               token.line, token.column, 
               token_type_to_string(token.type), 
               token.value);
        
        if (token.type == TOKEN_EOF || token.type == TOKEN_ERROR) {
            break;
        }
    }
    
    return 0;
}