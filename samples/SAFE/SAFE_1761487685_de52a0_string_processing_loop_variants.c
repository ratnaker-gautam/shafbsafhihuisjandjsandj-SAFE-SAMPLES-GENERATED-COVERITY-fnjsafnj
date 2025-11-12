//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024

int main() {
    char input[MAX_INPUT_LEN + 1];
    char processed[MAX_INPUT_LEN + 1];
    int word_count = 0;
    int char_count = 0;
    int in_word = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input provided\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    size_t proc_idx = 0;
    for (size_t i = 0; i < input_len; i++) {
        if (isalpha((unsigned char)input[i])) {
            processed[proc_idx++] = tolower((unsigned char)input[i]);
            char_count++;
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else if (isspace((unsigned char)input[i])) {
            if (in_word) {
                processed[proc_idx++] = ' ';
            }
            in_word = 0;
        }
        
        if (proc_idx >= MAX_INPUT_LEN) {
            break;
        }
    }
    
    if (proc_idx > 0 && processed[proc_idx - 1] == ' ') {
        proc_idx--;
    }
    processed[proc_idx] = '\0';
    
    printf("Original: \"%s\"\n", input);
    printf("Processed: \"%s\"\n", processed);
    printf("Word count: %d\n", word_count);
    printf("Character count (letters only): %d\n", char_count);
    
    int vowel_count = 0;
    int consonant_count = 0;
    char *ptr = processed;
    while (*ptr) {
        if (isalpha((unsigned char)*ptr)) {
            char c = tolower((unsigned char)*ptr);
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                vowel_count++;
            } else {
                consonant_count++;
            }
        }
        ptr++;
    }
    
    printf("Vowels: %d\n", vowel_count);
    printf("Consonants: %d\n", consonant_count);
    
    printf("Reversed words: ");
    char temp[MAX_INPUT_LEN + 1];
    size_t temp_idx = 0;
    size_t start = 0;
    
    for (size_t i = 0; i <= proc_idx; i++) {
        if (processed[i] == ' ' || processed[i] == '\0') {
            for (size_t j = i; j > start; j--) {
                if (j > 0 && j - 1 >= start) {
                    temp[temp_idx++] = processed[j - 1];
                }
            }
            if (processed[i] == ' ') {
                temp[temp_idx++] = ' ';
            }
            start = i + 1;
        }
        
        if (temp_idx >= MAX_INPUT_LEN) {
            break;
        }
    }
    temp[temp_idx] = '\0';
    
    printf("%s\n", temp);
    
    return 0;
}