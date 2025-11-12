//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_STRING, TOKEN_EOF };

struct Token {
    enum TokenType type;
    char value[32];
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *input, int *pos, char *buffer) {
    int start = *pos;
    int i = 0;
    while (isdigit(input[*pos])) {
        if (i < 31) {
            buffer[i++] = input[*pos];
        }
        (*pos)++;
    }
    buffer[i] = '\0';
    return i > 0;
}

int parse_identifier(const char *input, int *pos, char *buffer) {
    int start = *pos;
    int i = 0;
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        if (i < 31) {
            buffer[i++] = input[*pos];
        }
        (*pos)++;
        while (isalnum(input[*pos]) || input[*pos] == '_') {
            if (i < 31) {
                buffer[i++] = input[*pos];
            }
            (*pos)++;
        }
        buffer[i] = '\0';
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, char *buffer) {
    int i = 0;
    if (is_operator_char(input[*pos])) {
        if (i < 31) {
            buffer[i++] = input[*pos];
        }
        (*pos)++;
        if (is_operator_char(input[*pos])) {
            if (i < 31) {
                buffer[i++] = input[*pos];
            }
            (*pos)++;
        }
        buffer[i] = '\0';
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, char *buffer) {
    int i = 0;
    if (input[*pos] == '"') {
        (*pos)++;
        while (input[*pos] != '"' && input[*pos] != '\0') {
            if (i < 31) {
                buffer[i++] = input[*pos];
            }
            (*pos)++;
        }
        if (input[*pos] == '"') {
            (*pos)++;
        }
        buffer[i] = '\0';
        return 1;
    }
    return 0;
}

int get_next_token(const char *input, int *pos, struct Token *token) {
    int length = strlen(input);
    while (*pos < length && isspace(input[*pos])) {
        (*pos)++;
    }
    if (*pos >= length) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    if (parse_number(input, pos, token->value)) {
        token->type = TOKEN_NUMBER;
        return 1;
    }
    if (parse_identifier(input, pos, token->value)) {
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    if (parse_operator(input, pos, token->value)) {
        token->type = TOKEN_OPERATOR;
        return 1;
    }
    if (parse_string(input, pos, token->value)) {
        token->type = TOKEN_STRING;
        return 1;
    }
    return 0;
}

void print_token(struct Token *token) {
    const char *type_names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF"
    };
    printf("%s: %s\n", type_names[token->type], token->value);
}

int main() {
    char input[1024];
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    input[strcspn(input, "\n")] = '\0';
    int pos = 0;
    struct Token token;
    int token_count = 0;
    while (1) {
        if (!get_next_token(input, &pos, &token)) {
            fprintf(stderr, "Invalid token at position %d\n", pos);
            return 1;
        }
        if (token.type == TOKEN_EOF) {
            break;
        }
        print_token(&token);
        token_count++;
        if (token_count > 100) {
            fprintf(stderr, "Too many tokens\n");
            return 1;
        }
    }
    printf("Total tokens: %d\n", token_count);
    return 0;
}