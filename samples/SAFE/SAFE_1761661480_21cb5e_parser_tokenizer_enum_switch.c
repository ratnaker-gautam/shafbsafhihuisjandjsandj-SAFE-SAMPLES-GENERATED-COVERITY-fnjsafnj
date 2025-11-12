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

struct Tokenizer {
    const char *input;
    int position;
    int line;
    int column;
    int length;
};

void tokenizer_init(struct Tokenizer *t, const char *input) {
    if (t == NULL || input == NULL) return;
    t->input = input;
    t->position = 0;
    t->line = 1;
    t->column = 1;
    t->length = strlen(input);
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

struct Token tokenizer_next(struct Tokenizer *t) {
    struct Token token = {TOKEN_INVALID, "", t->line, t->column};
    
    if (t == NULL || t->input == NULL) {
        return token;
    }
    
    while (t->position < t->length && isspace(t->input[t->position])) {
        if (t->input[t->position] == '\n') {
            t->line++;
            t->column = 1;
        } else {
            t->column++;
        }
        t->position++;
    }
    
    if (t->position >= t->length) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = t->input[t->position];
    token.line = t->line;
    token.column = t->column;
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (t->position < t->length && i < 31 && isdigit(t->input[t->position])) {
            token.value[i++] = t->input[t->position++];
            t->column++;
        }
        token.value[i] = '\0';
    } else if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (t->position < t->length && i < 31 && 
               (isalnum(t->input[t->position]) || t->input[t->position] == '_')) {
            token.value[i++] = t->input[t->position++];
            t->column++;
        }
        token.value[i] = '\0';
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (t->position < t->length && i < 31 && is_operator_char(t->input[t->position])) {
            token.value[i++] = t->input[t->position++];
            t->column++;
        }
        token.value[i] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        int i = 0;
        t->position++;
        t->column++;
        while (t->position < t->length && i < 31 && t->input[t->position] != '"') {
            token.value[i++] = t->input[t->position++];
            t->column++;
        }
        if (t->position < t->length && t->input[t->position] == '"') {
            t->position++;
            t->column++;
        }
        token.value[i] = '\0';
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = current;
        token.value[1] = '\0';
        t->position++;
        t->column++;
    }
    
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "INVALID"
    };
    
    printf("Line %d, Column %d: %s", token.line, token.column, type_names[token.type]);
    if (token.type != TOKEN_EOF) {
        printf(" '%s'", token.value);
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
    
    struct Tokenizer tokenizer;
    tokenizer_init(&tokenizer, input);
    
    printf("Tokens:\n");
    struct Token token;
    int token_count = 0;
    
    do {
        token = tokenizer_next(&tokenizer);
        print_token(token);
        token_count++;
    } while (token.type != TOKEN_EOF && token_count < 100);
    
    return 0;
}