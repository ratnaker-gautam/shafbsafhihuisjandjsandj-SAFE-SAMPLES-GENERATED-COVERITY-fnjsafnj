//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    return 1;
}

void reverse_string(char *str) {
    if (str == NULL) return;
    char *start = str;
    char *end = str + strlen(str) - 1;
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

void convert_to_uppercase(char *str) {
    if (str == NULL) return;
    char *ptr = str;
    while (*ptr != '\0') {
        *ptr = toupper((unsigned char)*ptr);
        ptr++;
    }
}

int count_vowels(const char *str) {
    if (str == NULL) return 0;
    int count = 0;
    const char *ptr = str;
    while (*ptr != '\0') {
        char c = tolower((unsigned char)*ptr);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
        ptr++;
    }
    return count;
}

int main() {
    char input[MAX_INPUT_LEN];
    
    printf("Enter a string: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    char working_copy[MAX_INPUT_LEN];
    if (strlen(input) >= sizeof(working_copy)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    strcpy(working_copy, input);
    
    printf("Original: %s\n", input);
    
    reverse_string(working_copy);
    printf("Reversed: %s\n", working_copy);
    
    strcpy(working_copy, input);
    convert_to_uppercase(working_copy);
    printf("Uppercase: %s\n", working_copy);
    
    int vowel_count = count_vowels(input);
    printf("Vowel count: %d\n", vowel_count);
    
    return 0;
}