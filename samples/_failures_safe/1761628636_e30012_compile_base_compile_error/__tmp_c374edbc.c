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
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(const char *input, int *pos) {
    while (input[*pos] && isspace(input[*pos])) {
        (*pos)++;
    }
}

int parse_number(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    while (input[*pos] && isdigit(input[*pos])) {
        (*pos)++;
    }
    if (*pos - start >= MAX_TOKEN_LEN) {
        return 0;
    }
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    while (input[*pos] && (isalnum(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
    }
    if (*pos - start >= MAX_TOKEN_LEN) {
        return 0;
    }
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    if (*pos >= MAX_TOKEN_LEN - 1) {
        return 0;
    }
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    token->type = TOKEN_OPERATOR;
    (*pos)++;
    return 1;
}

int parse_string(const char *input, int *pos, struct Token *token) {
    if (input[*pos] != '"') {
        return 0;
    }
    (*pos)++;
    int start = *pos;
    while (input[*pos] && input[*pos] != '"') {
        (*pos)++;
    }
    if (!input[*pos]) {
        return 0;
    }
    if (*pos - start >= MAX_TOKEN_LEN) {
        return 0;
    }
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_STRING;
    (*pos)++;
    return 1;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int line = 1;
    int column = 1;

    while (input[pos] && token_count < max_tokens) {
        skip_whitespace(input, &pos);
        if (!input[pos]) {
            break;
        }

        tokens[token_count].line = line;
        tokens[token_count].column = column;

        if (isdigit(input[pos])) {
            if (!parse_number(input, &pos, &tokens[token_count])) {
                return -1;
            }
        } else if (isalpha(input[pos]) || input[pos] == '_') {
            if (!parse_identifier(input, &pos, &tokens[token_count])) {
                return -1;
            }
        } else if (is_operator_char(input[pos])) {
            if (!parse_operator(input, &pos, &tokens[token_count])) {
                return -1;
            }
        } else if (input[pos] == '"') {
            if (!parse_string(input, &pos, &tokens[token_count])) {
                return -1;
            }
        } else {
            return -1;
        }

        column += (int)strlen(tokens[token_count].value);
        token_count++;
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

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"
    };
    printf("%s: '%s' at line %d, column %d\n", 
           type_names[token.type], token.value, token.line, token.column);
}

int main() {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0