//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][64];
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
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int in_word = 0;
    int current_word_len = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha((unsigned char)input[i])) {
            if (!in_word) {
                in_word = 1;
                current_word_len = 0;
            }
            if (current_word_len < 63 && word_count < MAX_WORDS) {
                words[word_count][current_word_len] = tolower((unsigned char)input[i]);
                current_word_len++;
            }
        } else {
            if (in_word && current_word_len > 0 && word_count < MAX_WORDS) {
                words[word_count][current_word_len] = '\0';
                word_lengths[word_count] = current_word_len;
                total_chars += current_word_len;
                word_count++;
            }
            in_word = 0;
            current_word_len = 0;
        }
    }
    
    printf("\nProcessing results:\n");
    printf("Total words: %d\n", word_count);
    printf("Total characters in words: %d\n", total_chars);
    
    if (word_count > 0) {
        double avg_length = (double)total_chars / word_count;
        printf("Average word length: %.2f\n", avg_length);
        
        printf("\nWords sorted by length:\n");
        
        for (int i = 0; i < word_count - 1; i++) {
            for (int j = i + 1; j < word_count; j++) {
                if (word_lengths[i] > word_lengths[j] || 
                   (word_lengths[i] == word_lengths[j] && strcmp(words[i], words[j]) > 0)) {
                    char temp_word[64];
                    strcpy(temp_word, words[i]);
                    strcpy(words[i], words[j]);
                    strcpy(words[j], temp_word);
                    
                    int temp_len = word_lengths[i];
                    word_lengths[i] = word_lengths[j];
                    word_lengths[j] = temp_len;
                }
            }
        }
        
        for (int i = 0; i < word_count; i++) {
            printf("%d. %s (%d)\n", i + 1, words[i], word_lengths[i]);
        }
        
        int unique_count = 0;
        printf("\nUnique words:\n");
        for (int i = 0; i < word_count; i++) {
            int is_unique = 1;
            for (int j = 0; j < i; j++) {
                if (strcmp(words[i], words[j]) == 0) {
                    is_unique = 0;
                    break;
                }
            }
            if (is_unique) {
                printf("%s ", words[i]);
                unique_count++;
            }
        }
        printf("\nUnique word count: %d\n", unique_count);
    }
    
    return 0;
}