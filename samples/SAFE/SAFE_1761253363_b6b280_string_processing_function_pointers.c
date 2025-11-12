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

void remove_whitespace(char *str, size_t len) {
    if (str == NULL) return;
    size_t write_pos = 0;
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        if (!isspace((unsigned char)str[i])) {
            str[write_pos] = str[i];
            write_pos++;
        }
    }
    str[write_pos] = '\0';
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
    
    if (choice < 1 || choice > 4) {
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
    
    return len > 0;
}

void apply_string_operation(string_operation_t operation, char *str, size_t len) {
    if (operation == NULL || str == NULL) return;
    operation(str, len);
}

int main(void) {
    char input[256];
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
    
    printf("Available string operations:\n");
    for (int i = 0; i < 4; i++) {
        printf("%d. %s\n", i + 1, operation_names[i]);
    }
    
    printf("\nSelect operation (1-4): ");
    int choice = get_valid_choice();
    if (choice == -1) {
        printf("Invalid choice. Please enter a number between 1 and 4.\n");
        return 1;
    }
    
    printf("Enter a string (max 255 characters): ");
    if (!get_input_string(input, sizeof(input))) {
        printf("Invalid input or empty string.\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len == 0) {
        printf("Empty string provided.\n");
        return 1;
    }
    
    char original[256];
    strncpy(original, input, sizeof(original) - 1);
    original[sizeof(original) - 1] = '\0';
    
    apply_string_operation(operations[choice - 1], input, input_len);
    
    printf("\nOriginal: %s\n", original);
    printf("Result:   %s\n", input);
    
    return 0;
}