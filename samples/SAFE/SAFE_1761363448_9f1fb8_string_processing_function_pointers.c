//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef void (*string_processor)(char *);

void to_uppercase(char *str) {
    if (str == NULL) return;
    for (char *p = str; *p != '\0'; p++) {
        *p = toupper((unsigned char)*p);
    }
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (char *p = str; *p != '\0'; p++) {
        *p = tolower((unsigned char)*p);
    }
}

void reverse_string(char *str) {
    if (str == NULL) return;
    size_t len = strlen(str);
    for (size_t i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

void remove_non_alnum(char *str) {
    if (str == NULL) return;
    char *read = str;
    char *write = str;
    while (*read != '\0') {
        if (isalnum((unsigned char)*read)) {
            *write = *read;
            write++;
        }
        read++;
    }
    *write = '\0';
}

int process_string(char *input, string_processor processor) {
    if (input == NULL || processor == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 1023) return 0;
    processor(input);
    return 1;
}

int main() {
    char input[1024];
    string_processor processors[] = {to_uppercase, to_lowercase, reverse_string, remove_non_alnum};
    const char *names[] = {"Uppercase", "Lowercase", "Reversed", "Alphanumeric only"};
    int choice;
    
    printf("Enter a string (max 1023 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
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
    
    printf("Choose transformation:\n");
    printf("1. Uppercase\n");
    printf("2. Lowercase\n");
    printf("3. Reverse\n");
    printf("4. Remove non-alphanumeric\n");
    printf("Enter choice (1-4): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice < 1 || choice > 4) {
        printf("Invalid choice\n");
        return 1;
    }
    
    char buffer[1024];
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    if (process_string(buffer, processors[choice - 1])) {
        printf("Original: %s\n", input);
        printf("%s: %s\n", names[choice - 1], buffer);
    } else {
        printf("Processing failed\n");
        return 1;
    }
    
    return 0;
}