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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

struct Token get_next_token(const char *input, int *pos, int *line, int *col) {
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
    
    if (isdigit(input[i])) {
        token.type = TOKEN_NUMBER;
        int j = 0;
        while (i < len && (isdigit(input[i]) || input[i] == '.')) {
            if (j < 63) {
                token.value[j++] = input[i];
            }
            i++;
            (*col)++;
        }
        token.value[j] = '\0';
    } else if (isalpha(input[i]) || input[i] == '_') {
        token.type = TOKEN_IDENTIFIER;
        int j = 0;
        while (i < len && (isalnum(input[i]) || input[i] == '_')) {
            if (j < 63) {
                token.value[j++] = input[i];
            }
            i++;
            (*col)++;
        }
        token.value[j] = '\0';
    } else if (input[i] == '"') {
        token.type = TOKEN_STRING;
        int j = 0;
        i++;
        (*col)++;
        while (i < len && input[i] != '"') {
            if (j < 63) {
                token.value[j++] = input[i];
            }
            i++;
            (*col)++;
        }
        if (i < len && input[i] == '"') {
            i++;
            (*col)++;
        }
        token.value[j] = '\0';
    } else if (is_operator_char(input[i])) {
        token.type = TOKEN_OPERATOR;
        int j = 0;
        while (i < len && is_operator_char(input[i]) && j < 63) {
            token.value[j++] = input[i];
            i++;
            (*col)++;
        }
        token.value[j] = '\0';
    } else {
        token.value[0] = input[i];
        token.value[1] = '\0';
        i++;
        (*col)++;
    }
    
    *pos = i;
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    printf("Line %d, Col %d: %s", token.line, token.column, type_names[token.type]);
    if (token.type != TOKEN_EOF && token.type != TOKEN_ERROR) {
        printf(" ('%s')", token.value);
    }
    printf("\n");
}

int main(void) {
    char input[1024];
    printf("Enter text to tokenize (max 1023 characters):\n");
    
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
    
    while (1) {
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