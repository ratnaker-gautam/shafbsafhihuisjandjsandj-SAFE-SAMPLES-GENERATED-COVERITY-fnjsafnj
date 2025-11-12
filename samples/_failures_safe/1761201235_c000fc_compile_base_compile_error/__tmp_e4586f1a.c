//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_STRING, TOKEN_IDENTIFIER, TOKEN_OPERATOR, TOKEN_EOF };

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *input, int *pos, int len, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    while (*pos < len && (isdigit(input[*pos]) || input[*pos] == '.')) {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*pos)++;
    }
    int num_len = *pos - start;
    if (num_len > 31) return 0;
    strncpy(token->value, input + start, num_len);
    token->value[num_len] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_string(const char *input, int *pos, int len, struct Token *token) {
    if (input[*pos] != '"') return 0;
    (*pos)++;
    int start = *pos;
    while (*pos < len && input[*pos] != '"') {
        if (input[*pos] == '\\' && *pos + 1 < len) (*pos)++;
        (*pos)++;
    }
    if (*pos >= len) return 0;
    int str_len = *pos - start;
    if (str_len > 31) return 0;
    strncpy(token->value, input + start, str_len);
    token->value[str_len] = '\0';
    token->type = TOKEN_STRING;
    (*pos)++;
    return 1;
}

int parse_identifier(const char *input, int *pos, int len, struct Token *token) {
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    int start = *pos;
    while (*pos < len && (isalnum(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
    }
    int id_len = *pos - start;
    if (id_len > 31) return 0;
    strncpy(token->value, input + start, id_len);
    token->value[id_len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char *input, int *pos, int len, struct Token *token) {
    if (!is_operator_char(input[*pos])) return 0;
    int start = *pos;
    while (*pos < len && is_operator_char(input[*pos])) {
        (*pos)++;
    }
    int op_len = *pos - start;
    if (op_len > 31) return 0;
    strncpy(token->value, input + start, op_len);
    token->value[op_len] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int get_next_token(const char *input, int *pos, int len, struct Token *token, int line, int *column) {
    while (*pos < len && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            line++;
            *column = 1;
        } else {
            (*column)++;
        }
        (*pos)++;
    }
    if (*pos >= len) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = line;
        token->column = *column;
        return 1;
    }
    token->line = line;
    token->column = *column;
    if (parse_number(input, pos, len, token)) {
        *column += (int)strlen(token->value);
        return 1;
    }
    if (parse_string(input, pos, len, token)) {
        *column += (int)strlen(token->value) + 2;
        return 1;
    }
    if (parse_identifier(input, pos, len, token)) {
        *column += (int)strlen(token->value);
        return 1;
    }
    if (parse_operator(input, pos, len, token)) {
        *column += (int)strlen(token->value);
        return 1;
    }
    return 0;
}

void print_token(struct Token *token) {
    const char *type_names[] = {"NUMBER", "STRING", "IDENTIFIER", "OPERATOR", "EOF"};
    printf("Line %d, Column %d: %s [%s]\n", token->line, token->column, type_names[token->type], token->value);
}

int main(void) {
    char input[1024];
    printf("Enter input to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    int len = (int)strlen(input);
    if (len > 0 && input[len-1] == '\n')