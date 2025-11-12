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
    
    printf("\nChoose operation:\n");
    printf("1. Convert to uppercase\n");
    printf("2. Convert to lowercase\n");
    printf("3. Reverse strings\n");
    printf("4. Remove whitespace\n");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    string_processor processor = NULL;
    switch (choice) {
        case 1: processor = to_uppercase; break;
        case 2: processor = to_lowercase; break;
        case 3: processor = reverse_string; break;
        case 4: processor = remove_whitespace; break;
        default:
            printf("Invalid choice.\n");
            return 1;
    }
    
    process_string_array(strings, valid_count, processor);
    
    printf("\nProcessed strings:\n");
    for (size_t i = 0; i < valid_count; i++) {
        printf("%s\n", strings[i]);
    }
    
    return 0;
}