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

char* remove_non_alpha(char* str) {
    if (str == NULL) return NULL;
    char* read = str;
    char* write = str;
    while (*read) {
        if (isalpha((unsigned char)*read)) {
            *write = *read;
            write++;
        }
        read++;
    }
    *write = '\0';
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
    char input[5][256];
    char* strings[5];
    size_t valid_count = 0;
    
    printf("Enter up to 5 strings (max 255 chars each):\n");
    for (int i = 0; i < 5; i++) {
        printf("String %d: ", i + 1);
        if (fgets(input[i], sizeof(input[i]), stdin) == NULL) {
            break;
        }
        size_t len = strlen(input[i]);
        if (len > 0 && input[i][len - 1] == '\n') {
            input[i][len - 1] = '\0';
        }
        if (strlen(input[i]) > 0) {
            strings[valid_count] = input[i];
            valid_count++;
        }
    }
    
    if (valid_count == 0) {
        printf("No valid strings entered.\n");
        return 1;
    }
    
    string_processor processors[] = {to_uppercase, to_lowercase, reverse_string, remove_non_alpha};
    const char* processor_names[] = {"Uppercase", "Lowercase", "Reversed", "Alpha-only"};
    size_t num_processors = sizeof(processors) / sizeof(processors[0]);
    
    printf("\nProcessing results:\n");
    for (size_t i = 0; i < num_processors; i++) {
        printf("\n%s:\n", processor_names[i]);
        char temp_strings[5][256];
        for (size_t j = 0; j < valid_count; j++) {
            strncpy(temp_strings[j], strings[j], sizeof(temp_strings[j]) - 1);
            temp_strings[j][sizeof(temp_strings[j]) - 1] = '\0';
        }
        char* temp_ptrs[5];
        for (size_t j = 0; j < valid_count; j++) {
            temp_ptrs[j] = temp_strings[j];
        }
        process_string_array(temp_ptrs, valid_count, processors[i]);
        for (size_t j = 0; j < valid_count; j++) {
            printf("  %s\n", temp_ptrs[j]);
        }
    }
    
    return 0;
}