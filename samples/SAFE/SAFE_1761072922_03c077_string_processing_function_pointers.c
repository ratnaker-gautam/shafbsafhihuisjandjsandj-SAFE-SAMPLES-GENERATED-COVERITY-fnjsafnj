//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: string_processing
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
    size_t start = 0;
    size_t end = len - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void remove_non_alnum(char *str, size_t len) {
    if (str == NULL) return;
    size_t write_pos = 0;
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        if (isalnum((unsigned char)str[i])) {
            str[write_pos++] = str[i];
        }
    }
    if (write_pos < len) {
        str[write_pos] = '\0';
    }
}

int get_valid_choice(void) {
    int choice;
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    if (sscanf(buffer, "%d", &choice) != 1) {
        return -1;
    }
    return choice;
}

int get_input_string(char *buffer, size_t size) {
    if (buffer == NULL || size == 0) return 0;
    if (fgets(buffer, (int)size, stdin) == NULL) {
        return 0;
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    return (int)len;
}

void apply_operation(string_operation_t operation, char *str, size_t len) {
    if (operation != NULL && str != NULL) {
        operation(str, len);
    }
}

int main(void) {
    char input[1024];
    string_operation_t operations[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_non_alnum
    };
    const char *operation_names[] = {
        "Convert to uppercase",
        "Convert to lowercase", 
        "Reverse string",
        "Remove non-alphanumeric characters"
    };
    const int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    printf("Enter a string (max %zu characters): ", sizeof(input) - 1);
    int input_len = get_input_string(input, sizeof(input));
    if (input_len <= 0) {
        printf("Error: Invalid input or empty string.\n");
        return 1;
    }
    
    printf("\nAvailable operations:\n");
    for (int i = 0; i < num_operations; i++) {
        printf("%d. %s\n", i + 1, operation_names[i]);
    }
    
    printf("\nSelect operation (1-%d): ", num_operations);
    int choice = get_valid_choice();
    if (choice < 1 || choice > num_operations) {
        printf("Error: Invalid choice.\n");
        return 1;
    }
    
    char working_copy[sizeof(input)];
    size_t copy_len = (size_t)input_len;
    if (copy_len >= sizeof(working_copy)) {
        printf("Error: String too long for processing.\n");
        return 1;
    }
    strncpy(working_copy, input, sizeof(working_copy));
    working_copy[sizeof(working_copy) - 1] = '\0';
    
    printf("\nOriginal: %s\n", input);
    apply_operation(operations[choice - 1], working_copy, copy_len);
    printf("Result: %s\n", working_copy);
    
    return 0;
}