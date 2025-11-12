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
    str->data = malloc(str->capacity);
    if (str->data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    str->data[0] = '\0';
    str->length = 0;
}

void appendToDynamicString(DynamicString *str, char c) {
    if (str->length + 1 >= str->capacity) {
        size_t new_capacity = str->capacity * 2;
        char *new_data = realloc(str->data, new_capacity);
        if (new_data == NULL) {
            fprintf(stderr, "Memory reallocation failed\n");
            free(str->data);
            exit(1);
        }
        str->data = new_data;
        str->capacity = new_capacity;
    }
    str->data[str->length] = c;
    str->length++;
    str->data[str->length] = '\0';
}

void freeDynamicString(DynamicString *str) {
    free(str->data);
    str->data = NULL;
    str->capacity = 0;
    str->length = 0;
}

int isDelimiter(char c) {
    return isspace(c) || c == ',' || c == ';' || c == '.' || c == '!' || c == '?';
}

int tokenizeString(const char *input, TokenArray *tokenArray) {
    if (input == NULL || tokenArray == NULL) {
        return 0;
    }

    tokenArray->count = 0;
    DynamicString currentToken;
    initDynamicString(&currentToken);

    for (size_t i = 0; input[i] != '\0'; i++) {
        if (isDelimiter(input[i])) {
            if (currentToken.length > 0) {
                if (tokenArray->count >= MAX_TOKENS) {
                    freeDynamicString(&currentToken);
                    return tokenArray->count;
                }
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
    return tokenArray->count;
}

int main() {
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

    if (len == 1 && input[0] == '\n') {
        printf("No input provided\n");
        return 1;
    }

    TokenArray tokens;
    int tokenCount = tokenizeString(input, &tokens);

    if (tokenCount == 0) {
        printf("No tokens found\n");
        return 0;
    }

    printf("Tokens found: %d\n", tokenCount);
    for (int i = 0; i < tokenCount; i++) {
        printf("Token %d: '%s'\n", i + 1, tokens.tokens[i]);
    }

    return 0;
}