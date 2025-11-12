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
} DynamicBuffer;

typedef struct {
    char tokens[MAX_TOKENS][MAX_TOKEN_LEN];
    int count;
} TokenArray;

void initBuffer(DynamicBuffer *buf) {
    buf->capacity = 16;
    buf->data = malloc(buf->capacity);
    if (buf->data == NULL) {
        exit(EXIT_FAILURE);
    }
    buf->data[0] = '\0';
    buf->length = 0;
}

void appendToBuffer(DynamicBuffer *buf, char c) {
    if (buf->length + 1 >= buf->capacity) {
        size_t new_cap = buf->capacity * 2;
        char *new_data = realloc(buf->data, new_cap);
        if (new_data == NULL) {
            free(buf->data);
            exit(EXIT_FAILURE);
        }
        buf->data = new_data;
        buf->capacity = new_cap;
    }
    buf->data[buf->length++] = c;
    buf->data[buf->length] = '\0';
}

void freeBuffer(DynamicBuffer *buf) {
    free(buf->data);
}

int isDelimiter(char c) {
    return isspace(c) || c == ',' || c == ';' || c == '(' || c == ')' || c == '{' || c == '}' || c == '\0';
}

int tokenizeString(const char *input, TokenArray *tokenArr) {
    if (input == NULL || tokenArr == NULL) {
        return -1;
    }
    
    tokenArr->count = 0;
    const char *ptr = input;
    
    while (*ptr != '\0' && tokenArr->count < MAX_TOKENS) {
        while (isspace(*ptr)) {
            ptr++;
        }
        
        if (*ptr == '\0') {
            break;
        }
        
        if (isDelimiter(*ptr) && !isspace(*ptr)) {
            if (tokenArr->count < MAX_TOKENS) {
                tokenArr->tokens[tokenArr->count][0] = *ptr;
                tokenArr->tokens[tokenArr->count][1] = '\0';
                tokenArr->count++;
            }
            ptr++;
            continue;
        }
        
        const char *start = ptr;
        while (*ptr != '\0' && !isDelimiter(*ptr)) {
            ptr++;
        }
        
        size_t token_len = ptr - start;
        if (token_len >= MAX_TOKEN_LEN) {
            token_len = MAX_TOKEN_LEN - 1;
        }
        
        if (tokenArr->count < MAX_TOKENS) {
            strncpy(tokenArr->tokens[tokenArr->count], start, token_len);
            tokenArr->tokens[tokenArr->count][token_len] = '\0';
            tokenArr->count++;
        }
    }
    
    return tokenArr->count;
}

int main(void) {
    DynamicBuffer inputBuffer;
    initBuffer(&inputBuffer);
    
    printf("Enter text to tokenize: ");
    
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        if (inputBuffer.length < 1000) {
            appendToBuffer(&inputBuffer, (char)c);
        }
    }
    
    if (inputBuffer.length == 0) {
        printf("No input provided.\n");
        freeBuffer(&inputBuffer);
        return EXIT_FAILURE;
    }
    
    TokenArray tokens;
    int token_count = tokenizeString(inputBuffer.data, &tokens);
    
    if (token_count < 0) {
        printf("Tokenization failed.\n");
        freeBuffer(&inputBuffer);
        return EXIT_FAILURE;
    }
    
    printf("Tokens found: %d\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: '%s'\n", i + 1, tokens.tokens[i]);
    }
    
    freeBuffer(&inputBuffer);
    return EXIT_SUCCESS;
}