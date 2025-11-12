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
    int line_count = 0;
    
    printf("Enter text (max %d characters):\n", MAX_INPUT_LEN);
    
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
    
    char_count = input_len;
    line_count = 1;
    
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
                    word_count++;
                }
                in_word = 0;
                current_word_len = 0;
            }
        }
    }
    
    printf("\nText Analysis Results:\n");
    printf("Characters: %d\n", char_count);
    printf("Lines: %d\n", line_count);
    printf("Words: %d\n", word_count);
    
    if (word_count > 0) {
        printf("\nWord List (lowercase):\n");
        for (int i = 0; i < word_count; i++) {
            if (i < 20) {
                printf("%s\n", words[i]);
            } else if (i == 20) {
                printf("... (showing first 20 words)\n");
                break;
            }
        }
    }
    
    int freq[MAX_WORDS] = {0};
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        if (freq[i] == -1) continue;
        
        int count = 1;
        for (int j = i + 1; j < word_count; j++) {
            if (freq[j] != -1 && strcmp(words[i], words[j]) == 0) {
                count++;
                freq[j] = -1;
            }
        }
        freq[i] = count;
        unique_count++;
    }
    
    printf("\nUnique words: %d\n", unique_count);
    
    if (unique_count > 0) {
        printf("Most frequent words:\n");
        int printed = 0;
        for (int target = word_count; target > 0 && printed < 5; target--) {
            for (int i = 0; i < word_count && printed < 5; i++) {
                if (freq[i] == target) {
                    printf("%s: %d occurrences\n", words[i], freq[i]);
                    printed++;
                }
            }
        }
    }
    
    return 0;
}