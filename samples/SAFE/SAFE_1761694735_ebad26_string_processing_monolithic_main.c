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
    
    for (int i = 0; i < input_len; i++) {
        if (input[i] == '\n') {
            line_count++;
        }
    }
    
    int in_word = 0;
    int word_start = 0;
    int current_word_len = 0;
    
    for (int i = 0; i <= input_len && word_count < MAX_WORDS; i++) {
        if (i < input_len && isalpha((unsigned char)input[i])) {
            if (!in_word) {
                in_word = 1;
                word_start = i;
                current_word_len = 0;
            }
            current_word_len++;
        } else {
            if (in_word) {
                if (current_word_len > 0 && current_word_len <= MAX_WORD_LEN) {
                    if (word_count < MAX_WORDS) {
                        strncpy(words[word_count], &input[word_start], current_word_len);
                        words[word_count][current_word_len] = '\0';
                        
                        for (int j = 0; j < current_word_len; j++) {
                            words[word_count][j] = tolower((unsigned char)words[word_count][j]);
                        }
                        
                        word_count++;
                    }
                }
                in_word = 0;
            }
        }
    }
    
    printf("\nText analysis results:\n");
    printf("Characters: %d\n", char_count);
    printf("Lines: %d\n", line_count);
    printf("Words: %d\n", word_count);
    
    if (word_count > 0) {
        printf("\nWords found (lowercase):\n");
        for (int i = 0; i < word_count; i++) {
            if (i < 10) {
                printf("%d. %s\n", i + 1, words[i]);
            } else {
                printf("%d. %s\n", i + 1, words[i]);
                if (i == 14) {
                    printf("... (showing first 15 words)\n");
                    break;
                }
            }
        }
    }
    
    int freq[MAX_WORDS] = {0};
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                freq[j]++;
                found = 1;
                break;
            }
        }
        if (!found && unique_count < MAX_WORDS) {
            strcpy(words[unique_count], words[i]);
            freq[unique_count] = 1;
            unique_count++;
        }
    }
    
    if (unique_count > 0) {
        printf("\nWord frequency (top 5):\n");
        for (int i = 0; i < unique_count && i < 5; i++) {
            int max_freq = 0;
            int max_index = 0;
            for (int j = 0; j < unique_count; j++) {
                if (freq[j] > max_freq) {
                    max_freq = freq[j];
                    max_index = j;
                }
            }
            printf("%s: %d\n", words[max_index], max_freq);
            freq[max_index] = 0;
        }
    }
    
    return 0;
}