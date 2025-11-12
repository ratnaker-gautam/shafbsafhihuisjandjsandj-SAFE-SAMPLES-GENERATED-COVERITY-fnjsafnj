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
    
    const char *current = input;
    int token_count = 0;
    
    printf("Tokens:\n");
    printf("%-12s %-12s %s\n", "Token #", "Type", "Value");
    printf("--------------------------------\n");
    
    while (1) {
        struct Token token = get_next_token(&current);
        
        if (token_count >= 100) {
            fprintf(stderr, "Too many tokens\n");
            break;
        }
        
        switch (token.type) {
            case TOKEN_EOF:
                printf("%-12d %-12s %s\n", token_count, token_type_to_string(token.type), "(end)");
                return 0;
            case TOKEN_INVALID:
                printf("%-12d %-12s '%s'\n", token_count, token_type_to_string(token.type), token.value);
                fprintf(stderr, "Invalid token encountered\n");
                return 1;
            default:
                printf("%-12d %-12s '%s'\n", token_count, token_type_to_string(token.type), token.value);
                break;
        }
        
        token_count++;
    }
    
    return 0;
}