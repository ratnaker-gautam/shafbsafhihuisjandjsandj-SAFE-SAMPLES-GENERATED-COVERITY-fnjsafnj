//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_EOF, TOKEN_INVALID };

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
    
    char c = **input;
    
    if (isalpha(c)) {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((*input)[i] && (isalnum((*input)[i]) || (*input)[i] == '_')) {
            if (i < 31) {
                token.value[i] = (*input)[i];
            }
            i++;
        }
        if (i < 32) {
            token.value[i] = '\0';
        } else {
            token.value[31] = '\0';
        }
        *input += i;
    } else if (isdigit(c)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while ((*input)[i] && isdigit((*input)[i])) {
            if (i < 31) {
                token.value[i] = (*input)[i];
            }
            i++;
        }
        if (i < 32) {
            token.value[i] = '\0';
        } else {
            token.value[31] = '\0';
        }
        *input += i;
    } else if (is_operator_char(c)) {
        token.type = TOKEN_OPERATOR;
        token.value[0] = c;
        token.value[1] = '\0';
        (*input)++;
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = c;
        token.value[1] = '\0';
        (*input)++;
    }
    
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "EOF", "INVALID"
    };
    printf("Token: %-12s Value: %s\n", type_names[token.type], token.value);
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
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    const char *current = input;
    struct Token token;
    
    do {
        token = get_next_token(&current);
        print_token(token);
    } while (token.type != TOKEN_EOF && token.type != TOKEN_INVALID);
    
    int token_count = 0;
    current = input;
    while (1) {
        token = get_next_token(&current);
        if (token.type == TOKEN_EOF) break;
        if (token.type == TOKEN_INVALID) break;
        token_count++;
    }
    printf("Total valid tokens: %d\n", token_count);
    
    struct Token tokens[100];
    int i = 0;
    current = input;
    for (i = 0; i < 100; i++) {
        tokens[i] = get_next_token(&current);
        if (tokens[i].type == TOKEN_EOF || tokens[i].type == TOKEN_INVALID) {
            break;
        }
    }
    
    printf("Tokens stored in array:\n");
    for (int j = 0; j < i; j++) {
        print_token(tokens[j]);
    }
    
    return 0;
}