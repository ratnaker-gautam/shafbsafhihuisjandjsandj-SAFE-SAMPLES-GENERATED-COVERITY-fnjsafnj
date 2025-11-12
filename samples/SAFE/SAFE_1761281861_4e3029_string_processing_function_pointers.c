//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef char* (*string_processor)(char*);

char* to_uppercase(char* str) {
    if (str == NULL) return NULL;
    for (char* p = str; *p; p++) {
        *p = toupper((unsigned char)*p);
    }
    return str;
}

char* to_lowercase(char* str) {
    if (str == NULL) return NULL;
    for (char* p = str; *p; p++) {
        *p = tolower((unsigned char)*p);
    }
    return str;
}

char* reverse_string(char* str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str);
    for (size_t i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
    return str;
}

char* remove_whitespace(char* str) {
    if (str == NULL) return NULL;
    char* dst = str;
    char* src = str;
    while (*src) {
        if (!isspace((unsigned char)*src)) {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
    return str;
}

void process_string_array(char** strings, size_t count, string_processor processor) {
    if (strings == NULL || processor == NULL) return;
    for (size_t i = 0; i < count; i++) {
        if (strings[i] != NULL) {
            processor(strings[i]);
        }
    }
}

int main() {
    const size_t max_strings = 5;
    const size_t max_length = 100;
    
    char input_buffer[max_strings][max_length];
    char* strings[max_strings];
    
    printf("Enter %zu strings (max %zu characters each):\n", max_strings, max_length - 1);
    for (size_t i = 0; i < max_strings; i++) {
        printf("String %zu: ", i + 1);
        if (fgets(input_buffer[i], max_length, stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            return 1;
        }
        
        size_t len = strlen(input_buffer[i]);
        if (len > 0 && input_buffer[i][len - 1] == '\n') {
            input_buffer[i][len - 1] = '\0';
        }
        
        strings[i] = input_buffer[i];
    }
    
    string_processor processors[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_whitespace
    };
    
    const char* processor_names[] = {
        "Uppercase",
        "Lowercase",
        "Reversed",
        "No whitespace"
    };
    
    size_t num_processors = sizeof(processors) / sizeof(processors[0]);
    
    printf("\nProcessing results:\n");
    for (size_t i = 0; i < num_processors; i++) {
        printf("\n--- %s ---\n", processor_names[i]);
        
        char temp_strings[max_strings][max_length];
        for (size_t j = 0; j < max_strings; j++) {
            strncpy(temp_strings[j], strings[j], max_length - 1);
            temp_strings[j][max_length - 1] = '\0';
        }
        
        char* temp_ptrs[max_strings];
        for (size_t j = 0; j < max_strings; j++) {
            temp_ptrs[j] = temp_strings[j];
        }
        
        process_string_array(temp_ptrs, max_strings, processors[i]);
        
        for (size_t j = 0; j < max_strings; j++) {
            printf("String %zu: %s\n", j + 1, temp_ptrs[j]);
        }
    }
    
    return 0;
}