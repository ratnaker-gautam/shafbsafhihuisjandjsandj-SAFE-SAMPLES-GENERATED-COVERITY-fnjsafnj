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

void initDynamicString(DynamicString *ds, size_t initial_capacity) {
    if (initial_capacity == 0) initial_capacity = 16;
    ds->data = malloc(initial_capacity);
    if (ds->data == NULL) exit(EXIT_FAILURE);
    ds->capacity = initial_capacity;
    ds->length = 0;
    ds->data[0] = '\0';
}

void appendToDynamicString(DynamicString *ds, char c) {
    if (ds->length + 1 >= ds->capacity) {
        size_t new_capacity = ds->capacity * 2;
        char *new_data = realloc(ds->data, new_capacity);
        if (new_data == NULL) {
            free(ds->data);
            exit(EXIT_FAILURE);
        }
        ds->data = new_data;
        ds->capacity = new_capacity;
    }
    ds->data[ds->length++] = c;
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

int tokenizeString(const char *input, TokenArray *token_array) {
    if (input == NULL || token_array == NULL) return 0;
    
    token_array->count = 0;
    const char *ptr = input;
    
    while (*ptr != '\0' && token_array->count < MAX_TOKENS) {
        while (*ptr != '\0' && isDelimiter(*ptr)) ptr++;
        if (*ptr == '\0') break;
        
        const char *start = ptr;
        while (*ptr != '\0' && !isDelimiter(*ptr)) ptr++;
        
        size_t token_len = ptr - start;
        if (token_len >= MAX_TOKEN_LEN) token_len = MAX_TOKEN_LEN - 1;
        
        char *token = malloc(token_len + 1);
        if (token == NULL) return 0;
        
        strncpy(token, start, token_len);
        token[token_len] = '\0';
        token_array->tokens[token_array->count++] = token;
    }
    
    return 1;
}

void freeTokenArray(TokenArray *token_array) {
    for (int i = 0; i < token_array->count; i++) {
        free(token_array->tokens[i]);
    }
    token_array->count = 0;
}

int main() {
    char input_buffer[1024];
    DynamicString input_string;
    TokenArray tokens;
    
    printf("Enter text to tokenize: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        printf("No input provided\n");
        return EXIT_FAILURE;
    }
    
    initDynamicString(&input_string, 64);
    
    const char *ptr = input_buffer;
    while (*ptr != '\0') {
        appendToDynamicString(&input_string, *ptr);
        ptr++;
    }
    
    if (!tokenizeString(input_string.data, &tokens)) {
        printf("Tokenization failed\n");
        freeDynamicString(&input_string);
        return EXIT_FAILURE;
    }
    
    printf("Tokens found: %d\n", tokens.count);
    for (int i = 0; i < tokens.count; i++) {
        printf("Token %d: '%s'\n", i + 1, tokens.tokens[i]);
    }
    
    freeTokenArray(&tokens);
    freeDynamicString(&input_string);
    
    return EXIT_SUCCESS;
}