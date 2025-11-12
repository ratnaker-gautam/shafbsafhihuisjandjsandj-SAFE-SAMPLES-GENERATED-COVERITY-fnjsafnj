//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_IDENTIFIER, TOKEN_EOF, TOKEN_INVALID };

struct Token {
    enum TokenType type;
    char value[32];
};

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

int parse_number(const char *input, int *pos, int len, char *buffer) {
    int start = *pos;
    int i = 0;
    while (*pos < len && isdigit(input[*pos])) {
        if (i < 31) {
            buffer[i++] = input[(*pos)++];
        } else {
            return 0;
        }
    }
    buffer[i] = '\0';
    return i > 0;
}

int parse_identifier(const char *input, int *pos, int len, char *buffer) {
    int start = *pos;
    int i = 0;
    if (*pos < len && isalpha(input[*pos])) {
        if (i < 31) {
            buffer[i++] = input[(*pos)++];
        } else {
            return 0;
        }
        while (*pos < len && (isalnum(input[*pos]) || input[*pos] == '_')) {
            if (i < 31) {
                buffer[i++] = input[(*pos)++];
            } else {
                return 0;
            }
        }
        buffer[i] = '\0';
        return 1;
    }
    return 0;
}

void skip_whitespace(const char *input, int *pos, int len) {
    while (*pos < len && isspace(input[*pos])) {
        (*pos)++;
    }
}

struct Token get_next_token(const char *input, int *pos, int len) {
    struct Token token = {TOKEN_INVALID, ""};
    skip_whitespace(input, pos, len);
    if (*pos >= len) {
        token.type = TOKEN_EOF;
        return token;
    }
    char current = input[*pos];
    if (isdigit(current)) {
        if (parse_number(input, pos, len, token.value)) {
            token.type = TOKEN_NUMBER;
        }
    } else if (isalpha(current)) {
        if (parse_identifier(input, pos, len, token.value)) {
            token.type = TOKEN_IDENTIFIER;
        }
    } else if (is_operator(current)) {
        token.value[0] = current;
        token.value[1] = '\0';
        token.type = TOKEN_OPERATOR;
        (*pos)++;
    } else {
        token.value[0] = current;
        token.value[1] = '\0';
        (*pos)++;
    }
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {"NUMBER", "OPERATOR", "IDENTIFIER", "EOF", "INVALID"};
    printf("Token: %-12s Value: %s\n", type_names[token.type], token.value);
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    int len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    int pos = 0;
    struct Token token;
    do {
        token = get_next_token(input, &pos, len);
        print_token(token);
    } while (token.type != TOKEN_EOF && token.type != TOKEN_INVALID);
    return 0;
}