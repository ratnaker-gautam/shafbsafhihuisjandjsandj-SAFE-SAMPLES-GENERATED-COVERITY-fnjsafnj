//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_STRING, TOKEN_EOF, TOKEN_UNKNOWN };

struct Token {
    enum TokenType type;
    char value[64];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int get_token(struct Token *token, const char *input, int *pos, int *line, int *col) {
    while (input[*pos] != '\0') {
        if (isspace(input[*pos])) {
            if (input[*pos] == '\n') {
                (*line)++;
                *col = 1;
            } else {
                (*col)++;
            }
            (*pos)++;
            continue;
        }

        token->line = *line;
        token->column = *col;

        if (isalpha(input[*pos]) || input[*pos] == '_') {
            int i = 0;
            while ((isalnum(input[*pos]) || input[*pos] == '_') && i < 63) {
                token->value[i++] = input[*pos];
                (*pos)++;
                (*col)++;
            }
            token->value[i] = '\0';
            token->type = TOKEN_IDENTIFIER;
            return 1;
        } else if (isdigit(input[*pos])) {
            int i = 0;
            while (isdigit(input[*pos]) && i < 63) {
                token->value[i++] = input[*pos];
                (*pos)++;
                (*col)++;
            }
            token->value[i] = '\0';
            token->type = TOKEN_NUMBER;
            return 1;
        } else if (input[*pos] == '"') {
            int i = 0;
            token->value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
            while (input[*pos] != '"' && input[*pos] != '\0' && i < 63) {
                token->value[i++] = input[*pos];
                (*pos)++;
                (*col)++;
            }
            if (input[*pos] == '"') {
                token->value[i++] = input[*pos];
                (*pos)++;
                (*col)++;
            }
            token->value[i] = '\0';
            token->type = TOKEN_STRING;
            return 1;
        } else if (is_operator_char(input[*pos])) {
            int i = 0;
            while (is_operator_char(input[*pos]) && i < 63) {
                token->value[i++] = input[*pos];
                (*pos)++;
                (*col)++;
            }
            token->value[i] = '\0';
            token->type = TOKEN_OPERATOR;
            return 1;
        } else {
            token->value[0] = input[*pos];
            token->value[1] = '\0';
            token->type = TOKEN_UNKNOWN;
            (*pos)++;
            (*col)++;
            return 1;
        }
    }

    token->type = TOKEN_EOF;
    token->value[0] = '\0';
    return 0;
}

int main(void) {
    char input[1024];
    printf("Enter input to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    int pos = 0;
    int line = 1;
    int column = 1;
    struct Token token;

    printf("Tokens:\n");
    printf("%-12s %-16s %-6s %-6s\n", "Type", "Value", "Line", "Column");
    printf("------------------------------------------------\n");

    while (get_token(&token, input, &pos, &line, &column)) {
        if (token.type == TOKEN_EOF) {
            break;
        }

        const char *type_str;
        switch (token.type) {
            case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            case TOKEN_STRING: type_str = "STRING"; break;
            case TOKEN_UNKNOWN: type_str = "UNKNOWN"; break;
            default: type_str = "INVALID"; break;
        }

        printf("%-12s %-16s %-6d %-6d\n", type_str, token.value, token.line, token.column);
    }

    return 0;
}