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

int get_valid_choice(void) {
    int choice;
    char buffer[256];
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    
    if (sscanf(buffer, "%d", &choice) != 1) {
        return -1;
    }
    
    if (choice < 1 || choice > 5) {
        return -1;
    }
    
    return choice;
}

void process_string_with_function(string_processor processor, const char *input, char *output, size_t output_size) {
    if (processor == NULL || input == NULL || output == NULL || output_size == 0) {
        return;
    }
    
    size_t input_len = strlen(input);
    if (input_len >= output_size) {
        return;
    }
    
    strncpy(output, input, output_size - 1);
    output[output_size - 1] = '\0';
    
    processor(output);
}

int main(void) {
    string_processor processors[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_whitespace
    };
    
    const char *processor_names[] = {
        "Convert to UPPERCASE",
        "Convert to lowercase",
        "Reverse string",
        "Remove whitespace",
        "Exit"
    };
    
    char input[256];
    char output[256];
    
    printf("Available string processing operations:\n");
    for (int i = 0; i < 5; i++) {
        printf("%d. %s\n", i + 1, processor_names[i]);
    }
    
    while (1) {
        printf("\nEnter your choice (1-5): ");
        int choice = get_valid_choice();
        
        if (choice == -1) {
            printf("Invalid choice. Please enter a number between 1 and 5.\n");
            continue;
        }
        
        if (choice == 5) {
            printf("Goodbye!\n");
            break;
        }
        
        printf("Enter a string to process: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading input.\n");
            continue;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strlen(input) == 0) {
            printf("Empty string entered.\n");
            continue;
        }
        
        process_string_with_function(processors[choice - 1], input, output, sizeof(output));
        
        printf("Original: %s\n", input);
        printf("Result: %s\n", output);
    }
    
    return 0;
}