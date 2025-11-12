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
        return 1;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int in_word = 0;
    int current_word_start = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha((unsigned char)input[i])) {
            if (!in_word) {
                current_word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                int word_len = i - current_word_start;
                
                if (word_count >= MAX_WORDS) {
                    fprintf(stderr, "Too many words\n");
                    return 1;
                }
                
                if (word_len >= 64) {
                    fprintf(stderr, "Word too long\n");
                    return 1;
                }
                
                strncpy(words[word_count], &input[current_word_start], word_len);
                words[word_count][word_len] = '\0';
                
                for (int j = 0; j < word_len; j++) {
                    words[word_count][j] = tolower((unsigned char)words[word_count][j]);
                }
                
                word_lengths[word_count] = word_len;
                total_chars += word_len;
                word_count++;
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
    printf("Total characters in words: %d\n", total_chars);
    printf("Average word length: %.2f\n", (double)total_chars / word_count);
    
    printf("\nWords sorted by length:\n");
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (word_lengths[j] > word_lengths[j + 1]) {
                int temp_len = word_lengths[j];
                word_lengths[j] = word_lengths[j + 1];
                word_lengths[j + 1] = temp_len;
                
                char temp_word[64];
                strcpy(temp_word, words[j]);
                strcpy(words[j], words[j + 1]);
                strcpy(words[j + 1], temp_word);
            }
        }
    }
    
    for (int i = 0; i < word_count; i++) {
        printf("%d. %s (%d)\n", i + 1, words[i], word_lengths[i]);
    }
    
    printf("\nWord frequency analysis:\n");
    int processed[MAX_WORDS] = {0};
    
    for (int i = 0; i < word_count; i++) {
        if (processed[i]) continue;
        
        int count = 1;
        for (int j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                count++;
                processed[j] = 1;
            }
        }
        
        printf("'%s': %d occurrence(s)\n", words[i], count);
        processed[i] = 1;
    }
    
    return 0;
}