//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

typedef void (*string_operation_t)(char *str, size_t len);

void to_uppercase(char *str, size_t len) {
    if (str == NULL) return;
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        str[i] = (char)toupper((unsigned char)str[i]);
    }
}

void to_lowercase(char *str, size_t len) {
    if (str == NULL) return;
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}

void reverse_string(char *str, size_t len) {
    if (str == NULL || len == 0) return;
    size_t actual_len = 0;
    while (actual_len < len && str[actual_len] != '\0') {
        actual_len++;
    }
    for (size_t i = 0; i < actual_len / 2; i++) {
        char temp = str[i];
        str[i] = str[actual_len - 1 - i];
        str[actual_len - 1 - i] = temp;
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
    if (write_pos < len) {
        str[write_pos] = '\0';
    }
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

void process_string_with_function(char *input, string_operation_t operation) {
    if (input == NULL || operation == NULL) return;
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    operation(input, len);
}

int main(void) {
    char input[256];
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
    
    printf("Enter a string (max 255 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    printf("\nAvailable operations:\n");
    for (int i = 0; i < num_operations; i++) {
        printf("%d. %s\n", i + 1, operation_names[i]);
    }
    
    printf("Select operation (1-%d): ", num_operations);
    int choice = get_valid_choice();
    
    if (choice < 1 || choice > num_operations) {
        printf("Invalid choice.\n");
        return 1;
    }
    
    char original[256];
    strncpy(original, input, sizeof(original) - 1);
    original[sizeof(original) - 1] = '\0';
    
    process_string_with_function(input, operations[choice - 1]);
    
    printf("\nOriginal: %s\n", original);
    printf("Result: %s\n", input);
    
    return 0;
}