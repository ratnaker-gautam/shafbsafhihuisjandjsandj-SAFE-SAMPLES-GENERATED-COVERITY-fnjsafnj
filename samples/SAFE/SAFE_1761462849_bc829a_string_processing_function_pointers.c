//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef void (*string_processor)(char *);

void to_uppercase(char *str) {
    if (str == NULL) return;
    for (char *p = str; *p; p++) {
        *p = toupper((unsigned char)*p);
    }
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (char *p = str; *p; p++) {
        *p = tolower((unsigned char)*p);
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

void remove_non_alnum(char *str) {
    if (str == NULL) return;
    char *dest = str;
    for (char *src = str; *src; src++) {
        if (isalnum((unsigned char)*src)) {
            *dest++ = *src;
        }
    }
    *dest = '\0';
}

int get_valid_choice(void) {
    int choice;
    char buffer[100];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
    if (sscanf(buffer, "%d", &choice) != 1) return -1;
    return choice;
}

int main(void) {
    string_processor processors[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_non_alnum
    };
    const char *names[] = {
        "Convert to uppercase",
        "Convert to lowercase",
        "Reverse string",
        "Remove non-alphanumeric characters"
    };
    const int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    char input[256];
    
    printf("Enter a string (max 255 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty string provided\n");
        return 1;
    }
    
    printf("\nAvailable string processors:\n");
    for (int i = 0; i < num_processors; i++) {
        printf("%d. %s\n", i + 1, names[i]);
    }
    
    printf("Select processor (1-%d): ", num_processors);
    int choice = get_valid_choice();
    
    if (choice < 1 || choice > num_processors) {
        printf("Invalid choice\n");
        return 1;
    }
    
    char working_copy[256];
    if (strlen(input) >= sizeof(working_copy)) {
        printf("Input too long\n");
        return 1;
    }
    strcpy(working_copy, input);
    
    processors[choice - 1](working_copy);
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", working_copy);
    
    return 0;
}