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

int get_user_choice(void) {
    int choice = 0;
    char input[16] = {0};
    
    printf("Available operations:\n");
    printf("1. Convert to uppercase\n");
    printf("2. Convert to lowercase\n");
    printf("3. Reverse string\n");
    printf("4. Remove whitespace\n");
    printf("Enter your choice (1-4): ");
    
    if (fgets(input, sizeof(input), stdin) != NULL) {
        if (sscanf(input, "%d", &choice) == 1) {
            if (choice >= 1 && choice <= 4) {
                return choice;
            }
        }
    }
    return -1;
}

int get_input_string(char *buffer, size_t buffer_size) {
    if (buffer == NULL || buffer_size == 0) return 0;
    
    printf("Enter a string (max %zu characters): ", buffer_size - 1);
    
    if (fgets(buffer, (int)buffer_size, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strnlen(buffer, buffer_size);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    return 1;
}

void apply_operation(string_operation_t operation, char *str, size_t len) {
    if (operation != NULL && str != NULL && len > 0) {
        operation(str, len);
    }
}

int main(void) {
    char input_buffer[256] = {0};
    int choice;
    
    if (!get_input_string(input_buffer, sizeof(input_buffer))) {
        fprintf(stderr, "Error reading input string\n");
        return 1;
    }
    
    choice = get_user_choice();
    if (choice == -1) {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }
    
    string_operation_t operations[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_whitespace
    };
    
    char original[256];
    strncpy(original, input_buffer, sizeof(original) - 1);
    original[sizeof(original) - 1] = '\0';
    
    apply_operation(operations[choice - 1], input_buffer, sizeof(input_buffer));
    
    printf("Original: '%s'\n", original);
    printf("Result:   '%s'\n", input_buffer);
    
    return 0;
}