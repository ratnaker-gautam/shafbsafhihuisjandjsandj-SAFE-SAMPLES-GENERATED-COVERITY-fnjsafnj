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
    return c == '+' || c == '-' || c == '*' || c == '/' || 
           c == '=' || c == '<' || c == '>' || c == '!';
}

struct Token get_next_token(struct Tokenizer *t) {
    struct Token token = {TOKEN_ERROR, "", t->line, t->column};
    
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
        strncpy(token.value, &current, 1);
        token.value[1] = '\0';
        t->position++;
        t->column++;
        t->error_count++;
        return token;
    }
    
    token.type = TOKEN_EOF;
    strcpy(token.value, "EOF");
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
    char input_buffer[1024];
    
    printf("Enter text to tokenize (max 1023 characters): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (len == 1) {
        printf("No input provided\n");
        return 0;
    }
    
    struct Tokenizer tokenizer;
    tokenizer_init(&tokenizer, input_buffer);
    
    printf("\nTokenizing input:\n");
    printf("%-12s %-15s %-6s %-6s\n", "TYPE", "VALUE", "LINE", "COL");
    printf("----------------------------------------\n");
    
    int token_count = 0;
    struct Token token;
    
    do {
        token = get_next_token(&tokenizer);
        
        printf("%-12s %-15s %-6d %-6d\n",