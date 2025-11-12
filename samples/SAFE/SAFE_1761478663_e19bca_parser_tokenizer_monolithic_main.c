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
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[64];
    int line;
    int column;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int get_operator_length(const char *str) {
    if (str[0] == '\0') return 0;
    if (str[0] == '=' && str[1] == '=') return 2;
    if (str[0] == '!' && str[1] == '=') return 2;
    if (str[0] == '<' && str[1] == '=') return 2;
    if (str[0] == '>' && str[1] == '=') return 2;
    if (str[0] == '&' && str[1] == '&') return 2;
    if (str[0] == '|' && str[1] == '|') return 2;
    return 1;
}

struct Token get_next_token(const char **input, int *line, int *column) {
    struct Token token = {TOKEN_UNKNOWN, "", *line, *column};
    
    while (**input != '\0' && isspace(**input)) {
        if (**input == '\n') {
            (*line)++;
            *column = 1;
        } else {
            (*column)++;
        }
        (*input)++;
    }
    
    if (**input == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        return token;
    }
    
    token.line = *line;
    token.column = *column;
    
    if (isdigit(**input)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (**input != '\0' && (isdigit(**input) || **input == '.')) {
            if (i < 63) {
                token.value[i++] = **input;
            }
            (*column)++;
            (*input)++;
        }
        token.value[i] = '\0';
    } else if (isalpha(**input) || **input == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (**input != '\0' && (isalnum(**input) || **input == '_')) {
            if (i < 63) {
                token.value[i++] = **input;
            }
            (*column)++;
            (*input)++;
        }
        token.value[i] = '\0';
    } else if (**input == '"') {
        token.type = TOKEN_STRING;
        int i = 0;
        (*column)++;
        (*input)++;
        
        while (**input != '\0' && **input != '"') {
            if (i < 63) {
                token.value[i++] = **input;
            }
            (*column)++;
            (*input)++;
        }
        
        if (**input == '"') {
            (*column)++;
            (*input)++;
        }
        token.value[i] = '\0';
    } else if (is_operator_char(**input)) {
        token.type = TOKEN_OPERATOR;
        int len = get_operator_length(*input);
        strncpy(token.value, *input, len);
        token.value[len] = '\0';
        *column += len;
        *input += len;
    } else {
        token.value[0] = **input;
        token.value[1] = '\0';
        (*column)++;
        (*input)++;
    }
    
    return token;
}

void print_token(const struct Token *token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "UNKNOWN"
    };
    printf("Line %d, Column %d: %s", token->line, token->column, type_names[token->type]);
    if (token->type != TOKEN_EOF) {
        printf(" ('%s')", token->value);
    }
    printf("\n");
}

int main(void) {
    char input_buffer[1024];
    printf("Enter text to tokenize (max 1023 characters): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input_buffer[0] == '\n')) {
        printf("No input provided\n");
        return 1;
    }
    
    const char *input_ptr = input_buffer;
    int line = 1;
    int column = 1;
    
    printf("Tokens:\n");
    printf("-------\n");
    
    while (1) {
        struct Token token = get_next_token(&input_ptr, &line, &column);
        print_token(&token);
        
        if (token.type == TOKEN_EOF) {
            break;
        }
    }
    
    return 0;
}