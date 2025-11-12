//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORD_COUNT 100
#define MAX_WORD_LEN 50

struct WordInfo {
    char word[MAX_WORD_LEN];
    int count;
    int first_position;
};

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

int safe_strncpy(char *dest, const char *src, size_t n) {
    if (dest == NULL || src == NULL || n == 0) return 0;
    
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return 1;
}

int find_word_in_array(struct WordInfo *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void to_lower_case(char *str) {
    if (str == NULL) return;
    
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int process_input(const char *input, struct WordInfo *words, int max_words) {
    if (input == NULL || words == NULL || max_words <= 0) return 0;
    
    char buffer[MAX_INPUT_LEN];
    if (!safe_strncpy(buffer, input, sizeof(buffer))) return 0;
    
    int word_count = 0;
    size_t pos = 0;
    size_t input_len = strlen(buffer);
    
    while (pos < input_len && word_count < max_words) {
        while (pos < input_len && !is_valid_char(buffer[pos])) {
            pos++;
        }
        
        if (pos >= input_len) break;
        
        size_t start = pos;
        while (pos < input_len && is_valid_char(buffer[pos])) {
            pos++;
        }
        
        size_t word_len = pos - start;
        if (word_len >= MAX_WORD_LEN) {
            word_len = MAX_WORD_LEN - 1;
        }
        
        char current_word[MAX_WORD_LEN];
        for (size_t i = 0; i < word_len; i++) {
            current_word[i] = buffer[start + i];
        }
        current_word[word_len] = '\0';
        
        to_lower_case(current_word);
        
        int existing_index = find_word_in_array(words, word_count, current_word);
        if (existing_index != -1) {
            words[existing_index].count++;
        } else {
            if (word_count < max_words) {
                if (safe_strncpy(words[word_count].word, current_word, MAX_WORD_LEN)) {
                    words[word_count].count = 1;
                    words[word_count].first_position = (int)start;
                    word_count++;
                }
            }
        }
    }
    
    return word_count;
}

void print_word_stats(struct WordInfo *words, int count) {
    if (words == NULL || count <= 0) return;
    
    printf("\nWord Statistics:\n");
    printf("%-15s %-8s %-15s\n", "Word", "Count", "First Position");
    printf("--------------------------------\n");
    
    for (int i = 0; i < count; i++) {
        printf("%-15s %-8d %-15d\n", words[i].word, words[i].count, words[i].first_position);
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    struct WordInfo words[MAX_WORD_COUNT];
    
    printf("Enter text for word frequency analysis (max %d characters):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    int word_count = process_input(input, words, MAX_WORD_COUNT);
    
    if (word_count == 0) {
        printf("No valid words found in the input.\n");
        return 1;
    }
    
    print_word_stats(words, word_count);
    
    return 0;
}