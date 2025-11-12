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
    if (input == NULL || tokens == NULL || max_tokens <= 0) return -1;
    
    int token_count = 0;
    const char* ptr = input;
    
    while (*ptr != '\0' && token_count < max_tokens) {
        while (isspace(*ptr)) ptr++;
        if (*ptr == '\0') break;
        
        if (isdigit(*ptr)) {
            int i = 0;
            while (isdigit(*ptr) && i < 63) {
                tokens[token_count].value[i++] = *ptr++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        } else if (*ptr == '"') {
            ptr++;
            int i = 0;
            while (*ptr != '"' && *ptr != '\0' && i < 63) {
                tokens[token_count].value[i++] = *ptr++;
            }
            if (*ptr == '"') ptr++;
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
        } else if (is_symbol_char(*ptr)) {
            int i = 0;
            while (is_symbol_char(*ptr) && i < 63) {
                tokens[token_count].value[i++] = *ptr++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_SYMBOL;
            token_count++;
        } else {
            tokens[token_count].value[0] = *ptr++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_SYMBOL;
            token_count++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "");
        token_count++;
    }
    
    return token_count;
}

void print_token(struct Token token) {
    const char* type_names[] = {"NUMBER", "STRING", "SYMBOL", "EOF"};
    printf("%s: '%s'\n", type_names[token.type], token.value);
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
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    struct Token tokens[100];
    int count = tokenize(input, tokens, 100);
    
    if (count < 0) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    printf("Tokens:\n");
    for (int i = 0; i < count; i++) {
        print_token(tokens[i]);
    }
    
    return 0;
}