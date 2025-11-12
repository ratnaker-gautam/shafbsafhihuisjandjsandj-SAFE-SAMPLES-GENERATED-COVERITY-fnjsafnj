//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_STRING, TOKEN_SYMBOL, TOKEN_EOF };

struct Token {
    enum TokenType type;
    char value[32];
};

int is_symbol_char(char c) {
    return isalnum(c) || c == '_' || c == '-';
}

int tokenize(const char* input, struct Token* tokens, int max_tokens) {
    int token_count = 0;
    const char* ptr = input;
    
    while (*ptr && token_count < max_tokens) {
        while (isspace(*ptr)) ptr++;
        
        if (!*ptr) break;
        
        if (isdigit(*ptr)) {
            int i = 0;
            while (isdigit(*ptr) && i < 31) {
                tokens[token_count].value[i++] = *ptr++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        }
        else if (*ptr == '"') {
            ptr++;
            int i = 0;
            while (*ptr != '"' && *ptr != '\0' && i < 31) {
                tokens[token_count].value[i++] = *ptr++;
            }
            if (*ptr == '"') ptr++;
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
        }
        else if (is_symbol_char(*ptr)) {
            int i = 0;
            while (is_symbol_char(*ptr) && i < 31) {
                tokens[token_count].value[i++] = *ptr++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_SYMBOL;
            token_count++;
        }
        else {
            ptr++;
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
    printf("Type: %-8s Value: %s\n", type_names[token.type], token.value);
}

int main(void) {
    char input[1024];
    struct Token tokens[100];
    
    printf("Enter text to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, 100);
    
    if (token_count == 0) {
        printf("No tokens found\n");
        return 1;
    }
    
    printf("\nTokens found:\n");
    for (int i = 0; i < token_count; i++) {
        printf("%2d: ", i + 1);
        print_token(tokens[i]);
    }
    
    return 0;
}