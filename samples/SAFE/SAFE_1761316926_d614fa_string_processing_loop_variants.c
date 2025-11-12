//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024

int main() {
    char input[MAX_INPUT_LEN + 1];
    char processed[MAX_INPUT_LEN + 1];
    
    printf("Enter a string to process: ");
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
        printf("Empty input string\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    size_t proc_idx = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (isalpha((unsigned char)input[i])) {
            processed[proc_idx] = toupper((unsigned char)input[i]);
            proc_idx++;
        } else if (isdigit((unsigned char)input[i])) {
            processed[proc_idx] = input[i];
            proc_idx++;
        } else if (isspace((unsigned char)input[i])) {
            processed[proc_idx] = ' ';
            proc_idx++;
        }
        i++;
    }
    
    processed[proc_idx] = '\0';
    
    size_t final_len = strlen(processed);
    if (final_len == 0) {
        printf("No valid characters found\n");
        return 0;
    }
    
    printf("Original: \"%s\"\n", input);
    printf("Processed: \"%s\"\n", processed);
    
    char reversed[MAX_INPUT_LEN + 1];
    size_t rev_idx = 0;
    
    for (size_t j = final_len; j > 0; j--) {
        reversed[rev_idx] = processed[j - 1];
        rev_idx++;
    }
    reversed[rev_idx] = '\0';
    
    printf("Reversed: \"%s\"\n", reversed);
    
    int word_count = 0;
    int in_word = 0;
    
    for (size_t k = 0; k < final_len; k++) {
        if (processed[k] != ' ') {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }
    
    printf("Word count: %d\n", word_count);
    
    char most_frequent = '\0';
    int max_count = 0;
    
    for (char ch = 'A'; ch <= 'Z'; ch++) {
        int count = 0;
        size_t m = 0;
        do {
            if (processed[m] == ch) {
                count++;
            }
            m++;
        } while (m < final_len);
        
        if (count > max_count) {
            max_count = count;
            most_frequent = ch;
        }
    }
    
    if (most_frequent != '\0') {
        printf("Most frequent letter: '%c' (appears %d times)\n", most_frequent, max_count);
    } else {
        printf("No letters found in processed string\n");
    }
    
    return 0;
}