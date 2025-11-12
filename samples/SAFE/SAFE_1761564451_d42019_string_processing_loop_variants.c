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
    size_t i = 0;
    
    while (i < input_len) {
        if (isspace((unsigned char)input[i])) {
            if (in_word) {
                word_count++;
                in_word = 0;
            }
            i++;
            continue;
        }
        
        if (!in_word) {
            in_word = 1;
        }
        
        char current = input[i];
        if (isalpha((unsigned char)current)) {
            if (proc_idx < MAX_INPUT_LEN) {
                processed[proc_idx] = tolower((unsigned char)current);
                proc_idx++;
                char_count++;
            }
        }
        i++;
    }
    
    if (in_word) {
        word_count++;
    }
    
    if (proc_idx < MAX_INPUT_LEN) {
        processed[proc_idx] = '\0';
    } else {
        processed[MAX_INPUT_LEN] = '\0';
    }
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    printf("Word count: %d\n", word_count);
    printf("Alpha character count: %d\n", char_count);
    
    size_t j = 0;
    int vowel_count = 0;
    int consonant_count = 0;
    
    for (j = 0; j < proc_idx; j++) {
        char c = processed[j];
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            vowel_count++;
        } else {
            consonant_count++;
        }
    }
    
    printf("Vowels: %d\n", vowel_count);
    printf("Consonants: %d\n", consonant_count);
    
    size_t k = 0;
    printf("Character frequencies:\n");
    for (k = 0; k < 26; k++) {
        char target = 'a' + k;
        int freq = 0;
        size_t m = 0;
        
        while (m < proc_idx) {
            if (processed[m] == target) {
                freq++;
            }
            m++;
        }
        
        if (freq > 0) {
            printf("%c: %d\n", target, freq);
        }
    }
    
    return 0;
}