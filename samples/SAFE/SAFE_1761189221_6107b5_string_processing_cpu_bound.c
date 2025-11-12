//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int is_valid_input(const char *str, size_t len) {
    if (str == NULL || len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

int extract_words(const char *text, char words[][32], size_t max_words) {
    if (text == NULL || max_words == 0) return 0;
    
    int word_count = 0;
    size_t i = 0;
    while (text[i] != '\0' && word_count < max_words) {
        while (text[i] != '\0' && !isalpha(text[i])) i++;
        if (text[i] == '\0') break;
        
        size_t start = i;
        while (text[i] != '\0' && isalpha(text[i])) i++;
        size_t word_len = i - start;
        
        if (word_len >= 32) word_len = 31;
        
        for (size_t j = 0; j < word_len; j++) {
            words[word_count][j] = tolower(text[start + j]);
        }
        words[word_count][word_len] = '\0';
        
        if (word_len > 0) word_count++;
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
        result[0] = '\0';
        return;
    }
    
    int max_freq = 0;
    strcpy(result, words[0]);
    
    for (int i = 0; i < word_count; i++) {
        int freq = count_word_frequency(words, word_count, words[i]);
        if (freq > max_freq || (freq == max_freq && strcmp(words[i], result) < 0)) {
            max_freq = freq;
            strcpy(result, words[i]);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][32];
    char most_frequent[32];
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!is_valid_input(input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    int word_count = extract_words(input, words, MAX_WORDS);
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    find_most_frequent(words, word_count, most_frequent);
    
    int freq = count_word_frequency(words, word_count, most_frequent);
    printf("Most frequent word: %s (appears %d times)\n", most_frequent, freq);
    
    printf("Word frequency analysis:\n");
    for (int i = 0; i < word_count; i++) {
        if (i == 0 || strcmp(words[i], words[i-1]) != 0) {
            int count = count_word_frequency(words, word_count, words[i]);
            printf("%s: %d\n", words[i], count);
        }
    }
    
    return 0;
}