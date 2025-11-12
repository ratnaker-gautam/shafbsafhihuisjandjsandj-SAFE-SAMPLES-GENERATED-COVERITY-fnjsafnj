//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_STRING, TOKEN_EOF };

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

int parse_token(const char **input, struct Token *token) {
    skip_whitespace(input);
    
    if (**input == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    if (isalpha(**input)) {
        int i = 0;
        while (**input && (isalnum(**input) || **input == '_') && i < 31) {
            token->value[i++] = **input;
            (*input)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    
    if (isdigit(**input)) {
        int i = 0;
        while (**input && isdigit(**input) && i < 31) {
            token->value[i++] = **input;
            (*input)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_NUMBER;
        return 1;
    }
    
    if (**input == '"') {
        int i = 0;
        (*input)++;
        while (**input && **input != '"' && i < 31) {
            token->value[i++] = **input;
            (*input)++;
        }
        if (**input == '"') {
            (*input)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_STRING;
        return 1;
    }
    
    if (is_operator_char(**input)) {
        int i = 0;
        while (**input && is_operator_char(**input) && i < 31) {
            token->value[i++] = **input;
            (*input)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_OPERATOR;
        return 1;
    }
    
    return 0;
}

void print_token(const struct Token *token) {
    const char *type_names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF"
    };
    printf("%-12s: %s\n", type_names[token->type], token->value);
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    const char *cursor = input;
    struct Token token;
    int token_count = 0;
    
    do {
        if (!parse_token(&cursor, &token)) {
            fprintf(stderr, "Invalid token at: %s\n", cursor);
            return 1;
        }
        
        if (token.type != TOKEN_EOF) {
            print_token(&token);
            token_count++;
        }
        
    } while (token.type != TOKEN_EOF);
    
    printf("Total tokens: %d\n", token_count);
    return 0;
}