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
    char buffer[100];
    
    while (1) {
        printf("Choose operation (1-5):\n");
        printf("1. Convert to uppercase\n");
        printf("2. Convert to lowercase\n");
        printf("3. Reverse string\n");
        printf("4. Remove whitespace\n");
        printf("5. Exit\n");
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return 5;
        }
        
        if (sscanf(buffer, "%d", &choice) == 1 && choice >= 1 && choice <= 5) {
            return choice;
        }
        printf("Invalid choice. Please enter 1-5.\n");
    }
}

int main(void) {
    string_operation_t operations[] = {
        NULL,
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_whitespace
    };
    
    char input[1024];
    
    while (1) {
        int choice = get_valid_choice();
        
        if (choice == 5) {
            printf("Goodbye!\n");
            break;
        }
        
        printf("Enter a string (max 1023 characters): ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading input.\n");
            continue;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strlen(input) == 0) {
            printf("Empty string provided.\n");
            continue;
        }
        
        char working_copy[1024];
        if (strlen(input) >= sizeof(working_copy)) {
            printf("Input too long.\n");
            continue;
        }
        strcpy(working_copy, input);
        
        operations[choice](working_copy);
        
        printf("Original: \"%s\"\n", input);
        printf("Result:   \"%s\"\n\n", working_copy);
    }
    
    return 0;
}