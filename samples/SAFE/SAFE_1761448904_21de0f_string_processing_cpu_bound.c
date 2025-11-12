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
    if (len == 0 || len > max_len) return 0;
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

int extract_words(const char *input, char words[][32], size_t max_words) {
    if (input == NULL || max_words == 0) return 0;
    
    size_t word_count = 0;
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
                if (word_len < 32 && word_count < max_words) {
                    strncpy(words[word_count], &input[word_start], word_len);
                    words[word_count][word_len] = '\0';
                    to_lowercase(words[word_count]);
                    word_count++;
                }
                in_word = 0;
            }
        }
    }
    
    return word_count;
}

int compare_strings(const void *a, const void *b) {
    return strcmp((const char*)a, (const char*)b);
}

void process_string(const char *input) {
    if (!is_valid_input(input, MAX_INPUT_LEN)) {
        printf("Invalid input\n");
        return;
    }
    
    char words[MAX_WORDS][32] = {0};
    int word_count = extract_words(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No words found\n");
        return;
    }
    
    qsort(words, word_count, 32, compare_strings);
    
    printf("Sorted words (%d found):\n", word_count);
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    char longest_word[32] = "";
    size_t max_len = 0;
    
    for (int i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        if (len > max_len) {
            max_len = len;
            strncpy(longest_word, words[i], 31);
            longest_word[31] = '\0';
        }
    }
    
    printf("Longest word: %s (%zu characters)\n", longest_word, max_len);
    
    int unique_count = 0;
    for (int i = 0; i < word_count; i++) {
        if (i == 0 || strcmp(words[i], words[i-1]) != 0) {
            unique_count++;
        }
    }
    
    printf("Unique words: %d\n", unique_count);
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text to process (max %d characters):\n", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    process_string(input);
    
    return 0;
}