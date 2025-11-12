//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

typedef void (*string_operation_t)(char *str, size_t len);

void to_uppercase(char *str, size_t len) {
    if (str == NULL || len == 0) return;
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        str[i] = (char)toupper((unsigned char)str[i]);
    }
}

void to_lowercase(char *str, size_t len) {
    if (str == NULL || len == 0) return;
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}

void reverse_string(char *str, size_t len) {
    if (str == NULL || len == 0) return;
    size_t actual_len = strnlen(str, len);
    for (size_t i = 0; i < actual_len / 2; i++) {
        char temp = str[i];
        str[i] = str[actual_len - 1 - i];
        str[actual_len - 1 - i] = temp;
    }
}

void remove_whitespace(char *str, size_t len) {
    if (str == NULL || len == 0) return;
    size_t write_pos = 0;
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        if (!isspace((unsigned char)str[i])) {
            str[write_pos++] = str[i];
        }
    }
    if (write_pos < len) {
        str[write_pos] = '\0';
    }
}

int perform_string_operation(const char *input, char *output, size_t output_size, string_operation_t operation) {
    if (input == NULL || output == NULL || output_size == 0 || operation == NULL) {
        return -1;
    }
    
    size_t input_len = strlen(input);
    if (input_len >= output_size) {
        return -1;
    }
    
    strncpy(output, input, output_size - 1);
    output[output_size - 1] = '\0';
    
    operation(output, output_size);
    
    return 0;
}

int main(void) {
    char input[256];
    char output[256];
    int choice;
    
    printf("Enter a string (max 255 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    printf("Choose operation:\n");
    printf("1. Convert to uppercase\n");
    printf("2. Convert to lowercase\n");
    printf("3. Reverse string\n");
    printf("4. Remove whitespace\n");
    printf("Enter choice (1-4): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    string_operation_t operation = NULL;
    
    switch (choice) {
        case 1: operation = to_uppercase; break;
        case 2: operation = to_lowercase; break;
        case 3: operation = reverse_string; break;
        case 4: operation = remove_whitespace; break;
        default:
            printf("Invalid choice\n");
            return 1;
    }
    
    if (perform_string_operation(input, output, sizeof(output), operation) != 0) {
        printf("Error processing string\n");
        return 1;
    }
    
    printf("Original: %s\n", input);
    printf("Result: %s\n", output);
    
    return 0;
}