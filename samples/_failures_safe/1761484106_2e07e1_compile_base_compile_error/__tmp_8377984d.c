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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

struct Token tokenizer_next(struct Tokenizer *t) {
    struct Token token = {TOKEN_ERROR, "", t->line, t->column};
    
    if (t->input == NULL) {
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
        
        if (isdigit(current)) {
            int i = 0;
            while (isdigit(t->input[t->position]) && i < 31) {
                token.value[i++] = t->input[t->position++];
                t->column++;
            }
            token.value[i] = '\0';
            token.type = TOKEN_NUMBER;
            return token;
        }
        
        if (isalpha(current) || current == '_') {
            int i = 0;
            while ((isalnum(t->input[t->position]) || t->input[t->position] == '_') && i < 31) {
                token.value[i++] = t->input[t->position++];
                t->column++;
            }
            token.value[i] = '\0';
            token.type = TOKEN_IDENTIFIER;
            return token;
        }
        
        if (is_operator_char(current)) {
            int i = 0;
            while (is_operator_char(t->input[t->position]) && i < 31) {
                token.value[i++] = t->input[t->position++];
                t->column++;
            }
            token.value[i] = '\0';
            token.type = TOKEN_OPERATOR;
            return token;
        }
        
        if (current == '"') {
            int i = 0;
            token.value[i++] = t->input[t->position++];
            t->column++;
            
            while (t->input[t->position] != '"' && t->input[t->position] != '\0' && i < 31) {
                token.value[i++] = t->input[t->position++];
                t->column++;
            }
            
            if (t->input[t->position] == '"') {
                token.value[i++] = t->input[t->position++];
                t->column++;
                token.value[i] = '\0';
                token.type = TOKEN_STRING;
                return token;
            } else {
                token.type = TOKEN_ERROR;
                t->error_count++;
                return token;
            }
        }
        
        token.type = TOKEN_ERROR;
        t->error_count++;
        t->position++;
        t->column++;
        return token;
    }
    
    token.type = TOKEN_EOF;
    strncpy(token.value, "EOF", 31);
    token.value[31] = '\0';
    return token;
}

void print_token(const struct Token *token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    printf("Line %d, Column %d: %s", token->line, token->column, type_names[token->type]);
    if (token->type != TOKEN_EOF) {
        printf(" ('%s')", token->value);
    }
    printf("\n");
}

int main(void) {
    char input[1024];
    printf("Enter text to tokenize (max 1023 characters): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len <= 1) {
        printf("No input provided\n");
        return 1;
    }
    
    struct Tokenizer tokenizer;
    tokenizer_init(&tokenizer, input);
    
    printf("Tokens:\n");
    printf("-------\n");
    
    int token_count = 0;
    int max_tokens = 100;
    
    for (struct Token token = tokenizer_next(&tokenizer); 
         token.type != TOKEN_EOF && token_count < max_tokens; 
         token = tokenizer_next(&tokenizer)) {
        print_token(&token);
        token_count++;
        
        if (token.type == TOKEN_ERROR) {
            printf("