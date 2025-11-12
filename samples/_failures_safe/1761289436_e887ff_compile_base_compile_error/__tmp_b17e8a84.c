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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '(' || c == ')';
}

void skip_whitespace(struct Tokenizer *t) {
    while (t->position < t->length && isspace(t->input[t->position])) {
        t->position++;
    }
}

struct Token parse_number(struct Tokenizer *t) {
    struct Token token = {TOKEN_NUMBER, ""};
    size_t idx = 0;
    
    while (t->position < t->length && idx < 31) {
        char c = t->input[t->position];
        if (isdigit(c)) {
            token.value[idx++] = c;
            t->position++;
        } else {
            break;
        }
    }
    token.value[idx] = '\0';
    return token;
}

struct Token parse_identifier(struct Tokenizer *t) {
    struct Token token = {TOKEN_IDENTIFIER, ""};
    size_t idx = 0;
    
    while (t->position < t->length && idx < 31) {
        char c = t->input[t->position];
        if (isalnum(c) || c == '_') {
            token.value[idx++] = c;
            t->position++;
        } else {
            break;
        }
    }
    token.value[idx] = '\0';
    return token;
}

struct Token parse_operator(struct Tokenizer *t) {
    struct Token token = {TOKEN_OPERATOR, ""};
    token.value[0] = t->input[t->position];
    token.value[1] = '\0';
    t->position++;
    return token;
}

struct Token get_next_token(struct Tokenizer *t) {
    skip_whitespace(t);
    
    if (t->position >= t->length) {
        struct Token token = {TOKEN_EOF, ""};
        return token;
    }
    
    char current = t->input[t->position];
    
    if (isdigit(current)) {
        return parse_number(t);
    } else if (isalpha(current) || current == '_') {
        return parse_identifier(t);
    } else if (is_operator(current)) {
        return parse_operator(t);
    } else {
        struct Token token = {TOKEN_INVALID, ""};
        token.value[0] = current;
        token.value[1] = '\0';
        t->position++;
        return token;
    }
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "EOF", "INVALID"
    };
    printf("%s", type_names[token.type]);
    if (token.value[0] != '\0') {
        printf("(%s)", token.value);
    }
    printf("\n");
}

void parse_expression(struct Tokenizer *t);

void parse_primary(struct Tokenizer *t) {
    struct Token token = get_next_token(t);
    
    if (token.type == TOKEN_NUMBER) {
        printf("Found number: %s\n", token.value);
    } else if (token.type == TOKEN_IDENTIFIER) {
        printf("Found identifier: %s\n", token.value);
    } else if (token.type == TOKEN_OPERATOR && token.value[0] == '(') {
        printf("Entering subexpression\n");
        parse_expression(t);
        token = get_next_token(t);
        if (token.type != TOKEN_OPERATOR || token.value[0] != ')') {
            printf("Error: Expected closing parenthesis\n");
        } else {
            printf("Exiting subexpression\n");
        }
    } else {
        printf("Error: Unexpected token ");
        print_token(token);
    }
}

void parse_expression(struct Tokenizer *t) {
    parse_primary(t);
    
    struct Token token = get_next_token(t);
    
    if (token.type == TOKEN_OPERATOR && 
        (token.value[0] == '+' || token.value[0] == '-' || 
         token.value[0] == '*' || token.value[0] == '/')) {
        printf("Found operator: %s\n", token.value);
        parse_expression(t);
    } else if (token.type != TOKEN_EOF && 
               !(token.type == TOKEN_OPERATOR && token.value[0] == ')')) {
        if (t->position > 0) {
            t->position--;
        }
    }
}

int main() {
    char input[256];
    
    printf("Enter expression to parse: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (strlen(input) ==