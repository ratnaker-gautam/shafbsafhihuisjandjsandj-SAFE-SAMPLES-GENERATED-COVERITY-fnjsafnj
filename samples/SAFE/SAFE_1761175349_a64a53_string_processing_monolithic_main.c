//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int word_lengths[MAX_WORDS];
    int total_chars = 0;
    
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
        printf("No input provided\n");
        return 1;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *ptr = input;
    int in_word = 0;
    int current_word_len = 0;
    
    while (*ptr != '\0' && word_count < MAX_WORDS) {
        if (isalnum((unsigned char)*ptr)) {
            if (!in_word) {
                in_word = 1;
                current_word_len = 0;
            }
            
            if (current_word_len < MAX_WORD_LEN) {
                words[word_count][current_word_len] = *ptr;
                current_word_len++;
            }
        } else {
            if (in_word) {
                if (current_word_len > 0 && current_word_len <= MAX_WORD_LEN) {
                    words[word_count][current_word_len] = '\0';
                    word_lengths[word_count] = current_word_len;
                    total_chars += current_word_len;
                    word_count++;
                }
                in_word = 0;
            }
        }
        ptr++;
    }
    
    if (in_word && word_count < MAX_WORDS && current_word_len > 0 && current_word_len <= MAX_WORD_LEN) {
        words[word_count][current_word_len] = '\0';
        word_lengths[word_count] = current_word_len;
        total_chars += current_word_len;
        word_count++;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    printf("\nWord analysis:\n");
    printf("Total words: %d\n", word_count);
    printf("Total characters in words: %d\n", total_chars);
    
    double avg_length = (double)total_chars / word_count;
    printf("Average word length: %.2f\n", avg_length);
    
    printf("\nWords sorted by length:\n");
    
    int indices[MAX_WORDS];
    for (int i = 0; i < word_count; i++) {
        indices[i] = i;
    }
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (word_lengths[indices[j]] > word_lengths[indices[j + 1]]) {
                int temp = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = temp;
            }
        }
    }
    
    for (int i = 0; i < word_count; i++) {
        int idx = indices[i];
        printf("%d. %s (length: %d)\n", i + 1, words[idx], word_lengths[idx]);
    }
    
    printf("\nWord frequency analysis:\n");
    
    int processed[MAX_WORDS] = {0};
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        if (processed[i]) continue;
        
        int count = 1;
        for (int j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                count++;
                processed[j] = 1;
            }
        }
        
        printf("'%s' appears %d time%s\n", words[i], count, count == 1 ? "" : "s");
        unique_count++;
        processed[i] = 1;
    }
    
    printf("Unique words: %d\n", unique_count);
    
    return 0;
}