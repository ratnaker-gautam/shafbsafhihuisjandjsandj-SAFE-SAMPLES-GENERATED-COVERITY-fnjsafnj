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
    char *tokens[MAX_TOKENS];
    int count;
} TokenArray;

void initDynamicString(DynamicString *str) {
    str->capacity = 16;
    str->data = malloc(str->capacity);
    if (str->data == NULL) {
        exit(EXIT_FAILURE);
    }
    str->data[0] = '\0';
    str->length = 0;
}

void appendToDynamicString(DynamicString *str, char c) {
    if (str->length + 1 >= str->capacity) {
        str->capacity *= 2;
        char *new_data = realloc(str->data, str->capacity);
        if (new_data == NULL) {
            free(str->data);
            exit(EXIT_FAILURE);
        }
        str->data = new_data;
    }
    str->data[str->length] = c;
    str->length++;
    str->data[str->length] = '\0';
}

void freeDynamicString(DynamicString *str) {
    free(str->data);
}

int isDelimiter(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int tokenizeString(const char *input, TokenArray *tokenArray) {
    if (input == NULL || tokenArray == NULL) {
        return -1;
    }

    tokenArray->count = 0;
    const char *ptr = input;
    
    while (*ptr != '\0' && tokenArray->count < MAX_TOKENS) {
        while (isDelimiter(*ptr)) {
            ptr++;
        }
        
        if (*ptr == '\0') {
            break;
        }
        
        const char *start = ptr;
        while (*ptr != '\0' && !isDelimiter(*ptr)) {
            ptr++;
        }
        
        size_t token_len = ptr - start;
        if (token_len >= MAX_TOKEN_LEN) {
            token_len = MAX_TOKEN_LEN - 1;
        }
        
        char *token = malloc(token_len + 1);
        if (token == NULL) {
            return -1;
        }
        
        strncpy(token, start, token_len);
        token[token_len] = '\0';
        tokenArray->tokens[tokenArray->count] = token;
        tokenArray->count++;
    }
    
    return 0;
}

void freeTokenArray(TokenArray *tokenArray) {
    for (int i = 0; i < tokenArray->count; i++) {
        free(tokenArray->tokens[i]);
    }
    tokenArray->count = 0;
}

int main() {
    DynamicString inputStr;
    initDynamicString(&inputStr);
    
    printf("Enter text to tokenize (max %d chars): ", (int)(inputStr.capacity - 1));
    
    int c;
    while ((c = getchar()) != EOF && c != '\n') {
        if (inputStr.length < inputStr.capacity - 1) {
            appendToDynamicString(&inputStr, (char)c);
        }
    }
    
    if (inputStr.length == 0) {
        printf("No input provided.\n");
        freeDynamicString(&inputStr);
        return EXIT_FAILURE;
    }
    
    TokenArray tokens;
    if (tokenizeString(inputStr.data, &tokens) != 0) {
        printf("Error during tokenization.\n");
        freeDynamicString(&inputStr);
        return EXIT_FAILURE;
    }
    
    printf("Found %d tokens:\n", tokens.count);
    for (int i = 0; i < tokens.count; i++) {
        printf("%d: '%s'\n", i + 1, tokens.tokens[i]);
    }
    
    freeTokenArray(&tokens);
    freeDynamicString(&inputStr);
    
    return EXIT_SUCCESS;
}