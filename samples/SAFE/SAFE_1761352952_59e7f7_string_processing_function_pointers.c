//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef void (*string_processor)(char *);

void to_uppercase(char *str) {
    if (str == NULL) return;
    for (char *p = str; *p != '\0'; p++) {
        *p = toupper((unsigned char)*p);
    }
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (char *p = str; *p != '\0'; p++) {
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

void remove_vowels(char *str) {
    if (str == NULL) return;
    char *read = str;
    char *write = str;
    while (*read != '\0') {
        char c = tolower((unsigned char)*read);
        if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u') {
            *write = *read;
            write++;
        }
        read++;
    }
    *write = '\0';
}

int get_valid_choice(void) {
    int choice;
    char buffer[100];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
    if (sscanf(buffer, "%d", &choice) != 1) return -1;
    return choice;
}

void process_string(char *str, string_processor processor) {
    if (str == NULL || processor == NULL) return;
    char temp[256];
    if (strlen(str) >= sizeof(temp)) return;
    strcpy(temp, str);
    processor(temp);
    printf("Result: %s\n", temp);
}

int main(void) {
    char input[256];
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
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
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
        printf("Empty string entered\n");
        return 1;
    }
    
    printf("\nAvailable operations:\n");
    for (int i = 0; i < num_processors; i++) {
        printf("%d. %s\n", i + 1, names[i]);
    }
    
    printf("Choose an operation (1-%d): ", num_processors);
    int choice = get_valid_choice();
    
    if (choice < 1 || choice > num_processors) {
        printf("Invalid choice\n");
        return 1;
    }
    
    process_string(input, processors[choice - 1]);
    return 0;
}