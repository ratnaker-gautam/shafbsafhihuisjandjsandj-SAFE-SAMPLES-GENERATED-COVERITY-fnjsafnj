//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    return 1;
}

int split_into_words(const char *input, char words[][MAX_WORD_LEN], int *word_count) {
    if (!validate_input(input)) return 0;
    
    *word_count = 0;
    size_t input_len = strlen(input);
    size_t word_start = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha((unsigned char)input[i])) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len >= MAX_WORD_LEN) return 0;
                if (*word_count >= MAX_WORDS) return 0;
                
                strncpy(words[*word_count], input + word_start, word_len);
                words[*word_count][word_len] = '\0';
                (*word_count)++;
                in_word = 0;
            }
        }
    }
    return 1;
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int count_word_frequency(const char words[][MAX_WORD_LEN], int word_count, const char *target) {
    if (target == NULL || strlen(target) == 0 || strlen(target) >= MAX_WORD_LEN) return 0;
    
    int count = 0;
    char lowercase_target[MAX_WORD_LEN];
    strncpy(lowercase_target, target, MAX_WORD_LEN - 1);
    lowercase_target[MAX_WORD_LEN - 1] = '\0';
    to_lowercase(lowercase_target);
    
    for (int i = 0; i < word_count; i++) {
        char temp_word[MAX_WORD_LEN];
        strncpy(temp_word, words[i], MAX_WORD_LEN - 1);
        temp_word[MAX_WORD_LEN - 1] = '\0';
        to_lowercase(temp_word);
        
        if (strcmp(temp_word, lowercase_target) == 0) {
            count++;
        }
    }
    return count;
}

void process_text_analysis(const char *input) {
    if (!validate_input(input)) {
        printf("Invalid input\n");
        return;
    }
    
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    
    if (!split_into_words(input, words, &word_count)) {
        printf("Error processing input\n");
        return;
    }
    
    printf("Text analysis results:\n");
    printf("Total words: %d\n", word_count);
    
    if (word_count > 0) {
        printf("Word frequencies:\n");
        
        char processed_words[MAX_WORDS][MAX_WORD_LEN];
        int processed_count = 0;
        
        for (int i = 0; i < word_count; i++) {
            char current_lower[MAX_WORD_LEN];
            strncpy(current_lower, words[i], MAX_WORD_LEN - 1);
            current_lower[MAX_WORD_LEN - 1] = '\0';
            to_lowercase(current_lower);
            
            int already_counted = 0;
            for (int j = 0; j < processed_count; j++) {
                if (strcmp(processed_words[j], current_lower) == 0) {
                    already_counted = 1;
                    break;
                }
            }
            
            if (!already_counted) {
                int freq = count_word_frequency(words, word_count, words[i]);
                printf("  '%s': %d\n", current_lower, freq);
                
                if (processed_count < MAX_WORDS) {
                    strncpy(processed_words[processed_count], current_lower, MAX_WORD_LEN - 1);
                    processed_words[processed_count][MAX_WORD_LEN - 1] = '\0';
                    processed_count++;
                }
            }
        }
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    process_text_analysis(input);
    
    return 0;
}