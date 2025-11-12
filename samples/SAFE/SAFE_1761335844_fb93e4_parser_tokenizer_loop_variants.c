//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_STRING, TOKEN_SYMBOL, TOKEN_EOF };

struct Token {
    enum TokenType type;
    char value[32];
};

int is_symbol_char(char c) {
    return isalnum(c) || c == '_';
}

int parse_number(const char *input, int *pos, char *buffer) {
    int start = *pos;
    int i = 0;
    while (isdigit(input[*pos])) {
        if (i >= 31) return 0;
        buffer[i++] = input[(*pos)++];
    }
    buffer[i] = '\0';
    return 1;
}

int parse_string(const char *input, int *pos, char *buffer) {
    if (input[*pos] != '"') return 0;
    (*pos)++;
    int i = 0;
    while (input[*pos] != '"' && input[*pos] != '\0') {
        if (i >= 31) return 0;
        buffer[i++] = input[(*pos)++];
    }
    if (input[*pos] != '"') return 0;
    (*pos)++;
    buffer[i] = '\0';
    return 1;
}

int parse_symbol(const char *input, int *pos, char *buffer) {
    int i = 0;
    while (is_symbol_char(input[*pos])) {
        if (i >= 31) return 0;
        buffer[i++] = input[(*pos)++];
    }
    buffer[i] = '\0';
    return i > 0;
}

int get_next_token(const char *input, int *pos, struct Token *token) {
    int len = strlen(input);
    while (*pos < len && isspace(input[*pos])) {
        (*pos)++;
    }
    if (*pos >= len) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    if (isdigit(input[*pos])) {
        if (parse_number(input, pos, token->value)) {
            token->type = TOKEN_NUMBER;
            return 1;
        }
        return 0;
    }
    if (input[*pos] == '"') {
        if (parse_string(input, pos, token->value)) {
            token->type = TOKEN_STRING;
            return 1;
        }
        return 0;
    }
    if (is_symbol_char(input[*pos])) {
        if (parse_symbol(input, pos, token->value)) {
            token->type = TOKEN_SYMBOL;
            return 1;
        }
        return 0;
    }
    return 0;
}

void print_token(struct Token *token) {
    const char *type_names[] = {"NUMBER", "STRING", "SYMBOL", "EOF"};
    printf("Token: %s", type_names[token->type]);
    if (token->type != TOKEN_EOF) {
        printf(" [%s]", token->value);
    }
    printf("\n");
}

int main(void) {
    char input[256];
    printf("Enter text to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    input[strcspn(input, "\n")] = '\0';
    int pos = 0;
    struct Token token;
    int token_count = 0;
    for (;;) {
        if (!get_next_token(input, &pos, &token)) {
            fprintf(stderr, "Error parsing at position %d\n", pos);
            return 1;
        }
        print_token(&token);
        token_count++;
        if (token.type == TOKEN_EOF) break;
        if (token_count >= 100) {
            fprintf(stderr, "Too many tokens\n");
            return 1;
        }
    }
    printf("Total tokens: %d\n", token_count);
    return 0;
}