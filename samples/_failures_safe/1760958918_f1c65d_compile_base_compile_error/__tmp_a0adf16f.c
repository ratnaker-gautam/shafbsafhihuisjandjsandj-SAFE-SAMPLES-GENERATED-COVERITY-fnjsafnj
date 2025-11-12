//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: parser_tokenizer
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
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

int tokenize(const char* input, struct Token* tokens, int max_tokens) {
    int token_count = 0;
    int line = 1;
    int column = 1;
    int i = 0;
    int input_len = strlen(input);

    while (i < input_len && token_count < max_tokens) {
        while (i < input_len && isspace(input[i])) {
            if (input[i] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            i++;
        }

        if (i >= input_len) break;

        int start = i;
        int token_line = line;
        int token_column = column;

        if (isdigit(input[i])) {
            while (i < input_len && isdigit(input[i])) {
                i++;
                column++;
            }
            if (i - start >= MAX_TOKEN_LEN) {
                return -1;
            }
            tokens[token_count].type = TOKEN_NUMBER;
            strncpy(tokens[token_count].value, &input[start], i - start);
            tokens[token_count].value[i - start] = '\0';
            tokens[token_count].line = token_line;
            tokens[token_count].column = token_column;
            token_count++;
        } else if (isalpha(input[i]) || input[i] == '_') {
            while (i < input_len && is_valid_identifier_char(input[i])) {
                i++;
                column++;
            }
            if (i - start >= MAX_TOKEN_LEN) {
                return -1;
            }
            tokens[token_count].type = TOKEN_IDENTIFIER;
            strncpy(tokens[token_count].value, &input[start], i - start);
            tokens[token_count].value[i - start] = '\0';
            tokens[token_count].line = token_line;
            tokens[token_count].column = token_column;
            token_count++;
        } else if (input[i] == '"') {
            i++;
            column++;
            start = i;
            while (i < input_len && input[i] != '"') {
                if (input[i] == '\n') {
                    return -1;
                }
                i++;
                column++;
            }
            if (i >= input_len) {
                return -1;
            }
            if (i - start >= MAX_TOKEN_LEN) {
                return -1;
            }
            tokens[token_count].type = TOKEN_STRING;
            strncpy(tokens[token_count].value, &input[start], i - start);
            tokens[token_count].value[i - start] = '\0';
            tokens[token_count].line = token_line;
            tokens[token_count].column = token_column;
            token_count++;
            i++;
            column++;
        } else if (is_operator_char(input[i])) {
            while (i < input_len && is_operator_char(input[i])) {
                i++;
                column++;
            }
            if (i - start >= MAX_TOKEN_LEN) {
                return -1;
            }
            tokens[token_count].type = TOKEN_OPERATOR;
            strncpy(tokens[token_count].value, &input[start], i - start);
            tokens[token_count].value[i - start] = '\0';
            tokens[token_count].line = token_line;
            tokens[token_count].column = token_column;
            token_count++;
        } else {
            return -1;
        }
    }

    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        token_count++;
    }

    return token_count;
}

void print_token(const struct Token* token) {
    const char* type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"
    };
    printf("Token: %-12s Value: '%-10s' Line: %2d Column: %2d\n",
           type_names[token->type], token->value, token->line, token->column);
}

int main(void) {
    char input[1024];
    struct Token tokens[MAX_TOKENS];

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

    int