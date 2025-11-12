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
    char buffer[100];
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    
    if (sscanf(buffer, "%d", &choice) != 1) {
        return -1;
    }
    
    if (choice < 1 || choice > 5) {
        return -1;
    }
    
    return choice;
}

int main(void) {
    string_operation_t operations[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_whitespace,
        NULL
    };
    
    const char *operation_names[] = {
        "Convert to uppercase",
        "Convert to lowercase", 
        "Reverse string",
        "Remove whitespace",
        "Exit"
    };
    
    char input[1024];
    
    while (1) {
        printf("\nAvailable string operations:\n");
        for (int i = 0; i < 5; i++) {
            printf("%d. %s\n", i + 1, operation_names[i]);
        }
        printf("Select operation (1-5): ");
        
        int choice = get_valid_choice();
        if (choice == -1) {
            printf("Invalid choice. Please enter a number between 1 and 5.\n");
            continue;
        }
        
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
        
        operations[choice - 1](working_copy);
        
        printf("Original: \"%s\"\n", input);
        printf("Result:   \"%s\"\n", working_copy);
    }
    
    return 0;
}