//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_STRING, TOKEN_EOF, TOKEN_INVALID };

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
    
    if (**input == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    if (isalpha((unsigned char)**input) || **input == '_') {
        token->type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((**input && (isalnum((unsigned char)**input) || **input == '_')) && i < 31) {
            token->value[i++] = *(*input)++;
        }
        token->value[i] = '\0';
        return 1;
    }
    
    if (isdigit((unsigned char)**input)) {
        token->type = TOKEN_NUMBER;
        int i = 0;
        while (**input && isdigit((unsigned char)**input) && i < 31) {
            token->value[i++] = *(*input)++;
        }
        token->value[i] = '\0';
        return 1;
    }
    
    if (**input == '"') {
        token->type = TOKEN_STRING;
        int i = 0;
        (*input)++;
        while (**input && **input != '"' && i < 31) {
            token->value[i++] = *(*input)++;
        }
        if (**input == '"') {
            (*input)++;
        }
        token->value[i] = '\0';
        return 1;
    }
    
    if (is_operator_char(**input)) {
        token->type = TOKEN_OPERATOR;
        int i = 0;
        while (**input && is_operator_char(**input) && i < 31) {
            token->value[i++] = *(*input)++;
        }
        token->value[i] = '\0';
        return 1;
    }
    
    token->type = TOKEN_INVALID;
    token->value[0] = **input;
    token->value[1] = '\0';
    (*input)++;
    return 0;
}

const char* token_type_name(enum TokenType type) {
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
    char input_buffer[1024];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
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
    
    while (1) {
        if (!parse_token(&input_ptr, &token)) {
            printf("Invalid token: '%s'\n", token.value);
            break;
        }
        
        if (token.type == TOKEN_EOF) {
            break;
        }
        
        printf("%-12s: '%s'\n", token_type_name(token.type), token.value);
        token_count++;
        
        if (token_count >= 100) {
            printf("Token limit reached\n");
            break;
        }
    }
    
    printf("Total tokens: %d\n", token_count);
    return 0;
}