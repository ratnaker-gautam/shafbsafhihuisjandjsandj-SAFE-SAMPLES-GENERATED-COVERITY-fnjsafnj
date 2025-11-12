//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 4096

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

void process_string(char *str) {
    if (str == NULL) return;
    size_t len = strlen(str);
    
    for (size_t i = 0; i < len; i++) {
        if (islower((unsigned char)str[i])) {
            str[i] = (char)toupper((unsigned char)str[i]);
        } else if (isupper((unsigned char)str[i])) {
            str[i] = (char)tolower((unsigned char)str[i]);
        }
    }
    
    for (size_t i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
    
    int vowel_count = 0;
    int consonant_count = 0;
    for (size_t i = 0; i < len; i++) {
        char c = (char)tolower((unsigned char)str[i]);
        if (c >= 'a' && c <= 'z') {
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                vowel_count++;
            } else {
                consonant_count++;
            }
        }
    }
    
    printf("Processed string: %s\n", str);
    printf("Vowels: %d, Consonants: %d\n", vowel_count, consonant_count);
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter a string to process (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input: must be printable characters, non-empty, and under %d characters\n", MAX_INPUT_LEN);
        return 1;
    }
    
    process_string(input);
    
    return 0;
}