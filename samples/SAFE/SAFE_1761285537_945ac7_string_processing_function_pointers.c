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

    printf("Enter a string (max 1023 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    printf("\nAvailable operations:\n");
    for (int i = 0; i < num_operations; i++) {
        printf("%d. %s\n", i + 1, operation_names[i]);
    }

    printf("Select operation (1-%d): ", num_operations);
    int choice = get_valid_choice();
    if (choice < 1 || choice > num_operations) {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }

    char working_copy[1024];
    if (strlen(input) >= sizeof(working_copy)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    strncpy(working_copy, input, sizeof(working_copy) - 1);
    working_copy[sizeof(working_copy) - 1] = '\0';

    process_string_with_function(working_copy, operations[choice - 1]);

    printf("Original: %s", input);
    printf("Result: %s\n", working_copy);

    return 0;
}