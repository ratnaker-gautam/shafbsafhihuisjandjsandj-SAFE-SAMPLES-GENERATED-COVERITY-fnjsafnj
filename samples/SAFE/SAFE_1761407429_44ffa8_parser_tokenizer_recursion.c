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

void tokenizer_init(struct Tokenizer *t, const char *input) {
    t->input = input;
    t->position = 0;
    t->length = strlen(input);
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

struct Token get_next_token(struct Tokenizer *t) {
    struct Token token = {TOKEN_INVALID, ""};
    
    if (t->position >= t->length) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    while (t->position < t->length && isspace(t->input[t->position])) {
        t->position++;
    }
    
    if (t->position >= t->length) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = t->input[t->position];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        size_t i = 0;
        while (t->position < t->length && i < 31 && isdigit(t->input[t->position])) {
            token.value[i++] = t->input[t->position++];
        }
        token.value[i] = '\0';
    } else if (isalpha(current)) {
        token.type = TOKEN_IDENTIFIER;
        size_t i = 0;
        while (t->position < t->length && i < 31 && (isalnum(t->input[t->position]) || t->input[t->position] == '_')) {
            token.value[i++] = t->input[t->position++];
        }
        token.value[i] = '\0';
    } else if (is_operator(current)) {
        token.type = TOKEN_OPERATOR;
        token.value[0] = current;
        token.value[1] = '\0';
        t->position++;
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = current;
        token.value[1] = '\0';
        t->position++;
    }
    
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "EOF", "INVALID"
    };
    printf("Token: %-12s Value: '%s'\n", type_names[token.type], token.value);
}

void parse_expression(struct Tokenizer *t);

void parse_factor(struct Tokenizer *t) {
    struct Token token = get_next_token(t);
    
    if (token.type == TOKEN_NUMBER) {
        print_token(token);
    } else if (token.type == TOKEN_IDENTIFIER) {
        print_token(token);
    } else if (token.type == TOKEN_OPERATOR && token.value[0] == '-') {
        printf("Token: OPERATOR     Value: '-'\n");
        parse_factor(t);
    } else {
        printf("Unexpected token in factor: %s\n", token.value);
    }
}

void parse_term(struct Tokenizer *t) {
    parse_factor(t);
    
    struct Token token = get_next_token(t);
    if (token.type == TOKEN_OPERATOR && (token.value[0] == '*' || token.value[0] == '/')) {
        print_token(token);
        parse_factor(t);
        t->position--;
    } else {
        t->position--;
    }
}

void parse_expression(struct Tokenizer *t) {
    parse_term(t);
    
    struct Token token = get_next_token(t);
    if (token.type == TOKEN_OPERATOR && (token.value[0] == '+' || token.value[0] == '-')) {
        print_token(token);
        parse_term(t);
        t->position--;
    } else {
        t->position--;
    }
}

int main() {
    char input[256];
    
    printf("Enter an arithmetic expression: ");
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
    
    printf("\nToken stream:\n");
    parse_expression(&tokenizer);
    
    struct Token token;
    do {
        token = get_next_token(&tokenizer);
        if (token.type != TOKEN_EOF) {
            print_token(token);
        }
    } while (token.type != TOKEN_EOF);
    
    return 0;
}