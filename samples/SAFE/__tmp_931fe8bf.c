//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef void (*string_processor)(char *);

void to_uppercase(char *str) {
    if (str == NULL) return;
    for (char *p = str; *p; p++) {
        *p = toupper((unsigned char)*p);
    }
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (char *p = str; *p; p++) {
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
    char *dest = str;
    for (char *src = str; *src; src++) {
        if (isalnum((unsigned char)*src)) {
            *dest++ = *src;
        }
    }
    *dest = '\0';
}

int get_valid_choice(void) {
    int choice;
    char buffer[100];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    if (sscanf(buffer, "%d", &choice) != 1) {
        return -1;
    }
    return choice;
}

void get_input_string(char *buffer, size_t size) {
    if (buffer == NULL || size == 0) return;
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
}

int main(void) {
    string_processor processors[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_non_alnum
    };
    
    const char *menu_items[] = {
        "Convert to uppercase",
        "Convert to lowercase", 
        "Reverse string",
        "Remove non-alphanumeric characters"
    };
    
    const int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    char input[256];
    
    while (1) {
        printf("Available string processing operations:\n");
        for (int i = 0; i < num_processors; i++) {
            printf("%d. %s\n", i + 1, menu_items[i]);
        }
        printf("0. Exit\n");
        printf("Enter your choice: ");
        
        int choice = get_valid_choice();
        if (choice == -1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (choice == 0) {
            break;
        }
        
        if (choice < 1 || choice > num_processors) {
            printf("Invalid choice. Please select between 1 and %d.\n", num_processors);
            continue;
        }
        
        printf("Enter a string to process: ");
        get_input_string(input, sizeof(input));
        
        if (strlen(input) == 0) {
            printf("Empty string provided.\n");
            continue;
        }
        
        char original[256];
        strncpy(original, input, sizeof(original) - 1);
        original[sizeof(original) - 1] = '\0';
        
        processors[choice - 1](input);
        
        printf("Original: %s\n", original);
        printf("Result: %s\n\n", input);
    }
    
    return 0;
}