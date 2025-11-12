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
    int char_count = 0;
    int word_lengths[MAX_WORDS] = {0};
    
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
        return 1;
    }
    
    if (input_len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char current_word[MAX_WORD_LEN];
    int word_pos = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (isalnum((unsigned char)c) || c == '\'' || c == '-') {
            if (word_pos < MAX_WORD_LEN - 1) {
                current_word[word_pos++] = c;
                in_word = 1;
            }
        } else {
            if (in_word && word_pos > 0) {
                if (word_count < MAX_WORDS) {
                    current_word[word_pos] = '\0';
                    strncpy(words[word_count], current_word, MAX_WORD_LEN - 1);
                    words[word_count][MAX_WORD_LEN - 1] = '\0';
                    word_lengths[word_count] = word_pos;
                    word_count++;
                    char_count += word_pos;
                }
                word_pos = 0;
                in_word = 0;
            }
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    printf("\nProcessing results:\n");
    printf("Total words: %d\n", word_count);
    printf("Total characters in words: %d\n", char_count);
    printf("Average word length: %.2f\n", (float)char_count / word_count);
    
    printf("\nWords sorted by length:\n");
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = i + 1; j < word_count; j++) {
            if (word_lengths[i] > word_lengths[j]) {
                int temp_len = word_lengths[i];
                word_lengths[i] = word_lengths[j];
                word_lengths[j] = temp_len;
                
                char temp_word[MAX_WORD_LEN];
                strncpy(temp_word, words[i], MAX_WORD_LEN - 1);
                temp_word[MAX_WORD_LEN - 1] = '\0';
                strncpy(words[i], words[j], MAX_WORD_LEN - 1);
                words[i][MAX_WORD_LEN - 1] = '\0';
                strncpy(words[j], temp_word, MAX_WORD_LEN - 1);
                words[j][MAX_WORD_LEN - 1] = '\0';
            }
        }
    }
    
    for (int i = 0; i < word_count; i++) {
        if (i < 20) {
            printf("%d. %s (%d characters)\n", i + 1, words[i], word_lengths[i]);
        } else if (i == 20) {
            printf("... (showing first 20 words)\n");
            break;
        }
    }
    
    int max_length = 0;
    int min_length = MAX_WORD_LEN;
    for (int i = 0; i < word_count; i++) {
        if (word_lengths[i] > max_length) max_length = word_lengths[i];
        if (word_lengths[i] < min_length) min_length = word_lengths[i];
    }
    
    printf("\nWord length statistics:\n");
    printf("Shortest word: %d characters\n", min_length);
    printf("Longest word: %d characters\n", max_length);
    
    int length_counts[MAX_WORD_LEN] = {0};
    for (int i = 0; i < word_count; i++) {
        if (word_lengths[i] < MAX_WORD_LEN) {
            length_counts[word_lengths[i]]++;
        }
    }
    
    printf("\nWord length distribution:\n");
    for (int i = 1; i <= max_length && i < MAX_WORD_LEN; i++) {
        if (length_counts[i] > 0) {
            printf("%2d characters: %d words\n", i, length_counts[i]);
        }
    }
    
    return 0;
}