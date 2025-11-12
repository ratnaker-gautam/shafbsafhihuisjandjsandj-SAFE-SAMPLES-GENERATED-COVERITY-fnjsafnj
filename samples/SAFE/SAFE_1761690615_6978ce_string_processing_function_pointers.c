//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

typedef void (*string_operation_t)(char *str, size_t len);

void to_uppercase(char *str, size_t len) {
    if (str == NULL) return;
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void to_lowercase(char *str, size_t len) {
    if (str == NULL) return;
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void reverse_string(char *str, size_t len) {
    if (str == NULL || len == 0) return;
    size_t i = 0;
    size_t j = len - 1;
    while (i < j) {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

void remove_non_alpha(char *str, size_t len) {
    if (str == NULL) return;
    size_t write_pos = 0;
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        if (isalpha((unsigned char)str[i])) {
            str[write_pos++] = str[i];
        }
    }
    str[write_pos] = '\0';
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
    if (fgets(buffer, (int)size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
    }
}

int main(void) {
    string_operation_t operations[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_non_alpha
    };
    
    const char *operation_names[] = {
        "Convert to uppercase",
        "Convert to lowercase", 
        "Reverse string",
        "Remove non-alphabetic characters"
    };
    
    const int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    char input[256];
    
    printf("Available string operations:\n");
    for (int i = 0; i < num_operations; i++) {
        printf("%d. %s\n", i + 1, operation_names[i]);
    }
    
    printf("Select operation (1-%d): ", num_operations);
    int choice = get_valid_choice();
    
    if (choice < 1 || choice > num_operations) {
        printf("Invalid choice.\n");
        return 1;
    }
    
    printf("Enter a string (max 255 characters): ");
    get_input_string(input, sizeof(input));
    
    if (strlen(input) == 0) {
        printf("Empty string provided.\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len >= sizeof(input)) {
        printf("Input too long.\n");
        return 1;
    }
    
    char working_copy[256];
    if (strlen(input) >= sizeof(working_copy)) {
        printf("Input too long for processing.\n");
        return 1;
    }
    strncpy(working_copy, input, sizeof(working_copy) - 1);
    working_copy[sizeof(working_copy) - 1] = '\0';
    
    operations[choice - 1](working_copy, sizeof(working_copy));
    
    printf("Original: %s\n", input);
    printf("Result: %s\n", working_copy);
    
    return 0;
}