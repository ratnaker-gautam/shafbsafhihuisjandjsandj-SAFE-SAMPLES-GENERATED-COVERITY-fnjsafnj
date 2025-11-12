//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef void (*string_operation_t)(char *str, int len);

void to_uppercase(char *str, int len) {
    if (str == NULL || len <= 0) return;
    for (int i = 0; i < len && str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void to_lowercase(char *str, int len) {
    if (str == NULL || len <= 0) return;
    for (int i = 0; i < len && str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void reverse_string(char *str, int len) {
    if (str == NULL || len <= 0) return;
    int actual_len = 0;
    while (actual_len < len && str[actual_len] != '\0') {
        actual_len++;
    }
    for (int i = 0; i < actual_len / 2; i++) {
        char temp = str[i];
        str[i] = str[actual_len - 1 - i];
        str[actual_len - 1 - i] = temp;
    }
}

void remove_non_alpha(char *str, int len) {
    if (str == NULL || len <= 0) return;
    int write_pos = 0;
    for (int i = 0; i < len && str[i] != '\0'; i++) {
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
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    if (sscanf(input, "%d", &choice) != 1) {
        return -1;
    }
    return choice;
}

void process_string_with_function(char *str, string_operation_t operation) {
    if (str == NULL || operation == NULL) return;
    char buffer[256];
    if (strlen(str) >= sizeof(buffer)) {
        printf("Input string too long.\n");
        return;
    }
    strncpy(buffer, str, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    operation(buffer, sizeof(buffer));
    printf("Result: %s\n", buffer);
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
    
    printf("Enter a string to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    if (strlen(input) == 0) {
        printf("Empty string provided.\n");
        return 1;
    }
    
    printf("\nAvailable operations:\n");
    for (int i = 0; i < num_operations; i++) {
        printf("%d. %s\n", i + 1, operation_names[i]);
    }
    
    printf("\nSelect operation (1-%d): ", num_operations);
    int choice = get_valid_choice();
    if (choice < 1 || choice > num_operations) {
        printf("Invalid choice.\n");
        return 1;
    }
    
    process_string_with_function(input, operations[choice - 1]);
    
    return 0;
}