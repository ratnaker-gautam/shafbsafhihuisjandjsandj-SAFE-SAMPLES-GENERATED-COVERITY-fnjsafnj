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

void remove_non_alnum(char *str, size_t len) {
    if (str == NULL || len == 0) return;
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

int get_user_choice(void) {
    int choice = 0;
    char input[16];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        if (sscanf(input, "%d", &choice) != 1) {
            return -1;
        }
    }
    return choice;
}

void get_user_string(char *buffer, size_t buffer_size) {
    if (buffer == NULL || buffer_size == 0) return;
    if (fgets(buffer, (int)buffer_size, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
    }
}

int main(void) {
    const size_t MAX_STR_LEN = 256;
    char input_string[MAX_STR_LEN];
    char working_string[MAX_STR_LEN];
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
    const int num_operations = 4;

    printf("Enter a string to process (max %zu characters): ", MAX_STR_LEN - 1);
    get_user_string(input_string, sizeof(input_string));
    
    if (strlen(input_string) == 0) {
        printf("Error: Empty string entered.\n");
        return 1;
    }

    printf("\nAvailable operations:\n");
    for (int i = 0; i < num_operations; i++) {
        printf("%d. %s\n", i + 1, operation_names[i]);
    }
    
    printf("Select operation (1-%d): ", num_operations);
    int choice = get_user_choice();
    
    if (choice < 1 || choice > num_operations) {
        printf("Error: Invalid choice.\n");
        return 1;
    }

    strncpy(working_string, input_string, sizeof(working_string) - 1);
    working_string[sizeof(working_string) - 1] = '\0';
    
    size_t str_len = strlen(working_string);
    operations[choice - 1](working_string, str_len);
    
    printf("\nOriginal string: %s\n", input_string);
    printf("Processed string: %s\n", working_string);
    
    return 0;
}