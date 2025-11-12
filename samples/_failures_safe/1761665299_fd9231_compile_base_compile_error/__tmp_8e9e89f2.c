//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
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
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_operator(const char *input, int pos, char *output) {
    int len = 0;
    char c = input[pos];
    
    if (is_operator_char(c)) {
        output[len++] = c;
        if (is_operator_char(input[pos + 1])) {
            output[len++] = input[pos + 1];
        }
        output[len] = '\0';
    }
    
    return len;
}

int parse_number(const char *input, int pos, char *output) {
    int len = 0;
    int has_dot = 0;
    
    while (isdigit(input[pos]) || (input[pos] == '.' && !has_dot)) {
        if (input[pos] == '.') has_dot = 1;
        if (len < 31) {
            output[len++] = input[pos++];
        } else {
            break;
        }
    }
    output[len] = '\0';
    
    return len;
}

int parse_identifier(const char *input, int pos, char *output) {
    int len = 0;
    
    if (isalpha(input[pos]) || input[pos] == '_') {
        if (len < 31) {
            output[len++] = input[pos++];
        }
        while ((isalnum(input[pos]) || input[pos] == '_') && len < 31) {
            output[len++] = input[pos++];
        }
    }
    output[len] = '\0';
    
    return len;
}

int parse_string(const char *input, int pos, char *output) {
    int len = 0;
    
    if (input[pos] == '"') {
        if (len < 31) {
            output[len++] = input[pos++];
        }
        while (input[pos] != '\0' && input[pos] != '"' && len < 31) {
            output[len++] = input[pos++];
        }
        if (input[pos] == '"' && len < 31) {
            output[len++] = input[pos];
        }
        output[len] = '\0';
    }
    
    return len;
}

struct Token get_next_token(const char *input, int *pos, int line, int *column) {
    struct Token token = {TOKEN_INVALID, "", line, *column};
    
    while (input[*pos] != '\0' && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            line++;
            *column = 1;
        } else {
            (*column)++;
        }
        (*pos)++;
    }
    
    if (input[*pos] == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    token.line = line;
    token.column = *column;
    
    int len;
    
    if ((len = parse_number(input, *pos, token.value)) > 0) {
        token.type = TOKEN_NUMBER;
    } else if ((len = parse_identifier(input, *pos, token.value)) > 0) {
        token.type = TOKEN_IDENTIFIER;
    } else if ((len = parse_operator(input, *pos, token.value)) > 0) {
        token.type = TOKEN_OPERATOR;
    } else if ((len = parse_string(input, *pos, token.value)) > 0) {
        token.type = TOKEN_STRING;
    } else {
        if (len < 31) {
            token.value[0] = input[*pos];
            token.value[1] = '\0';
            len = 1;
        } else {
            len = 0;
        }
    }
    
    *pos += len;
    *column += len;
    
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "INVALID"
    };
    
    printf("Line %d, Column %d: %s", token.line, token.column, type_names[token.type]);
    if (token.type != TOKEN_EOF) {
        printf(" ('%s')", token.value);
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
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len <= 1) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int pos = 0;
    int line = 1;
    int column = 1