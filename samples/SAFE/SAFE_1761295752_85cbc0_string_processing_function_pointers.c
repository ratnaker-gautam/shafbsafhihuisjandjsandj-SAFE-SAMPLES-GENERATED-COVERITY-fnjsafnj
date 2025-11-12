//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

typedef void (*string_processor)(char *str);

void to_uppercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void reverse_string(char *str) {
    if (str == NULL) return;
    size_t len = strlen(str);
    for (size_t i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

void remove_whitespace(char *str) {
    if (str == NULL) return;
    size_t write_pos = 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (!isspace((unsigned char)str[i])) {
            str[write_pos++] = str[i];
        }
    }
    str[write_pos] = '\0';
}

int safe_input(char *buffer, size_t buffer_size) {
    if (buffer == NULL || buffer_size == 0) return 0;
    if (fgets(buffer, (int)buffer_size, stdin) == NULL) return 0;
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else if (len == buffer_size - 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    return 1;
}

int main(void) {
    string_processor processors[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_whitespace
    };
    const char *processor_names[] = {
        "Convert to uppercase",
        "Convert to lowercase",
        "Reverse string",
        "Remove whitespace"
    };
    const size_t num_processors = sizeof(processors) / sizeof(processors[0]);
    
    char input[256];
    char working_buffer[256];
    
    printf("Enter a string to process (max 255 characters): ");
    if (!safe_input(input, sizeof(input))) {
        printf("Error reading input.\n");
        return 1;
    }
    
    if (strlen(input) == 0) {
        printf("Empty string provided.\n");
        return 1;
    }
    
    printf("\nAvailable string processors:\n");
    for (size_t i = 0; i < num_processors; i++) {
        printf("%zu. %s\n", i + 1, processor_names[i]);
    }
    
    printf("Select processor (1-%zu): ", num_processors);
    char choice_str[16];
    if (!safe_input(choice_str, sizeof(choice_str))) {
        printf("Error reading choice.\n");
        return 1;
    }
    
    unsigned long choice = 0;
    for (size_t i = 0; choice_str[i] != '\0'; i++) {
        if (!isdigit((unsigned char)choice_str[i])) {
            printf("Invalid choice format.\n");
            return 1;
        }
        if (choice > UINT32_MAX / 10) {
            printf("Choice too large.\n");
            return 1;
        }
        choice = choice * 10 + (choice_str[i] - '0');
    }
    
    if (choice < 1 || choice > num_processors) {
        printf("Invalid choice. Must be between 1 and %zu.\n", num_processors);
        return 1;
    }
    
    strncpy(working_buffer, input, sizeof(working_buffer) - 1);
    working_buffer[sizeof(working_buffer) - 1] = '\0';
    
    string_processor selected_processor = processors[choice - 1];
    selected_processor(working_buffer);
    
    printf("\nOriginal: %s\n", input);
    printf("Processed: %s\n", working_buffer);
    
    return 0;
}