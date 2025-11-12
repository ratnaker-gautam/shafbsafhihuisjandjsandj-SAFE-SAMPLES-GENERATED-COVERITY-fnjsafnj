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
    char buffer[100];
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    
    if (sscanf(buffer, "%d", &choice) != 1) {
        return -1;
    }
    
    return choice;
}

void get_input_string(char *buffer, size_t size) {
    if (buffer == NULL || size == 0) return;
    
    if (fgets(buffer, (int)size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
}

void process_string_with_function(char *str, string_processor processor) {
    if (str == NULL || processor == NULL) return;
    
    char original[256];
    if (strlen(str) >= sizeof(original)) {
        printf("Input string too long for processing.\n");
        return;
    }
    
    strcpy(original, str);
    processor(str);
    printf("Original: %s\n", original);
    printf("Processed: %s\n", str);
}

int main(void) {
    char input[256];
    string_processor processors[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_vowels
    };
    
    const char *menu[] = {
        "Convert to uppercase",
        "Convert to lowercase", 
        "Reverse string",
        "Remove vowels",
        "Exit"
    };
    
    int num_choices = sizeof(menu) / sizeof(menu[0]);
    
    while (1) {
        printf("\nString Processing Menu:\n");
        for (int i = 0; i < num_choices; i++) {
            printf("%d. %s\n", i + 1, menu[i]);
        }
        printf("Enter your choice (1-%d): ", num_choices);
        
        int choice = get_valid_choice();
        if (choice == -1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (choice == num_choices) {
            printf("Goodbye!\n");
            break;
        }
        
        if (choice < 1 || choice > num_choices - 1) {
            printf("Invalid choice. Please select 1-%d.\n", num_choices);
            continue;
        }
        
        printf("Enter a string to process: ");
        get_input_string(input, sizeof(input));
        
        if (strlen(input) == 0) {
            printf("Empty string entered.\n");
            continue;
        }
        
        process_string_with_function(input, processors[choice - 1]);
        
        printf("\nPress Enter to continue...");
        while (getchar() != '\n');
    }
    
    return 0;
}