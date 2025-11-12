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
    int length;
};

void tokenizer_init(struct Tokenizer *t, const char *input) {
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
    
    while (t->position < t->length) {
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
            while (t->position < t->length && i < 31 && isdigit(t->input[t->position])) {
                token.value[i++] = t->input[t->position++];
                t->column++;
            }
            token.value[i] = '\0';
            token.type = TOKEN_NUMBER;
            return token;
        }
        
        if (isalpha(current) || current == '_') {
            int i = 0;
            while (t->position < t->length && i < 31 && 
                   (isalnum(t->input[t->position]) || t->input[t->position] == '_')) {
                token.value[i++] = t->input[t->position++];
                t->column++;
            }
            token.value[i] = '\0';
            token.type = TOKEN_IDENTIFIER;
            return token;
        }
        
        if (current == '"') {
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
            token.type = TOKEN_STRING;
            return token;
        }
        
        if (is_operator_char(current)) {
            int i = 0;
            while (t->position < t->length && i < 31 && is_operator_char(t->input[t->position])) {
                token.value[i++] = t->input[t->position++];
                t->column++;
            }
            token.value[i] = '\0';
            token.type = TOKEN_OPERATOR;
            return token;
        }
        
        token.value[0] = current;
        token.value[1] = '\0';
        t->position++;
        t->column++;
        token.type = TOKEN_INVALID;
        return token;
    }
    
    token.type = TOKEN_EOF;
    return token;
}

void print_token(const struct Token *token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "INVALID"
    };
    printf("Line %d, Column %d: %s", token->line, token->column, type_names[token->type]);
    if (token->type != TOKEN_EOF) {
        printf(" ('%s')", token->value);
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
    
    if (len == 1) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    struct Tokenizer tokenizer;
    tokenizer_init(&tokenizer, input);
    
    printf("Tokens:\n");
    
    struct Token token;
    do {
        token = tokenizer_next(&tokenizer);
        print_token(&token);
    } while (token.type != TOKEN_EOF);
    
    return 0;
}