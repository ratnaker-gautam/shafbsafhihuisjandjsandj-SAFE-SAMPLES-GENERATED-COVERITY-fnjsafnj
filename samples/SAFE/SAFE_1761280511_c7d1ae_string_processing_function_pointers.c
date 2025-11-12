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

void remove_whitespace(char *str, int len) {
    if (str == NULL || len <= 0) return;
    int write_pos = 0;
    for (int i = 0; i < len && str[i] != '\0'; i++) {
        if (!isspace((unsigned char)str[i])) {
            str[write_pos++] = str[i];
        }
    }
    if (write_pos < len) {
        str[write_pos] = '\0';
    }
}

int validate_input(const char *input, int max_len) {
    if (input == NULL || max_len <= 0) return 0;
    int len = 0;
    while (len < max_len && input[len] != '\0') {
        if (!isprint((unsigned char)input[len])) {
            return 0;
        }
        len++;
    }
    return len < max_len && input[len] == '\0';
}

void process_string(const char *input, string_operation_t operation, char *output, int output_size) {
    if (input == NULL || operation == NULL || output == NULL || output_size <= 0) return;
    
    int input_len = strlen(input);
    if (input_len >= output_size) return;
    
    strncpy(output, input, output_size - 1);
    output[output_size - 1] = '\0';
    
    operation(output, output_size - 1);
}

int main() {
    char input[256];
    char output[256];
    int choice;
    
    string_operation_t operations[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_whitespace
    };
    
    const char *operation_names[] = {
        "Convert to UPPERCASE",
        "Convert to lowercase",
        "Reverse string",
        "Remove whitespace"
    };
    
    const int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    printf("Enter a string (max 255 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    int len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(input, sizeof(input))) {
        printf("Invalid input: contains non-printable characters or too long\n");
        return 1;
    }
    
    printf("\nAvailable operations:\n");
    for (int i = 0; i < num_operations; i++) {
        printf("%d. %s\n", i + 1, operation_names[i]);
    }
    
    printf("Choose operation (1-%d): ", num_operations);
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice\n");
        return 1;
    }
    
    if (choice < 1 || choice > num_operations) {
        printf("Invalid choice\n");
        return 1;
    }
    
    process_string(input, operations[choice - 1], output, sizeof(output));
    
    printf("\nOriginal: %s\n", input);
    printf("Result: %s\n", output);
    
    return 0;
}