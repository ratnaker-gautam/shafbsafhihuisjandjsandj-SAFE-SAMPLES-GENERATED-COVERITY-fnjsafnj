//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_STRING, TOKEN_SYMBOL, TOKEN_EOF };

struct Token {
    enum TokenType type;
    char value[64];
    int line;
};

int is_symbol_char(char c) {
    return isalnum(c) || c == '_' || c == '-';
}

int tokenize(const char* input, struct Token* tokens, int max_tokens) {
    int pos = 0;
    int line = 1;
    int token_count = 0;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        while (isspace(input[pos])) {
            if (input[pos] == '\n') line++;
            pos++;
        }
        
        if (input[pos] == '\0') break;
        
        if (isdigit(input[pos])) {
            int start = pos;
            while (isdigit(input[pos])) pos++;
            
            if (token_count >= max_tokens) return -1;
            tokens[token_count].type = TOKEN_NUMBER;
            tokens[token_count].line = line;
            int len = pos - start;
            if (len >= 64) len = 63;
            strncpy(tokens[token_count].value, &input[start], len);
            tokens[token_count].value[len] = '\0';
            token_count++;
        }
        else if (input[pos] == '"') {
            pos++;
            int start = pos;
            while (input[pos] != '\0' && input[pos] != '"') {
                if (input[pos] == '\n') line++;
                pos++;
            }
            
            if (input[pos] != '"') return -1;
            
            if (token_count >= max_tokens) return -1;
            tokens[token_count].type = TOKEN_STRING;
            tokens[token_count].line = line;
            int len = pos - start;
            if (len >= 64) len = 63;
            strncpy(tokens[token_count].value, &input[start], len);
            tokens[token_count].value[len] = '\0';
            token_count++;
            pos++;
        }
        else if (is_symbol_char(input[pos])) {
            int start = pos;
            while (is_symbol_char(input[pos])) pos++;
            
            if (token_count >= max_tokens) return -1;
            tokens[token_count].type = TOKEN_SYMBOL;
            tokens[token_count].line = line;
            int len = pos - start;
            if (len >= 64) len = 63;
            strncpy(tokens[token_count].value, &input[start], len);
            tokens[token_count].value[len] = '\0';
            token_count++;
        }
        else {
            return -1;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        tokens[token_count].line = line;
        token_count++;
    }
    
    return token_count;
}

void print_token(struct Token token) {
    const char* type_names[] = {"NUMBER", "STRING", "SYMBOL", "EOF"};
    printf("Line %d: %s", token.line, type_names[token.type]);
    if (token.type != TOKEN_EOF) {
        printf(" '%s'", token.value);
    }
    printf("\n");
}

int main(void) {
    char input[1024];
    printf("Enter text to tokenize (max %zu chars):\n", sizeof(input) - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    struct Token tokens[100];
    int token_count = tokenize(input, tokens, 100);
    
    if (token_count < 0) {
        fprintf(stderr, "Error: Invalid input or too many tokens\n");
        return 1;
    }
    
    printf("\nTokens found:\n");
    for (int i = 0; i < token_count; i++) {
        print_token(tokens[i]);
    }
    
    return 0;
}