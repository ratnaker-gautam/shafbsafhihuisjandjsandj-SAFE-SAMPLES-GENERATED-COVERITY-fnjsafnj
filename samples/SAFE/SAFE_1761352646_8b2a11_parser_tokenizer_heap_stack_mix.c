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
    if (str->data == NULL) exit(1);
    str->data[0] = '\0';
    str->length = 0;
}

void appendChar(DynamicString *str, char c) {
    if (str->length + 1 >= str->capacity) {
        str->capacity *= 2;
        char *new_data = realloc(str->data, str->capacity);
        if (new_data == NULL) {
            free(str->data);
            exit(1);
        }
        str->data = new_data;
    }
    str->data[str->length] = c;
    str->length++;
    str->data[str->length] = '\0';
}

void freeString(DynamicString *str) {
    free(str->data);
}

int isDelimiter(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int tokenizeInput(const char *input, TokenArray *tokens) {
    tokens->count = 0;
    int i = 0;
    
    while (input[i] != '\0' && tokens->count < MAX_TOKENS) {
        while (isDelimiter(input[i])) i++;
        if (input[i] == '\0') break;
        
        int start = i;
        while (input[i] != '\0' && !isDelimiter(input[i])) {
            i++;
        }
        
        int length = i - start;
        if (length >= MAX_TOKEN_LEN) length = MAX_TOKEN_LEN - 1;
        
        if (length > 0) {
            strncpy(tokens->tokens[tokens->count], &input[start], length);
            tokens->tokens[tokens->count][length] = '\0';
            tokens->count++;
        }
    }
    
    return tokens->count;
}

void processTokens(TokenArray *tokens) {
    for (int i = 0; i < tokens->count; i++) {
        printf("Token %d: '%s' (length: %zu)\n", 
               i + 1, tokens->tokens[i], strlen(tokens->tokens[i]));
    }
}

int main() {
    DynamicString input;
    initString(&input);
    
    printf("Enter text to tokenize (max %d tokens): ", MAX_TOKENS);
    
    int c;
    while ((c = getchar()) != EOF && c != '\n') {
        appendChar(&input, (char)c);
    }
    
    if (input.length == 0) {
        printf("No input provided.\n");
        freeString(&input);
        return 1;
    }
    
    TokenArray tokens;
    int tokenCount = tokenizeInput(input.data, &tokens);
    
    if (tokenCount == 0) {
        printf("No tokens found in input.\n");
    } else {
        printf("Found %d tokens:\n", tokenCount);
        processTokens(&tokens);
    }
    
    freeString(&input);
    return 0;
}