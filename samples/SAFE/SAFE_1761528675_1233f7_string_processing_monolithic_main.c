//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int char_count = 0;
    int word_lengths[MAX_WORDS];
    
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
        printf("Empty input provided\n");
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
            
            if (current_word_len < MAX_WORD_LEN && word_count < MAX_WORDS) {
                words[word_count][current_word_len] = tolower((unsigned char)input[i]);
                current_word_len++;
            }
        } else {
            if (in_word) {
                if (current_word_len > 0 && word_count < MAX_WORDS) {
                    words[word_count][current_word_len] = '\0';
                    word_lengths[word_count] = current_word_len;
                    word_count++;
                    char_count += current_word_len;
                }
                in_word = 0;
                current_word_len = 0;
            }
        }
    }
    
    printf("\nText analysis results:\n");
    printf("Total characters: %zu\n", input_len);
    printf("Total words: %d\n", word_count);
    
    if (word_count > 0) {
        printf("Average word length: %.2f\n", (float)char_count / word_count);
        
        int max_len = 0;
        int min_len = MAX_WORD_LEN;
        
        for (int i = 0; i < word_count; i++) {
            if (word_lengths[i] > max_len) {
                max_len = word_lengths[i];
            }
            if (word_lengths[i] < min_len) {
                min_len = word_lengths[i];
            }
        }
        
        printf("Longest word length: %d\n", max_len);
        printf("Shortest word length: %d\n", min_len);
        
        printf("\nWords found:\n");
        for (int i = 0; i < word_count; i++) {
            if (i < 20) {
                printf("%s", words[i]);
                if (i < word_count - 1 && i < 19) {
                    printf(", ");
                }
            }
        }
        if (word_count > 20) {
            printf("... (%d more words)", word_count - 20);
        }
        printf("\n");
    }
    
    return 0;
}