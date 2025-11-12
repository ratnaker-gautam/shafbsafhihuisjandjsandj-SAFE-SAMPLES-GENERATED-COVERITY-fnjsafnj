//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

typedef struct {
    char *data;
    size_t capacity;
    size_t length;
} DynamicString;

typedef struct {
    char tokens[MAX_TOKENS][MAX_TOKEN_LEN];
    int count;
} TokenArray;

void initDynamicString(DynamicString *str) {
    str->capacity = 16;
    str->length = 0;
    str->data = malloc(str->capacity);
    if (str->data) {
        str->data[0] = '\0';
    }
}

void appendToDynamicString(DynamicString *str, char c) {
    if (str->length + 1 >= str->capacity) {
        size_t new_capacity = str->capacity * 2;
        char *new_data = realloc(str->data, new_capacity);
        if (!new_data) return;
        str->data = new_data;
        str->capacity = new_capacity;
    }
    str->data[str->length++] = c;
    str->data[str->length] = '\0';
}

void freeDynamicString(DynamicString *str) {
    free(str->data);
}

int isDelimiter(char c) {
    return isspace(c) || c == ',' || c == ';' || c == '.' || c == '!' || c == '?';
}

int tokenizeString(const char *input, TokenArray *tokenArray) {
    if (!input || !tokenArray) return 0;
    
    tokenArray->count = 0;
    size_t input_len = strlen(input);
    if (input_len == 0) return 1;
    
    DynamicString currentToken;
    initDynamicString(&currentToken);
    if (!currentToken.data) return 0;
    
    for (size_t i = 0; i < input_len && tokenArray->count < MAX_TOKENS; i++) {
        if (isDelimiter(input[i])) {
            if (currentToken.length > 0) {
                if (currentToken.length < MAX_TOKEN_LEN) {
                    strncpy(tokenArray->tokens[tokenArray->count], currentToken.data, MAX_TOKEN_LEN - 1);
                    tokenArray->tokens[tokenArray->count][MAX_TOKEN_LEN - 1] = '\0';
                    tokenArray->count++;
                }
                currentToken.length = 0;
                currentToken.data[0] = '\0';
            }
        } else {
            appendToDynamicString(&currentToken, input[i]);
        }
    }
    
    if (currentToken.length > 0 && tokenArray->count < MAX_TOKENS) {
        if (currentToken.length < MAX_TOKEN_LEN) {
            strncpy(tokenArray->tokens[tokenArray->count], currentToken.data, MAX_TOKEN_LEN - 1);
            tokenArray->tokens[tokenArray->count][MAX_TOKEN_LEN - 1] = '\0';
            tokenArray->count++;
        }
    }
    
    freeDynamicString(&currentToken);
    return 1;
}

void printTokens(const TokenArray *tokenArray) {
    if (!tokenArray) return;
    
    printf("Found %d tokens:\n", tokenArray->count);
    for (int i = 0; i < tokenArray->count; i++) {
        printf("%d: '%s'\n", i + 1, tokenArray->tokens[i]);
    }
}

int main() {
    char input_buffer[1024];
    printf("Enter text to tokenize: ");
    
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    TokenArray tokens;
    if (!tokenizeString(input_buffer, &tokens)) {
        printf("Tokenization failed\n");
        return 1;
    }
    
    printTokens(&tokens);
    return 0;
}