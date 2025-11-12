//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 1000
#define MAX_WORDS 500

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
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

int extract_words(const char *text, char words[][32], int max_words) {
    if (text == NULL || max_words <= 0) return 0;
    
    int word_count = 0;
    size_t i = 0;
    size_t len = strlen(text);
    
    while (i < len && word_count < max_words) {
        while (i < len && !is_word_char(text[i])) i++;
        if (i >= len) break;
        
        size_t start = i;
        while (i < len && is_word_char(text[i]) && (i - start) < 31) i++;
        
        size_t word_len = i - start;
        if (word_len > 0 && word_len < 32) {
            strncpy(words[word_count], &text[start], word_len);
            words[word_count][word_len] = '\0';
            to_lowercase(words[word_count]);
            word_count++;
        }
    }
    
    return word_count;
}

int count_word_frequency(const char *word, char words[][32], int word_count) {
    if (word == NULL || word_count <= 0) return 0;
    
    int count = 0;
    char lower_word[32];
    strncpy(lower_word, word, 31);
    lower_word[31] = '\0';
    to_lowercase(lower_word);
    
    for (int i = 0; i < word_count; i++) {
        if (strcmp(words[i], lower_word) == 0) {
            count++;
        }
    }
    return count;
}

void find_most_frequent(char words[][32], int word_count, char result[32]) {
    if (word_count <= 0 || result == NULL) {
        if (result) result[0] = '\0';
        return;
    }
    
    int max_freq = 0;
    int max_index = 0;
    
    for (int i = 0; i < word_count; i++) {
        int freq = count_word_frequency(words[i], words, word_count);
        if (freq > max_freq || (freq == max_freq && i < max_index)) {
            max_freq = freq;
            max_index = i;
        }
    }
    
    if (max_freq > 0) {
        strncpy(result, words[max_index], 31);
        result[31] = '\0';
    } else {
        result[0] = '\0';
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][32];
    
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
    
    printf("Found %d words\n", word_count);
    
    char most_frequent[32];
    find_most_frequent(words, word_count, most_frequent);
    
    if (most_frequent[0] != '\0') {
        int freq = count_word_frequency(most_frequent, words, word_count);
        printf("Most frequent word: '%s' (appears %d times)\n", most_frequent, freq);
    }
    
    printf("Word frequency breakdown:\n");
    int processed[MAX_WORDS] = {0};
    for (int i = 0; i < word_count; i++) {
        if (processed[i]) continue;
        
        int freq = count_word_frequency(words[i], words, word_count);
        printf("  '%s': %d\n", words[i], freq);
        
        for (int j = i; j < word_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                processed[j] = 1;
            }
        }
    }
    
    return 0;
}