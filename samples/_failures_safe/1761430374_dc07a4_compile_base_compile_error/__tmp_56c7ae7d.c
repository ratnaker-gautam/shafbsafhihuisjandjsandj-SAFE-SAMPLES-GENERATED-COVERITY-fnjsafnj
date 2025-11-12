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
    size_t pos;
    size_t len;
};

void init_tokenizer(struct Tokenizer *t, const char *str) {
    t->input = str;
    t->pos = 0;
    t->len = strlen(str);
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

struct Token get_next_token(struct Tokenizer *t) {
    struct Token token = {TOKEN_INVALID, ""};
    
    if (t->pos >= t->len) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    while (t->pos < t->len && isspace(t->input[t->pos])) {
        t->pos++;
    }
    
    if (t->pos >= t->len) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = t->input[t->pos];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        size_t i = 0;
        while (t->pos < t->len && i < 31 && isdigit(t->input[t->pos])) {
            token.value[i++] = t->input[t->pos++];
        }
        token.value[i] = '\0';
    } else if (isalpha(current)) {
        token.type = TOKEN_IDENTIFIER;
        size_t i = 0;
        while (t->pos < t->len && i < 31 && (isalnum(t->input[t->pos]) || t->input[t->pos] == '_')) {
            token.value[i++] = t->input[t->pos++];
        }
        token.value[i] = '\0';
    } else if (is_operator(current)) {
        token.type = TOKEN_OPERATOR;
        token.value[0] = current;
        token.value[1] = '\0';
        t->pos++;
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = current;
        token.value[1] = '\0';
        t->pos++;
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
    
    if (token.type == TOKEN_NUMBER || token.type == TOKEN_IDENTIFIER) {
        print_token(token);
    } else if (token.type == TOKEN_OPERATOR && token.value[0] == '-') {
        printf("Unary minus detected\n");
        parse_factor(t);
    } else {
        printf("Error: Expected number or identifier, got ");
        print_token(token);
    }
}

void parse_term(struct Tokenizer *t) {
    parse_factor(t);
    
    struct Token token = get_next_token(t);
    if (token.type == TOKEN_OPERATOR && (token.value[0] == '*' || token.value[0] == '/')) {
        print_token(token);
        parse_term(t);
    } else {
        size_t token_len = strlen(token.value);
        if (t->pos >= token_len) {
            t->pos -= token_len;
        }
    }
}

void parse_expression(struct Tokenizer *t) {
    parse_term(t);
    
    struct Token token = get_next_token(t);
    if (token.type == TOKEN_OPERATOR && (token.value[0] == '+' || token.value[0] == '-')) {
        print_token(token);
        parse_expression(t);
    } else {
        size_t token_len = strlen(token.value);
        if (t->pos >= token_len) {
            t->pos -= token_len;
        }
    }
}

int main(void) {
    char input[256];
    
    printf("Enter an expression to tokenize and parse: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    printf("\nTokenizing and parsing: %s\n\n", input);
    
    struct Tokenizer tokenizer;
    init_tokenizer(&tokenizer, input);
    
    printf("Tokens:\n");
    struct Token token;
    do {
        token = get_next_token(&tokenizer);
        print_token(token);
    } while (token.type != TOKEN_EOF && token.type != TOKEN_INVALID);
    
    printf("\nRecursive descent parse:\n");
    init_tokenizer(&tokenizer, input);
    parse_expression(&tokenizer);
    
    struct Token final = get_next_token(&tokenizer