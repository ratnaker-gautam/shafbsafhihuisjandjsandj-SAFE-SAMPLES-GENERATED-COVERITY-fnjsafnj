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

void initString(DynamicString *str) {
    str->capacity = 16;
    str->data = malloc(str->capacity);
    if (str->data == NULL) exit(EXIT_FAILURE);
    str->data[0] = '\0';
    str->length = 0;
}

void appendChar(DynamicString *str, char c) {
    if (str->length + 2 > str->capacity) {
        size_t new_cap = str->capacity * 2;
        char *new_data = realloc(str->data, new_cap);
        if (new_data == NULL) {
            free(str->data);
            exit(EXIT_FAILURE);
        }
        str->data = new_data;
        str->capacity = new_cap;
    }
    str->data[str->length++] = c;
    str->data[str->length] = '\0';
}

void freeString(DynamicString *str) {
    free(str->data);
}

int isDelimiter(char c) {
    return isspace(c) || c == ',' || c == ';' || c == '.' || c == '!' || c == '?';
}

int tokenizeString(const char *input, TokenArray *tokenArr) {
    if (input == NULL || tokenArr == NULL) return 0;
    
    tokenArr->count = 0;
    const char *ptr = input;
    
    while (*ptr != '\0' && tokenArr->count < MAX_TOKENS) {
        while (*ptr != '\0' && isDelimiter(*ptr)) ptr++;
        
        if (*ptr == '\0') break;
        
        const char *start = ptr;
        while (*ptr != '\0' && !isDelimiter(*ptr)) ptr++;
        
        size_t token_len = ptr - start;
        if (token_len >= MAX_TOKEN_LEN) token_len = MAX_TOKEN_LEN - 1;
        
        if (token_len > 0) {
            strncpy(tokenArr->tokens[tokenArr->count], start, token_len);
            tokenArr->tokens[tokenArr->count][token_len] = '\0';
            tokenArr->count++;
        }
    }
    
    return tokenArr->count;
}

void printTokens(const TokenArray *tokenArr) {
    if (tokenArr == NULL) return;
    
    printf("Found %d tokens:\n", tokenArr->count);
    for (int i = 0; i < tokenArr->count; i++) {
        printf("%d: '%s'\n", i + 1, tokenArr->tokens[i]);
    }
}

int main() {
    DynamicString inputStr;
    initString(&inputStr);
    
    printf("Enter text to tokenize (max %d chars): ", (int)(inputStr.capacity - 1));
    
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        if (inputStr.length < inputStr.capacity - 1) {
            appendChar(&inputStr, (char)c);
        }
    }
    
    if (inputStr.length == 0) {
        printf("No input provided.\n");
        freeString(&inputStr);
        return EXIT_SUCCESS;
    }
    
    TokenArray tokens;
    int token_count = tokenizeString(inputStr.data, &tokens);
    
    if (token_count > 0) {
        printTokens(&tokens);
    } else {
        printf("No tokens found in input.\n");
    }
    
    freeString(&inputStr);
    return EXIT_SUCCESS;
}