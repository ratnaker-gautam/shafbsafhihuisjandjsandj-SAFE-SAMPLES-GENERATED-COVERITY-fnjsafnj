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

void remove_vowels(char *str) {
    if (str == NULL) return;
    size_t write_pos = 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        char c = tolower((unsigned char)str[i]);
        if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u') {
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
    
    return choice;
}

void process_string_with_function(char *str, string_processor processor) {
    if (str == NULL || processor == NULL) return;
    
    char buffer[1024];
    if (strlen(str) >= sizeof(buffer)) {
        printf("Input string too long.\n");
        return;
    }
    
    strcpy(buffer, str);
    processor(buffer);
    printf("Result: %s\n", buffer);
}

int main(void) {
    char input[1024];
    string_processor processors[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_vowels
    };
    const char *names[] = {
        "Convert to uppercase",
        "Convert to lowercase", 
        "Reverse string",
        "Remove vowels"
    };
    const int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    printf("Enter a string to process (max 1023 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty string provided.\n");
        return 1;
    }
    
    printf("\nAvailable string processing functions:\n");
    for (int i = 0; i < num_processors; i++) {
        printf("%d. %s\n", i + 1, names[i]);
    }
    
    printf("\nSelect a function (1-%d): ", num_processors);
    int choice = get_valid_choice();
    
    if (choice < 1 || choice > num_processors) {
        printf("Invalid choice.\n");
        return 1;
    }
    
    printf("\nOriginal: %s\n", input);
    process_string_with_function(input, processors[choice - 1]);
    
    return 0;
}