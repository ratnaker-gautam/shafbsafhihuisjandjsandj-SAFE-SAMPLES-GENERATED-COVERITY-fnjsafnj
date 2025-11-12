//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

int extract_words(const char *text, char words[][32], int max_words) {
    if (text == NULL || max_words <= 0) return 0;
    
    int word_count = 0;
    size_t text_len = strlen(text);
    size_t word_start = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= text_len && word_count < max_words; i++) {
        unsigned char c = (i < text_len) ? (unsigned char)text[i] : ' ';
        
        if (isalpha(c)) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len < 32) {
                    for (size_t j = 0; j < word_len; j++) {
                        words[word_count][j] = tolower((unsigned char)text[word_start + j]);
                    }
                    words[word_count][word_len] = '\0';
                    word_count++;
                }
                in_word = 0;
            }
        }
    }
    
    return word_count;
}

int count_word_frequency(const char words[][32], int word_count, const char *target) {
    if (words == NULL || target == NULL || word_count <= 0) return 0;
    
    int count = 0;
    for (int i = 0; i < word_count; i++) {
        if (strcmp(words[i], target) == 0) {
            count++;
        }
    }
    return count;
}

void find_most_frequent(const char words[][32], int word_count, char result[32]) {
    if (words == NULL || word_count <= 0 || result == NULL) {
        if (result) result[0] = '\0';
        return;
    }
    
    int max_freq = 0;
    result[0] = '\0';
    
    for (int i = 0; i < word_count; i++) {
        int freq = count_word_frequency(words, word_count, words[i]);
        if (freq > max_freq || (freq == max_freq && strcmp(words[i], result) < 0)) {
            max_freq = freq;
            strncpy(result, words[i], 31);
            result[31] = '\0';
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][32];
    char most_frequent[32];
    
    printf("Enter text for analysis: ");
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
    
    int word_count = extract_words(input, words, MAX_WORDS);
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    find_most_frequent(words, word_count, most_frequent);
    
    if (most_frequent[0] != '\0') {
        int freq = count_word_frequency(words, word_count, most_frequent);
        printf("Most frequent word: '%s' (appears %d times)\n", most_frequent, freq);
    }
    
    printf("Total words processed: %d\n", word_count);
    
    return 0;
}