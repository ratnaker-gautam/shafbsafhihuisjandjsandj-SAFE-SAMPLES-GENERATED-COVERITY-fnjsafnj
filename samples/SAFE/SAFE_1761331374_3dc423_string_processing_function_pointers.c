//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

typedef void (*string_processor)(char *str);

void to_uppercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = (char)toupper((unsigned char)str[i]);
    }
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}

void reverse_string(char *str) {
    if (str == NULL) return;
    size_t len = strlen(str);
    for (size_t i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
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

void process_string(char *str, string_processor processor) {
    if (str == NULL || processor == NULL) return;
    processor(str);
}

int main() {
    char input[256];
    string_processor processors[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_whitespace
    };
    const char *names[] = {
        "Uppercase",
        "Lowercase", 
        "Reverse",
        "Remove whitespace"
    };
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    printf("Enter a string to process: ");
    if (!safe_input(input, sizeof(input))) {
        printf("Error reading input\n");
        return 1;
    }
    
    if (strlen(input) == 0) {
        printf("Empty string provided\n");
        return 1;
    }
    
    printf("\nAvailable operations:\n");
    for (int i = 0; i < num_processors; i++) {
        printf("%d. %s\n", i + 1, names[i]);
    }
    
    printf("Select operation (1-%d): ", num_processors);
    char choice_str[16];
    if (!safe_input(choice_str, sizeof(choice_str))) {
        printf("Error reading choice\n");
        return 1;
    }
    
    int choice = 0;
    if (sscanf(choice_str, "%d", &choice) != 1) {
        printf("Invalid choice format\n");
        return 1;
    }
    
    if (choice < 1 || choice > num_processors) {
        printf("Choice out of range\n");
        return 1;
    }
    
    char original[256];
    strncpy(original, input, sizeof(original) - 1);
    original[sizeof(original) - 1] = '\0';
    
    process_string(input, processors[choice - 1]);
    
    printf("\nOriginal: %s\n", original);
    printf("Result: %s\n", input);
    
    return 0;
}