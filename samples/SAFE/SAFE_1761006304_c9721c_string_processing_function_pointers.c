//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

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

char* remove_vowels(char* str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str);
    char* result = malloc(len + 1);
    if (result == NULL) return NULL;
    
    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        char c = tolower((unsigned char)str[i]);
        if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u') {
            result[j++] = str[i];
        }
    }
    result[j] = '\0';
    return result;
}

void process_string_array(char** strings, size_t count, string_processor processor) {
    if (strings == NULL || processor == NULL) return;
    
    for (size_t i = 0; i < count; i++) {
        if (strings[i] != NULL) {
            char* processed = processor(strings[i]);
            if (processed != NULL) {
                printf("Original: %s\n", strings[i]);
                printf("Processed: %s\n\n", processed);
                free(processed);
            }
        }
    }
}

int main() {
    char* test_strings[] = {
        "Hello World",
        "Function Pointers",
        "C Programming",
        "Test String"
    };
    size_t count = sizeof(test_strings) / sizeof(test_strings[0]);
    
    string_processor processors[] = {
        to_uppercase,
        reverse_string,
        remove_vowels
    };
    size_t num_processors = sizeof(processors) / sizeof(processors[0]);
    
    for (size_t i = 0; i < num_processors; i++) {
        printf("Processing with function %zu:\n", i + 1);
        printf("============================\n");
        process_string_array(test_strings, count, processors[i]);
    }
    
    return 0;
}