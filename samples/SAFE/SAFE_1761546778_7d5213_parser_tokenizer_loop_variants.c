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
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(const char **input, int *line) {
    while (**input != '\0') {
        if (**input == '\n') {
            (*line)++;
            (*input)++;
        } else if (isspace((unsigned char)**input)) {
            (*input)++;
        } else {
            break;
        }
    }
}

struct Token get_next_token(const char **input, int *line) {
    struct Token token = {TOKEN_ERROR, "", *line};
    skip_whitespace(input, line);
    
    if (**input == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = **input;
    
    if (isdigit((unsigned char)current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit((unsigned char)**input) && i < 31) {
            token.value[i++] = **input;
            (*input)++;
        }
        token.value[i] = '\0';
    } else if (isalpha((unsigned char)current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum((unsigned char)**input) || **input == '_') && i < 31) {
            token.value[i++] = **input;
            (*input)++;
        }
        token.value[i] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        (*input)++;
        int i = 0;
        while (**input != '"' && **input != '\0' && i < 31) {
            token.value[i++] = **input;
            (*input)++;
        }
        token.value[i] = '\0';
        if (**input == '"') {
            (*input)++;
        } else {
            token.type = TOKEN_ERROR;
        }
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (is_operator_char(**input) && i < 31) {
            token.value[i++] = **input;
            (*input)++;
        }
        token.value[i] = '\0';
    } else {
        token.type = TOKEN_ERROR;
        token.value[0] = **input;
        token.value[1] = '\0';
        (*input)++;
    }
    
    return token;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input_buffer[1024];
    printf("Enter text to tokenize: ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    const char *input_ptr = input_buffer;
    int line = 1;
    
    printf("\nTokens:\n");
    printf("Line | Type       | Value\n");
    printf("-----+------------+------------------\n");
    
    struct Token token;
    do {
        token = get_next_token(&input_ptr, &line);
        
        switch (token.type) {
            case TOKEN_NUMBER:
            case TOKEN_IDENTIFIER:
            case TOKEN_OPERATOR:
            case TOKEN_STRING:
            case TOKEN_ERROR:
                printf("%4d | %-10s | %s\n", token.line, token_type_to_string(token.type), token.value);
                break;
            case TOKEN_EOF:
                printf("%4d | %-10s | \n", token.line, token_type_to_string(token.type));
                break;
            default:
                break;
        }
        
    } while (token.type != TOKEN_EOF && token.type != TOKEN_ERROR);
    
    if (token.type == TOKEN_ERROR) {
        fprintf(stderr, "Lexical error at line %d\n", token.line);
        return 1;
    }
    
    return 0;
}