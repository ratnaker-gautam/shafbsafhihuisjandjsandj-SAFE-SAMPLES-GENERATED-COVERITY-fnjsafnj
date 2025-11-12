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
    if (str->length + 2 > str->capacity) {
        str->capacity *= 2;
        char *new_data = realloc(str->data, str->capacity);
        if (new_data == NULL) {
            free(str->data);
            exit(1);
        }
        str->data = new_data;
    }
    str->data[str->length++] = c;
    str->data[str->length] = '\0';
}

void freeString(DynamicString *str) {
    free(str->data);
}

int isDelimiter(char c) {
    return isspace(c) || c == ',' || c == ';' || c == '(' || c == ')' || c == '{' || c == '}' || c == '\0';
}

int tokenizeInput(const char *input, TokenArray *tokenArr) {
    if (input == NULL || tokenArr == NULL) return 0;
    
    tokenArr->count = 0;
    int i = 0;
    
    while (input[i] != '\0' && tokenArr->count < MAX_TOKENS) {
        while (isDelimiter(input[i]) && input[i] != '\0') i++;
        if (input[i] == '\0') break;
        
        int start = i;
        while (!isDelimiter(input[i]) && input[i] != '\0') i++;
        int length = i - start;
        
        if (length > 0 && length < MAX_TOKEN_LEN) {
            strncpy(tokenArr->tokens[tokenArr->count], input + start, length);
            tokenArr->tokens[tokenArr->count][length] = '\0';
            tokenArr->count++;
        }
    }
    
    return tokenArr->count;
}

void processTokens(TokenArray *tokenArr) {
    if (tokenArr == NULL || tokenArr->count == 0) return;
    
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
        if (inputStr.length < inputStr.capacity - 2) {
            appendChar(&inputStr, (char)c);
        }
    }
    
    if (inputStr.length == 0) {
        printf("No input provided.\n");
        freeString(&inputStr);
        return 1;
    }
    
    TokenArray tokenArr;
    int tokenCount = tokenizeInput(inputStr.data, &tokenArr);
    
    if (tokenCount > 0) {
        processTokens(&tokenArr);
    } else {
        printf("No tokens found in input.\n");
    }
    
    freeString(&inputStr);
    return 0;
}