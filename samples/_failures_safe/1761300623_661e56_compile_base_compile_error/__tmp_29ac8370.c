//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
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

struct Tokenizer {
    const char *input;
    int position;
    int line;
    int column;
    int error_count;
};

void tokenizer_init(struct Tokenizer *t, const char *input) {
    t->input = input;
    t->position = 0;
    t->line = 1;
    t->column = 1;
    t->error_count = 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || 
           c == '=' || c == '<' || c == '>' || c == '!';
}

void skip_whitespace(struct Tokenizer *t) {
    while (t->input[t->position] != '\0') {
        char c = t->input[t->position];
        if (c == ' ' || c == '\t') {
            t->position++;
            t->column++;
        } else if (c == '\n') {
            t->position++;
            t->line++;
            t->column = 1;
        } else {
            break;
        }
    }
}

struct Token next_token(struct Tokenizer *t) {
    struct Token token = {TOKEN_ERROR, "", t->line, t->column};
    
    skip_whitespace(t);
    
    if (t->input[t->position] == '\0') {
        token.type = TOKEN_EOF;
        strncpy(token.value, "EOF", sizeof(token.value) - 1);
        return token;
    }
    
    char current = t->input[t->position];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit(t->input[t->position]) && i < (int)sizeof(token.value) - 1) {
            token.value[i++] = t->input[t->position++];
            t->column++;
        }
        token.value[i] = '\0';
    } else if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum(t->input[t->position]) || t->input[t->position] == '_') && 
               i < (int)sizeof(token.value) - 1) {
            token.value[i++] = t->input[t->position++];
            t->column++;
        }
        token.value[i] = '\0';
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (is_operator_char(t->input[t->position]) && i < (int)sizeof(token.value) - 1) {
            token.value[i++] = t->input[t->position++];
            t->column++;
        }
        token.value[i] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        token.value[0] = '"';
        t->position++;
        t->column++;
        int i = 1;
        while (t->input[t->position] != '"' && t->input[t->position] != '\0' && 
               i < (int)sizeof(token.value) - 2) {
            token.value[i++] = t->input[t->position++];
            t->column++;
        }
        if (t->input[t->position] == '"') {
            token.value[i++] = '"';
            t->position++;
            t->column++;
            token.value[i] = '\0';
        } else {
            token.type = TOKEN_ERROR;
            strncpy(token.value, "UNTERMINATED_STRING", sizeof(token.value) - 1);
            t->error_count++;
        }
    } else {
        token.type = TOKEN_ERROR;
        token.value[0] = current;
        token.value[1] = '\0';
        t->position++;
        t->column++;
        t->error_count++;
    }
    
    return token;
}

const char *token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
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
    
    printf("\nTokens:\n");
    printf("Line Col Type        Value\n");
    printf("---- --- ----------  --------\n");
    
    struct