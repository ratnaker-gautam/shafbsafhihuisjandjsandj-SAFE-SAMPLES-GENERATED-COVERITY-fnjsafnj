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
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(const char **input) {
    while (**input && isspace(**input)) {
        (*input)++;
    }
}

struct Token get_next_token(const char **input) {
    struct Token token = {TOKEN_INVALID, ""};
    skip_whitespace(input);
    
    if (**input == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    if (isalpha(**input)) {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (**input && (isalnum(**input) || **input == '_') && i < 31) {
            token.value[i++] = **input;
            (*input)++;
        }
        token.value[i] = '\0';
    } else if (isdigit(**input)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (**input && isdigit(**input) && i < 31) {
            token.value[i++] = **input;
            (*input)++;
        }
        token.value[i] = '\0';
    } else if (**input == '"') {
        token.type = TOKEN_STRING;
        (*input)++;
        int i = 0;
        while (**input && **input != '"' && i < 31) {
            token.value[i++] = **input;
            (*input)++;
        }
        token.value[i] = '\0';
        if (**input == '"') {
            (*input)++;
        }
    } else if (is_operator_char(**input)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (**input && is_operator_char(**input) && i < 31) {
            token.value[i++] = **input;
            (*input)++;
        }
        token.value[i] = '\0';
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = **input;
        token.value[1] = '\0';
        (*input)++;
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
    
    const char *cursor = input;
    int token_count = 0;
    
    printf("\nToken stream:\n");
    printf("%-12s %s\n", "TYPE", "VALUE");
    printf("------------ ----------\n");
    
    while (1) {
        struct Token token = get_next_token(&cursor);
        
        switch (token.type) {
            case TOKEN_IDENTIFIER:
            case TOKEN_NUMBER:
            case TOKEN_OPERATOR:
            case TOKEN_STRING:
                printf("%-12s '%s'\n", token_type_to_string(token.type), token.value);
                token_count++;
                break;
                
            case TOKEN_INVALID:
                printf("%-12s '%s'\n", token_type_to_string(token.type), token.value);
                token_count++;
                break;
                
            case TOKEN_EOF:
                printf("%-12s\n", token_type_to_string(token.type));
                break;
                
            default:
                break;
        }
        
        if (token.type == TOKEN_EOF) {
            break;
        }
        
        if (token_count >= 100) {
            fprintf(stderr, "Too many tokens\n");
            break;
        }
    }
    
    printf("\nTotal tokens: %d\n", token_count);
    return 0;
}