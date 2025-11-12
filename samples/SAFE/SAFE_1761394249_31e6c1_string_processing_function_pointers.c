//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef char* (*string_processor)(char*);

char* to_uppercase(char* str) {
    if (str == NULL) return NULL;
    for (char* p = str; *p != '\0'; p++) {
        *p = toupper((unsigned char)*p);
    }
    return str;
}

char* to_lowercase(char* str) {
    if (str == NULL) return NULL;
    for (char* p = str; *p != '\0'; p++) {
        *p = tolower((unsigned char)*p);
    }
    return str;
}

char* reverse_string(char* str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str);
    for (size_t i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
    return str;
}

char* remove_spaces(char* str) {
    if (str == NULL) return NULL;
    char* dst = str;
    char* src = str;
    while (*src != '\0') {
        if (!isspace((unsigned char)*src)) {
            *dst = *src;
            dst++;
        }
        src++;
    }
    *dst = '\0';
    return str;
}

void process_string(char* input, string_processor processor) {
    if (input == NULL || processor == NULL) {
        printf("Invalid input or processor\n");
        return;
    }
    
    char buffer[256];
    if (strlen(input) >= sizeof(buffer)) {
        printf("Input too long\n");
        return;
    }
    
    strcpy(buffer, input);
    char* result = processor(buffer);
    if (result != NULL) {
        printf("Result: %s\n", result);
    }
}

int main() {
    char input[256];
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
        "No spaces"
    };
    
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    printf("Enter a string (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty string\n");
        return 1;
    }
    
    printf("\nAvailable transformations:\n");
    for (int i = 0; i < num_processors; i++) {
        printf("%d. %s\n", i + 1, names[i]);
    }
    
    printf("Choose transformation (1-%d): ", num_processors);
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice < 1 || choice > num_processors) {
        printf("Invalid choice\n");
        return 1;
    }
    
    process_string(input, processors[choice - 1]);
    
    return 0;
}