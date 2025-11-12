//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef void (*string_processor)(char *str);

void to_uppercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void reverse_string(char *str) {
    if (str == NULL) return;
    size_t len = strlen(str);
    for (size_t i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

void remove_whitespace(char *str) {
    if (str == NULL) return;
    size_t j = 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (!isspace((unsigned char)str[i])) {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}

int get_valid_choice(void) {
    int choice;
    char input[16];
    
    while (1) {
        printf("Choose operation (1-5):\n");
        printf("1. Convert to uppercase\n");
        printf("2. Convert to lowercase\n");
        printf("3. Reverse string\n");
        printf("4. Remove whitespace\n");
        printf("5. Exit\n");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            return 5;
        }
        
        if (sscanf(input, "%d", &choice) == 1 && choice >= 1 && choice <= 5) {
            return choice;
        }
        
        printf("Invalid choice. Please enter 1-5.\n");
    }
}

int main(void) {
    string_processor processors[] = {
        NULL,
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_whitespace
    };
    
    char input[256];
    
    while (1) {
        int choice = get_valid_choice();
        
        if (choice == 5) {
            break;
        }
        
        printf("Enter a string (max 255 characters): ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strlen(input) == 0) {
            printf("Empty string provided.\n");
            continue;
        }
        
        char buffer[256];
        strncpy(buffer, input, sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';
        
        processors[choice](buffer);
        
        printf("Original: \"%s\"\n", input);
        printf("Result:   \"%s\"\n\n", buffer);
    }
    
    printf("Goodbye!\n");
    return 0;
}