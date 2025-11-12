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
    while (**input && isspace((unsigned char)**input)) {
        (*input)++;
    }
}

int parse_token(const char **input, struct Token *token) {
    skip_whitespace(input);
    
    if (!**input) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    char current = **input;
    
    if (isalpha((unsigned char)current) || current == '_') {
        token->type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((**input && (isalnum((unsigned char)**input) || **input == '_')) && i < 31) {
            token->value[i++] = *(*input)++;
        }
        token->value[i] = '\0';
    } else if (isdigit((unsigned char)current)) {
        token->type = TOKEN_NUMBER;
        int i = 0;
        while (**input && isdigit((unsigned char)**input) && i < 31) {
            token->value[i++] = *(*input)++;
        }
        token->value[i] = '\0';
    } else if (is_operator_char(current)) {
        token->type = TOKEN_OPERATOR;
        int i = 0;
        while (**input && is_operator_char(**input) && i < 31) {
            token->value[i++] = *(*input)++;
        }
        token->value[i] = '\0';
    } else if (current == '"') {
        token->type = TOKEN_STRING;
        (*input)++;
        int i = 0;
        while (**input && **input != '"' && i < 31) {
            token->value[i++] = *(*input)++;
        }
        if (**input == '"') {
            (*input)++;
        }
        token->value[i] = '\0';
    } else {
        return 0;
    }
    
    return 1;
}

const char* token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input_buffer[1024];
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    const char *input_ptr = input_buffer;
    struct Token token;
    int token_count = 0;
    
    printf("Tokens:\n");
    
    while (parse_token(&input_ptr, &token)) {
        if (token.type == TOKEN_EOF) {
            break;
        }
        
        printf("%d: %s -> '%s'\n", token_count, token_type_name(token.type), token.value);
        token_count++;
        
        if (token_count >= 100) {
            fprintf(stderr, "Too many tokens\n");
            break;
        }
    }
    
    if (token_count == 0) {
        printf("No valid tokens found\n");
    }
    
    return 0;
}