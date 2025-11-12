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

void initDynamicString(DynamicString *ds, size_t initial_capacity) {
    ds->data = malloc(initial_capacity);
    if (ds->data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    ds->capacity = initial_capacity;
    ds->length = 0;
    ds->data[0] = '\0';
}

void appendToDynamicString(DynamicString *ds, char c) {
    if (ds->length + 1 >= ds->capacity) {
        size_t new_capacity = ds->capacity * 2;
        char *new_data = realloc(ds->data, new_capacity);
        if (new_data == NULL) {
            fprintf(stderr, "Memory reallocation failed\n");
            free(ds->data);
            exit(1);
        }
        ds->data = new_data;
        ds->capacity = new_capacity;
    }
    ds->data[ds->length] = c;
    ds->length++;
    ds->data[ds->length] = '\0';
}

void freeDynamicString(DynamicString *ds) {
    free(ds->data);
    ds->data = NULL;
    ds->capacity = 0;
    ds->length = 0;
}

int isDelimiter(char c) {
    return isspace(c) || c == ',' || c == ';' || c == '.' || c == '!' || c == '?';
}

int tokenizeString(const char *input, TokenArray *tokenArray) {
    if (input == NULL || tokenArray == NULL) {
        return 0;
    }
    
    tokenArray->count = 0;
    size_t input_len = strlen(input);
    if (input_len == 0) {
        return 1;
    }
    
    DynamicString currentToken;
    initDynamicString(&currentToken, 16);
    
    for (size_t i = 0; i < input_len; i++) {
        if (isDelimiter(input[i])) {
            if (currentToken.length > 0) {
                if (tokenArray->count >= MAX_TOKENS) {
                    freeDynamicString(&currentToken);
                    return 0;
                }
                if (currentToken.length >= MAX_TOKEN_LEN) {
                    currentToken.length = MAX_TOKEN_LEN - 1;
                    currentToken.data[currentToken.length] = '\0';
                }
                strncpy(tokenArray->tokens[tokenArray->count], currentToken.data, MAX_TOKEN_LEN - 1);
                tokenArray->tokens[tokenArray->count][MAX_TOKEN_LEN - 1] = '\0';
                tokenArray->count++;
                currentToken.length = 0;
                currentToken.data[0] = '\0';
            }
        } else {
            appendToDynamicString(&currentToken, input[i]);
        }
    }
    
    if (currentToken.length > 0) {
        if (tokenArray->count >= MAX_TOKENS) {
            freeDynamicString(&currentToken);
            return 0;
        }
        if (currentToken.length >= MAX_TOKEN_LEN) {
            currentToken.length = MAX_TOKEN_LEN - 1;
            currentToken.data[currentToken.length] = '\0';
        }
        strncpy(tokenArray->tokens[tokenArray->count], currentToken.data, MAX_TOKEN_LEN - 1);
        tokenArray->tokens[tokenArray->count][MAX_TOKEN_LEN - 1] = '\0';
        tokenArray->count++;
    }
    
    freeDynamicString(&currentToken);
    return 1;
}

int main() {
    char input_buffer[1024];
    printf("Enter text to tokenize: ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
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
        fprintf(stderr, "Tokenization failed: too many tokens or memory error\n");
        return 1;
    }
    
    printf("Found %d tokens:\n", tokens.count);
    for (int i = 0; i < tokens.count; i++) {
        printf("%d: '%s'\n", i + 1, tokens.tokens[i]);
    }
    
    return 0;
}