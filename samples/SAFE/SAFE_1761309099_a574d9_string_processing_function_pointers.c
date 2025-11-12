//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

typedef char* (*string_processor)(char*);

char* to_uppercase(char* str) {
    if (str == NULL) return NULL;
    char* result = malloc(strlen(str) + 1);
    if (result == NULL) return NULL;
    strcpy(result, str);
    for (char* p = result; *p; p++) {
        *p = toupper((unsigned char)*p);
    }
    return result;
}

char* to_lowercase(char* str) {
    if (str == NULL) return NULL;
    char* result = malloc(strlen(str) + 1);
    if (result == NULL) return NULL;
    strcpy(result, str);
    for (char* p = result; *p; p++) {
        *p = tolower((unsigned char)*p);
    }
    return result;
}

char* reverse_string(char* str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str);
    char* result = malloc(len + 1);
    if (result == NULL) return NULL;
    for (size_t i = 0; i < len; i++) {
        result[i] = str[len - 1 - i];
    }
    result[len] = '\0';
    return result;
}

char* remove_spaces(char* str) {
    if (str == NULL) return NULL;
    char* result = malloc(strlen(str) + 1);
    if (result == NULL) return NULL;
    char* dest = result;
    for (char* src = str; *src; src++) {
        if (!isspace((unsigned char)*src)) {
            *dest++ = *src;
        }
    }
    *dest = '\0';
    return result;
}

void process_string(char* input, string_processor processor) {
    if (input == NULL || processor == NULL) return;
    char* result = processor(input);
    if (result != NULL) {
        printf("Result: %s\n", result);
        free(result);
    }
}

int main() {
    string_processor processors[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_spaces
    };
    const char* names[] = {
        "Uppercase",
        "Lowercase", 
        "Reversed",
        "Spaces removed"
    };
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    char input[MAX_INPUT_LEN];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input string\n");
        return 1;
    }
    
    printf("\nOriginal: %s\n\n", input);
    
    for (int i = 0; i < num_processors; i++) {
        printf("%s: ", names[i]);
        process_string(input, processors[i]);
    }
    
    return 0;
}