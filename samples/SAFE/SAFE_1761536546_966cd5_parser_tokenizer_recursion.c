//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_EOF,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[32];
};

struct Tokenizer {
    const char *input;
    size_t position;
    size_t length;
};

void tokenizer_init(struct Tokenizer *tokenizer, const char *input) {
    tokenizer->input = input;
    tokenizer->position = 0;
    tokenizer->length = strlen(input);
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

void skip_whitespace(struct Tokenizer *tokenizer) {
    while (tokenizer->position < tokenizer->length && 
           isspace(tokenizer->input[tokenizer->position])) {
        tokenizer->position++;
    }
}

struct Token parse_number(struct Tokenizer *tokenizer) {
    struct Token token = {TOKEN_NUMBER, ""};
    size_t i = 0;
    
    while (tokenizer->position < tokenizer->length && 
           i < sizeof(token.value) - 1 &&
           isdigit(tokenizer->input[tokenizer->position])) {
        token.value[i++] = tokenizer->input[tokenizer->position++];
    }
    token.value[i] = '\0';
    return token;
}

struct Token parse_identifier(struct Tokenizer *tokenizer) {
    struct Token token = {TOKEN_IDENTIFIER, ""};
    size_t i = 0;
    
    while (tokenizer->position < tokenizer->length && 
           i < sizeof(token.value) - 1 &&
           isalpha(tokenizer->input[tokenizer->position])) {
        token.value[i++] = tokenizer->input[tokenizer->position++];
    }
    token.value[i] = '\0';
    return token;
}

struct Token parse_operator(struct Tokenizer *tokenizer) {
    struct Token token = {TOKEN_OPERATOR, ""};
    token.value[0] = tokenizer->input[tokenizer->position++];
    token.value[1] = '\0';
    return token;
}

struct Token get_next_token(struct Tokenizer *tokenizer) {
    skip_whitespace(tokenizer);
    
    if (tokenizer->position >= tokenizer->length) {
        struct Token token = {TOKEN_EOF, ""};
        return token;
    }
    
    char current = tokenizer->input[tokenizer->position];
    
    if (isdigit(current)) {
        return parse_number(tokenizer);
    } else if (isalpha(current)) {
        return parse_identifier(tokenizer);
    } else if (is_operator(current)) {
        return parse_operator(tokenizer);
    } else {
        struct Token token = {TOKEN_INVALID, ""};
        token.value[0] = current;
        token.value[1] = '\0';
        tokenizer->position++;
        return token;
    }
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "EOF", "INVALID"
    };
    printf("%s: %s\n", type_names[token.type], token.value);
}

void parse_expression(struct Tokenizer *tokenizer, int depth) {
    if (depth > 100) {
        printf("Error: Expression too complex\n");
        return;
    }
    
    struct Token token = get_next_token(tokenizer);
    
    if (token.type == TOKEN_EOF) {
        return;
    }
    
    if (token.type == TOKEN_INVALID) {
        printf("Error: Invalid token '");
        for (size_t i = 0; i < strlen(token.value); i++) {
            if (isprint(token.value[i])) {
                putchar(token.value[i]);
            } else {
                printf("\\x%02x", (unsigned char)token.value[i]);
            }
        }
        printf("'\n");
        return;
    }
    
    print_token(token);
    
    if (token.type == TOKEN_OPERATOR) {
        parse_expression(tokenizer, depth + 1);
        parse_expression(tokenizer, depth + 1);
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    struct Tokenizer tokenizer;
    tokenizer_init(&tokenizer, input);
    
    printf("Parsing tokens:\n");
    parse_expression(&tokenizer, 0);
    
    return 0;
}