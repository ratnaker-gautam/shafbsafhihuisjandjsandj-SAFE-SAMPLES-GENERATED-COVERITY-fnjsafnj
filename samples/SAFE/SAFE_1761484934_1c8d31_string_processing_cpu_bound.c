//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int is_valid_input(const char *str, size_t max_len) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= max_len) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

int extract_words(const char *text, char words[][32], size_t max_words) {
    if (text == NULL || words == NULL || max_words == 0) return 0;
    
    size_t word_count = 0;
    size_t i = 0;
    while (text[i] != '\0' && word_count < max_words) {
        while (text[i] != '\0' && !is_word_char(text[i])) i++;
        if (text[i] == '\0') break;
        
        size_t start = i;
        while (text[i] != '\0' && is_word_char(text[i])) i++;
        size_t length = i - start;
        
        if (length >= 32) length = 31;
        strncpy(words[word_count], &text[start], length);
        words[word_count][length] = '\0';
        to_lowercase(words[word_count]);
        word_count++;
    }
    return word_count;
}

int count_word_frequency(const char *word, char words[][32], int word_count) {
    if (word == NULL || words == NULL || word_count <= 0) return 0;
    
    int count = 0;
    for (int i = 0; i < word_count; i++) {
        if (strcmp(word, words[i]) == 0) {
            count++;
        }
    }
    return count;
}

void find_most_frequent(char words[][32], int word_count, char result[32]) {
    if (words == NULL || word_count <= 0 || result == NULL) {
        result[0] = '\0';
        return;
    }
    
    int max_freq = 0;
    strcpy(result, "");
    
    for (int i = 0; i < word_count; i++) {
        int freq = count_word_frequency(words[i], words, word_count);
        if (freq > max_freq || (freq == max_freq && strcmp(words[i], result) < 0)) {
            max_freq = freq;
            strcpy(result, words[i]);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][32];
    
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (!is_valid_input(input, sizeof(input))) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    int word_count = extract_words(input, words, MAX_WORDS);
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    char most_frequent[32];
    find_most_frequent(words, word_count, most_frequent);
    
    printf("Total words: %d\n", word_count);
    printf("Most frequent word: %s\n", most_frequent);
    printf("Frequency: %d\n", count_word_frequency(most_frequent, words, word_count));
    
    return 0;
}