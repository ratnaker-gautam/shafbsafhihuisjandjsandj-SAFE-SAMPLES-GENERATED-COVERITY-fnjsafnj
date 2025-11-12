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

struct Token next_token(struct Tokenizer *t) {
    struct Token token = {TOKEN_ERROR, "", t->line, t->column};
    
    if (t->input == NULL || t->position < 0) {
        return token;
    }
    
    while (t->input[t->position] != '\0') {
        char current = t->input[t->position];
        
        if (isspace(current)) {
            if (current == '\n') {
                t->line++;
                t->column = 1;
            } else {
                t->column++;
            }
            t->position++;
            continue;
        }
        
        token.line = t->line;
        token.column = t->column;
        
        if (isdigit(current)) {
            token.type = TOKEN_NUMBER;
            int i = 0;
            while (isdigit(t->input[t->position]) && i < 31) {
                token.value[i++] = t->input[t->position++];
                t->column++;
            }
            token.value[i] = '\0';
            return token;
        }
        
        if (isalpha(current) || current == '_') {
            token.type = TOKEN_IDENTIFIER;
            int i = 0;
            while ((isalnum(t->input[t->position]) || t->input[t->position] == '_') && i < 31) {
                token.value[i++] = t->input[t->position++];
                t->column++;
            }
            token.value[i] = '\0';
            return token;
        }
        
        if (current == '"') {
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
                token.type = TOKEN_ERROR;
                t->error_count++;
            }
            token.value[i] = '\0';
            return token;
        }
        
        if (is_operator_char(current)) {
            token.type = TOKEN_OPERATOR;
            int i = 0;
            while (is_operator_char(t->input[t->position]) && i < 31) {
                token.value[i++] = t->input[t->position++];
                t->column++;
            }
            token.value[i] = '\0';
            return token;
        }
        
        token.type = TOKEN_ERROR;
        int i = 0;
        if (i < 31) {
            token.value[0] = current;
            token.value[1] = '\0';
        } else {
            token.value[0] = '\0';
        }
        t->position++;
        t->column++;
        t->error_count++;
        return token;
    }
    
    token.type = TOKEN_EOF;
    token.value[0] = '\0';
    return token;
}

const char* token_type_name(enum TokenType type) {
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
    printf("Enter text to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    struct Tokenizer tokenizer;
    tokenizer_init(&tokenizer, input);
    
    printf("Tokens:\n");
    printf("%-12s %-12s %-8s %-8s\n", "TYPE", "VALUE", "LINE", "COLUMN");
    printf("----------------------------------------\n");
    
    struct Token token;
    do {
        token = next_token(&tokenizer);
        
        printf("%-12s %-12s %-8d