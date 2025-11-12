//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_STRING, TOKEN_SYMBOL, TOKEN_EOF };

struct Token {
    enum TokenType type;
    char value[64];
};

int is_symbol_char(char c) {
    return isalnum(c) || c == '_' || c == '-';
}

int tokenize(const char* input, struct Token* tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return -1;
    
    int token_count = 0;
    const char* p = input;
    
    while (*p && token_count < max_tokens) {
        while (isspace(*p)) p++;
        if (!*p) break;
        
        if (isdigit(*p)) {
            int i = 0;
            while (isdigit(*p) && i < 63) {
                tokens[token_count].value[i++] = *p++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        } else if (*p == '"') {
            p++;
            int i = 0;
            while (*p && *p != '"' && i < 63) {
                tokens[token_count].value[i++] = *p++;
            }
            if (*p == '"') p++;
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
        } else if (is_symbol_char(*p)) {
            int i = 0;
            while (is_symbol_char(*p) && i < 63) {
                tokens[token_count].value[i++] = *p++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_SYMBOL;
            token_count++;
        } else {
            tokens[token_count].value[0] = *p++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_SYMBOL;
            token_count++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

void print_token(struct Token token) {
    const char* type_names[] = {"NUMBER", "STRING", "SYMBOL", "EOF"};
    printf("Type: %-8s Value: '%s'\n", type_names[token.type], token.value);
}

int main(void) {
    char input[1024];
    printf("Enter text to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    struct Token tokens[100];
    int count = tokenize(input, tokens, 100);
    
    if (count < 0) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    printf("\nTokens found:\n");
    for (int i = 0; i < count; i++) {
        if (tokens[i].type == TOKEN_EOF) break;
        printf("%2d: ", i + 1);
        print_token(tokens[i]);
    }
    
    return 0;
}