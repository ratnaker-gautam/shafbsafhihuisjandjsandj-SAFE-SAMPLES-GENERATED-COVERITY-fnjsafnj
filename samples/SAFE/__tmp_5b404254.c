//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

typedef void (*TokenHandler)(Token*);

void handle_number(Token* token) {
    printf("Number: %s\n", token->value);
}

void handle_identifier(Token* token) {
    printf("Identifier: %s\n", token->value);
}

void handle_operator(Token* token) {
    printf("Operator: %s\n", token->value);
}

void handle_string(Token* token) {
    printf("String: %s\n", token->value);
}

void skip_whitespace(const char** input) {
    while (**input && isspace((unsigned char)**input)) {
        (*input)++;
    }
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int parse_number(const char** input, Token* token) {
    char* end;
    strtod(*input, &end);
    if (end > *input) {
        size_t len = end - *input;
        if (len >= MAX_TOKEN_LEN) {
            return 0;
        }
        strncpy(token->value, *input, len);
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        *input = end;
        return 1;
    }
    return 0;
}

int parse_identifier(const char** input, Token* token) {
    const char* start = *input;
    if (**input && (isalpha((unsigned char)**input) || **input == '_')) {
        (*input)++;
        while (**input && (isalnum((unsigned char)**input) || **input == '_')) {
            (*input)++;
        }
        size_t len = *input - start;
        if (len >= MAX_TOKEN_LEN) {
            return 0;
        }
        strncpy(token->value, start, len);
        token->value[len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    return 0;
}

int parse_operator(const char** input, Token* token) {
    if (**input && is_operator_char(**input)) {
        token->value[0] = **input;
        token->value[1] = '\0';
        token->type = TOKEN_OPERATOR;
        (*input)++;
        return 1;
    }
    return 0;
}

int parse_string(const char** input, Token* token) {
    if (**input == '"') {
        const char* start = *input + 1;
        (*input)++;
        while (**input && **input != '"') {
            (*input)++;
        }
        if (**input != '"') {
            return 0;
        }
        size_t len = *input - start;
        if (len >= MAX_TOKEN_LEN) {
            return 0;
        }
        strncpy(token->value, start, len);
        token->value[len] = '\0';
        token->type = TOKEN_STRING;
        (*input)++;
        return 1;
    }
    return 0;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    int count = 0;
    while (*input && count < max_tokens) {
        skip_whitespace(&input);
        if (!*input) {
            break;
        }
        Token token;
        if (parse_number(&input, &token)) {
            tokens[count++] = token;
        } else if (parse_identifier(&input, &token)) {
            tokens[count++] = token;
        } else if (parse_operator(&input, &token)) {
            tokens[count++] = token;
        } else if (parse_string(&input, &token)) {
            tokens[count++] = token;
        } else {
            break;
        }
    }
    if (count < max_tokens) {
        tokens[count].type = TOKEN_EOF;
        strcpy(tokens[count].value, "");
        count++;
    }
    return count;
}

void process_tokens(Token* tokens, int count, TokenHandler* handlers) {
    for (int i = 0; i < count; i++) {
        if (tokens[i].type == TOKEN_EOF) {
            break;
        }
        if (tokens[i].type >= 0 && tokens[i].type <= TOKEN_STRING) {
            handlers[tokens[i].type](&tokens[i]);
        }
    }
}

int main() {
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
    if (len == 1) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    Token tokens[MAX_TOKENS];
    int