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
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int tokenizeString(const char *input, TokenArray *token_array) {
    if (input == NULL || token_array == NULL) {
        return -1;
    }

    token_array->count = 0;
    const char *ptr = input;
    DynamicString current_token;
    initDynamicString(&current_token, 16);

    while (*ptr != '\0' && token_array->count < MAX_TOKENS) {
        if (isDelimiter(*ptr)) {
            if (current_token.length > 0) {
                token_array->tokens[token_array->count] = current_token.data;
                token_array->count++;
                initDynamicString(&current_token, 16);
            }
        } else {
            appendToDynamicString(&current_token, *ptr);
        }
        ptr++;
    }

    if (current_token.length > 0 && token_array->count < MAX_TOKENS) {
        token_array->tokens[token_array->count] = current_token.data;
        token_array->count++;
    } else {
        freeDynamicString(&current_token);
    }

    return token_array->count;
}

void freeTokenArray(TokenArray *token_array) {
    for (int i = 0; i < token_array->count; i++) {
        free(token_array->tokens[i]);
    }
    token_array->count = 0;
}

int main() {
    char input_buffer[1024];
    printf("Enter a string to tokenize: ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }

    if (strlen(input_buffer) == 0) {
        printf("Empty input string\n");
        return 0;
    }

    TokenArray tokens;
    int token_count = tokenizeString(input_buffer, &tokens);

    if (token_count < 0) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }

    printf("Found %d tokens:\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("%d: '%s'\n", i + 1, tokens.tokens[i]);
    }

    freeTokenArray(&tokens);
    return 0;
}