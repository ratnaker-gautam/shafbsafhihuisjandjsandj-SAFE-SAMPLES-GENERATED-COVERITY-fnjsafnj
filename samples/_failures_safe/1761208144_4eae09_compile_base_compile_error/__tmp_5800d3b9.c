//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_ERROR
};

struct Token {
    enum TokenType type;
    char value[64];
    int line;
    int column;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_operator(const char *input, int pos, char *op) {
    int input_len = (int)strlen(input);
    if (pos < 0 || pos >= input_len) return 0;
    
    char c = input[pos];
    if (!is_operator_char(c)) return 0;
    
    op[0] = c;
    op[1] = '\0';
    
    if (pos + 1 < input_len) {
        char next = input[pos + 1];
        if ((c == '=' && next == '=') || (c == '!' && next == '=') ||
            (c == '<' && next == '=') || (c == '>' && next == '=') ||
            (c == '&' && next == '&') || (c == '|' && next == '|')) {
            op[1] = next;
            op[2] = '\0';
            return 2;
        }
    }
    return 1;
}

struct Token get_next_token(const char *input, int *pos, int line, int *col) {
    struct Token token = {TOKEN_ERROR, "", line, *col};
    int input_len = (int)strlen(input);
    
    if (*pos < 0 || *pos >= input_len) {
        token.type = TOKEN_EOF;
        strcpy(token.value, "");
        return token;
    }
    
    while (*pos < input_len && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            line++;
            *col = 1;
        } else {
            (*col)++;
        }
        (*pos)++;
    }
    
    if (*pos >= input_len) {
        token.type = TOKEN_EOF;
        strcpy(token.value, "");
        return token;
    }
    
    token.line = line;
    token.column = *col;
    
    char current = input[*pos];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int value_len = 0;
        while (*pos < input_len && (isdigit(input[*pos]) || input[*pos] == '.')) {
            if (value_len < 63) {
                token.value[value_len++] = input[*pos];
            }
            (*pos)++;
            (*col)++;
        }
        token.value[value_len] = '\0';
        return token;
    }
    
    if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        int id_len = 0;
        while (*pos < input_len && (isalnum(input[*pos]) || input[*pos] == '_')) {
            if (id_len < 63) {
                token.value[id_len++] = input[*pos];
            }
            (*pos)++;
            (*col)++;
        }
        token.value[id_len] = '\0';
        return token;
    }
    
    if (current == '"') {
        token.type = TOKEN_STRING;
        int str_len = 0;
        (*pos)++;
        (*col)++;
        
        while (*pos < input_len && input[*pos] != '"') {
            if (str_len < 63) {
                token.value[str_len++] = input[*pos];
            }
            (*pos)++;
            (*col)++;
        }
        
        if (*pos < input_len && input[*pos] == '"') {
            (*pos)++;
            (*col)++;
        }
        token.value[str_len] = '\0';
        return token;
    }
    
    char op[3];
    int op_len = parse_operator(input, *pos, op);
    if (op_len > 0) {
        token.type = TOKEN_OPERATOR;
        strncpy(token.value, op, 63);
        token.value[63] = '\0';
        *pos += op_len;
        *col += op_len;
        return token;
    }
    
    token.type = TOKEN_ERROR;
    token.value[0] = current;
    token.value[1] = '\0';
    (*pos)++;
    (*col)++;
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    printf("Line %d, Col %d: %s", token.line, token.column, type_names[token.type]);
    if (token.type != TOKEN_EOF) {
        printf(" '%s'", token.value);
    }
    printf("\n");
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize (max 1023 chars): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(st