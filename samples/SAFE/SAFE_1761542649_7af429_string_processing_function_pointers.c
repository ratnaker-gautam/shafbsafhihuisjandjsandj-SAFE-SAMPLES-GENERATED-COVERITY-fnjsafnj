//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

typedef void (*string_processor)(char *str, size_t len);

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
    if (write_pos < len) {
        str[write_pos] = '\0';
    }
}

int safe_strlen(const char *str, size_t max_len) {
    if (str == NULL) return 0;
    for (size_t i = 0; i < max_len; i++) {
        if (str[i] == '\0') return (int)i;
    }
    return (int)max_len;
}

void process_string(char *input, size_t max_input_len, string_processor processor) {
    if (input == NULL || processor == NULL) return;
    int len = safe_strlen(input, max_input_len);
    if (len > 0) {
        processor(input, (size_t)len);
    }
}

int main(void) {
    char input_buffer[256];
    string_processor processors[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_whitespace
    };
    const char *processor_names[] = {
        "Uppercase",
        "Lowercase", 
        "Reverse",
        "Remove whitespace"
    };
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    printf("Enter a string to process (max 255 characters): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    int input_len = safe_strlen(input_buffer, sizeof(input_buffer));
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input string\n");
        return 1;
    }
    
    printf("\nAvailable string processors:\n");
    for (int i = 0; i < num_processors; i++) {
        printf("%d. %s\n", i + 1, processor_names[i]);
    }
    
    printf("Select processor (1-%d): ", num_processors);
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice < 1 || choice > num_processors) {
        printf("Invalid choice\n");
        return 1;
    }
    
    char working_copy[256];
    if ((size_t)input_len >= sizeof(working_copy)) {
        printf("Input too long for processing\n");
        return 1;
    }
    
    strncpy(working_copy, input_buffer, sizeof(working_copy) - 1);
    working_copy[sizeof(working_copy) - 1] = '\0';
    
    process_string(working_copy, sizeof(working_copy), processors[choice - 1]);
    
    printf("Original: %s\n", input_buffer);
    printf("Processed: %s\n", working_copy);
    
    return 0;
}