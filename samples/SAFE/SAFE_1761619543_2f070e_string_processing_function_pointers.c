//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef void (*string_operation_t)(char *str);

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
    size_t write_pos = 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (!isspace((unsigned char)str[i])) {
            str[write_pos++] = str[i];
        }
    }
    str[write_pos] = '\0';
}

int get_valid_choice(void) {
    int choice;
    char input[16];
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    if (sscanf(input, "%d", &choice) != 1) {
        return -1;
    }
    
    if (choice < 1 || choice > 5) {
        return -1;
    }
    
    return choice;
}

void process_string_with_function(char *str, string_operation_t operation) {
    if (str == NULL || operation == NULL) return;
    char buffer[256];
    
    if (strlen(str) >= sizeof(buffer)) {
        printf("Input string too long\n");
        return;
    }
    
    strcpy(buffer, str);
    operation(buffer);
    printf("Result: %s\n", buffer);
}

int main(void) {
    char input[256];
    string_operation_t operations[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_whitespace,
        NULL
    };
    
    printf("Enter a string (max 255 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty string provided\n");
        return 1;
    }
    
    printf("\nChoose an operation:\n");
    printf("1. Convert to uppercase\n");
    printf("2. Convert to lowercase\n");
    printf("3. Reverse string\n");
    printf("4. Remove whitespace\n");
    printf("5. Exit\n");
    printf("Enter choice (1-5): ");
    
    int choice = get_valid_choice();
    if (choice == -1) {
        printf("Invalid choice\n");
        return 1;
    }
    
    if (choice == 5) {
        printf("Goodbye!\n");
        return 0;
    }
    
    process_string_with_function(input, operations[choice - 1]);
    
    return 0;
}