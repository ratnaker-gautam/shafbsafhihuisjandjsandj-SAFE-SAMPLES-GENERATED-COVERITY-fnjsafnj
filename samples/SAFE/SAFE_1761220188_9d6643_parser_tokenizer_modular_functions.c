//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: parser_tokenizer
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
    TOKEN_EOF,
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

struct Token create_token(enum TokenType type, const char* value, int line, int column) {
    struct Token token;
    token.type = type;
    token.line = line;
    token.column = column;
    strncpy(token.value, value, MAX_TOKEN_LEN - 1);
    token.value[MAX_TOKEN_LEN - 1] = '\0';
    return token;
}

int tokenize(const char* input, struct Token* tokens, int max_tokens) {
    int token_count = 0;
    int line = 1;
    int column = 1;
    int i = 0;
    int input_len = strlen(input);

    while (i < input_len && token_count < max_tokens) {
        if (isspace(input[i])) {
            if (input[i] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            i++;
            continue;
        }

        if (isdigit(input[i])) {
            char number[MAX_TOKEN_LEN] = {0};
            int j = 0;
            while (i < input_len && j < MAX_TOKEN_LEN - 1 && isdigit(input[i])) {
                number[j++] = input[i++];
                column++;
            }
            tokens[token_count++] = create_token(TOKEN_NUMBER, number, line, column - j);
            continue;
        }

        if (isalpha(input[i]) || input[i] == '_') {
            char identifier[MAX_TOKEN_LEN] = {0};
            int j = 0;
            while (i < input_len && j < MAX_TOKEN_LEN - 1 && (isalnum(input[i]) || input[i] == '_')) {
                identifier[j++] = input[i++];
                column++;
            }
            tokens[token_count++] = create_token(TOKEN_IDENTIFIER, identifier, line, column - j);
            continue;
        }

        if (input[i] == '"') {
            char string[MAX_TOKEN_LEN] = {0};
            int j = 0;
            column++;
            i++;
            while (i < input_len && j < MAX_TOKEN_LEN - 1 && input[i] != '"') {
                string[j++] = input[i++];
                column++;
            }
            if (i < input_len && input[i] == '"') {
                i++;
                column++;
            }
            tokens[token_count++] = create_token(TOKEN_STRING, string, line, column - j - 1);
            continue;
        }

        if (is_operator_char(input[i])) {
            char op[3] = {0};
            op[0] = input[i];
            column++;
            i++;
            if (i < input_len && is_operator_char(input[i])) {
                op[1] = input[i];
                column++;
                i++;
            }
            tokens[token_count++] = create_token(TOKEN_OPERATOR, op, line, column - strlen(op));
            continue;
        }

        char unknown[2] = {input[i], '\0'};
        tokens[token_count++] = create_token(TOKEN_UNKNOWN, unknown, line, column);
        i++;
        column++;
    }

    if (token_count < max_tokens) {
        tokens[token_count++] = create_token(TOKEN_EOF, "", line, column);
    }

    return token_count;
}

void print_token(struct Token token) {
    const char* type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "UNKNOWN"
    };
    printf("Line %d, Column %d: %s", token.line, token.column, type_names[token.type]);
    if (token.type != TOKEN_EOF) {
        printf(" ('%s')", token.value);
    }
    printf("\n");
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

    struct Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);

    printf("Tokens found: %d\n", token_count);
    for (int i = 0; i < token_count; i++) {
        print_token(tokens[i]);
    }

    return 0;
}