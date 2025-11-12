//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512
#define MAX_WORD_LEN 64

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
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
        printf("No input provided\n");
        return 0;
    }
    
    i = 0;
    while (i < input_len && word_count < MAX_WORDS) {
        while (i < input_len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        j = 0;
        while (i < input_len && !isspace((unsigned char)input[i]) && j < MAX_WORD_LEN - 1) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        
        if (j > 0) {
            words[word_count][j] = '\0';
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    for (i = 0; i < word_count; i++) {
        size_t word_len = strlen(words[i]);
        int vowel_count = 0;
        int consonant_count = 0;
        int digit_count = 0;
        
        for (j = 0; j < word_len; j++) {
            char c = words[i][j];
            if (isdigit((unsigned char)c)) {
                digit_count++;
            } else if (isalpha((unsigned char)c)) {
                char lower_c = tolower((unsigned char)c);
                if (lower_c == 'a' || lower_c == 'e' || lower_c == 'i' || 
                    lower_c == 'o' || lower_c == 'u') {
                    vowel_count++;
                } else {
                    consonant_count++;
                }
            }
        }
        
        printf("Word %d: \"%s\" (Length: %zu", i + 1, words[i], word_len);
        if (vowel_count > 0) {
            printf(", Vowels: %d", vowel_count);
        }
        if (consonant_count > 0) {
            printf(", Consonants: %d", consonant_count);
        }
        if (digit_count > 0) {
            printf(", Digits: %d", digit_count);
        }
        printf(")\n");
    }
    
    printf("\nWord frequency analysis:\n");
    for (i = 0; i < word_count; i++) {
        if (words[i][0] == '\0') {
            continue;
        }
        
        int frequency = 1;
        for (j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                frequency++;
                words[j][0] = '\0';
            }
        }
        
        if (frequency > 0) {
            printf("'%s' appears %d time%s\n", words[i], frequency, frequency == 1 ? "" : "s");
        }
    }
    
    char longest_word[MAX_WORD_LEN] = "";
    char shortest_word[MAX_WORD_LEN] = "";
    size_t max_len = 0;
    size_t min_len = MAX_WORD_LEN;
    
    for (i = 0; i < word_count; i++) {
        size_t current_len = strlen(words[i]);
        if (current_len > 0) {
            if (current_len > max_len) {
                max_len = current_len;
                strncpy(longest_word, words[i], MAX_WORD_LEN - 1);
                longest_word[MAX_WORD_LEN - 1] = '\0';
            }
            if (current_len < min_len) {
                min_len = current_len;
                strncpy(shortest_word, words[i], MAX_WORD_LEN - 1);
                shortest_word[MAX_WORD_LEN - 1] = '\0';
            }
        }
    }
    
    if (max_len > 0) {
        printf("\nLongest word: '%s' (%zu characters)\n", longest_word, max_len);
    }
    if (min_len > 0 && min_len < MAX_WORD_LEN) {
        printf("Shortest word: '%s' (%zu characters)\n", shortest_word, min_len);
    }
    
    return 0;
}