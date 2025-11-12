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
    return strchr("+-*/=<>!&|", c) != NULL;
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

struct Token get_next_token(struct Tokenizer *t) {
    struct Token token = {TOKEN_INVALID, "", t->line, t->column};
    skip_whitespace(t);
    
    if (t->input[t->position] == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = t->input[t->position];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit(t->input[t->position]) && i < 31) {
            token.value[i++] = t->input[t->position++];
            t->column++;
        }
        token.value[i] = '\0';
    } else if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum(t->input[t->position]) || t->input[t->position] == '_') && i < 31) {
            token.value[i++] = t->input[t->position++];
            t->column++;
        }
        token.value[i] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        int i = 0;
        t->position++;
        t->column++;
        while (t->input[t->position] != '"' && t->input[t->position] != '\0' && i < 31) {
            token.value[i++] = t->input[t->position++];
            t->column++;
        }
        if (t->input[t->position] == '"') {
            t->position++;
            t->column++;
        } else {
            t->error_count++;
        }
        token.value[i] = '\0';
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (is_operator_char(t->input[t->position]) && i < 31) {
            token.value[i++] = t->input[t->position++];
            t->column++;
        }
        token.value[i] = '\0';
    } else {
        token.value[0] = t->input[t->position++];
        token.value[1] = '\0';
        t->column++;
        t->error_count++;
    }
    
    return token;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "INVALID";
    }
}

int main(void) {
    char input_buffer[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    struct Tokenizer tokenizer;
    tokenizer_init(&tokenizer, input_buffer);
    
    printf("Tokens:\n");
    printf("%-12s %-12s %-8s %-8s\n", "Type", "Value", "Line", "Column");
    printf("------------ ------------ -------- --------\n");
    
    struct Token token;
    int token_count = 0;
    
    do {
        token = get_next_token(&tokenizer);
        
        if (token.type != TOKEN_EOF) {
            printf("%-12s %-12s %-8d %-8d\n", 
                   token_type_to_string(token.type),
                   token.value,
                   token.line,
                   token.column);
            token_count++;
        }
        
        if (token.type == TOKEN_INVALID) {
            printf("Invalid token encountered\n");
        }