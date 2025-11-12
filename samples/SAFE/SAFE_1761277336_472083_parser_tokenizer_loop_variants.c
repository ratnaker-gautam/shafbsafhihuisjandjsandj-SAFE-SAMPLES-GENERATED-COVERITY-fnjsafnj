//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(const char **input) {
    while (**input && isspace(**input)) {
        (*input)++;
    }
}

int parse_number(const char **input, struct Token *token) {
    int i = 0;
    while (**input && (isdigit(**input) || **input == '.')) {
        if (i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = **input;
        }
        (*input)++;
    }
    token->value[i] = '\0';
    token->type = TOKEN_NUMBER;
    return i > 0;
}

int parse_identifier(const char **input, struct Token *token) {
    int i = 0;
    if (**input && (isalpha(**input) || **input == '_')) {
        token->value[i++] = **input;
        (*input)++;
        while (**input && (isalnum(**input) || **input == '_')) {
            if (i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = **input;
            }
            (*input)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    return 0;
}

int parse_operator(const char **input, struct Token *token) {
    int i = 0;
    if (**input && is_operator_char(**input)) {
        token->value[i++] = **input;
        (*input)++;
        while (**input && is_operator_char(**input) && i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = **input;
            (*input)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_OPERATOR;
        return 1;
    }
    return 0;
}

int parse_string(const char **input, struct Token *token) {
    int i = 0;
    if (**input == '"') {
        (*input)++;
        while (**input && **input != '"') {
            if (i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = **input;
            }
            (*input)++;
        }
        if (**input == '"') {
            (*input)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_STRING;
        return 1;
    }
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int count = 0;
    while (*input && count < max_tokens) {
        skip_whitespace(&input);
        if (!*input) break;

        struct Token token;
        int parsed = 0;

        if (parse_number(&input, &token)) parsed = 1;
        else if (parse_string(&input, &token)) parsed = 1;
        else if (parse_identifier(&input, &token)) parsed = 1;
        else if (parse_operator(&input, &token)) parsed = 1;

        if (parsed) {
            tokens[count++] = token;
        } else {
            input++;
        }
    }

    if (count < max_tokens) {
        tokens[count].type = TOKEN_EOF;
        strcpy(tokens[count].value, "");
        count++;
    }

    return count;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"
    };
    printf("%-12s: %s\n", type_names[token.type], token.value);
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

    struct Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);

    printf("\nTokens found:\n");
    int i = 0;
    while (i < token_count) {
        print_token(tokens[i]);
        i++;
    }

    int j = 0;
    printf("\nToken types:\n");
    do {
        printf("%d ", tokens[j].type);
        j++;
    } while (j < token_count);

    printf("\n\nToken values:\n");
    for (int k = 0; k < token_count; k++) {
        printf("\"%s\" ", tokens[k].value);
    }
    printf("\n");

    return 0;
}