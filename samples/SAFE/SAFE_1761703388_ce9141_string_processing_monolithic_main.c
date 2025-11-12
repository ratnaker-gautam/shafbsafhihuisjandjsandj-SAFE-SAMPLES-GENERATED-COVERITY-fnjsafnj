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
    int word_length = 0;
    int in_word = 0;
    int i;
    
    printf("Enter text to process: ");
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
    
    for (i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha((unsigned char)input[i])) {
            if (!in_word) {
                in_word = 1;
                word_length = 0;
            }
            if (word_length < 63 && word_count < MAX_WORDS) {
                words[word_count][word_length++] = tolower((unsigned char)input[i]);
            }
        } else {
            if (in_word) {
                if (word_length > 0 && word_count < MAX_WORDS) {
                    words[word_count][word_length] = '\0';
                    word_count++;
                }
                in_word = 0;
                word_length = 0;
            }
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    
    int unique_count = 0;
    int j;
    
    for (i = 0; i < word_count; i++) {
        int is_unique = 1;
        
        for (j = 0; j < i; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                is_unique = 0;
                break;
            }
        }
        
        if (is_unique) {
            unique_count++;
            int count = 1;
            
            for (j = i + 1; j < word_count; j++) {
                if (strcmp(words[i], words[j]) == 0) {
                    count++;
                }
            }
            
            printf("Word: %-15s Count: %d\n", words[i], count);
        }
    }
    
    printf("\nTotal unique words: %d\n", unique_count);
    
    char longest_word[64] = "";
    char shortest_word[64] = "";
    size_t max_len = 0;
    size_t min_len = 64;
    
    for (i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        if (len > max_len) {
            max_len = len;
            strncpy(longest_word, words[i], sizeof(longest_word) - 1);
            longest_word[sizeof(longest_word) - 1] = '\0';
        }
        if (len < min_len && len > 0) {
            min_len = len;
            strncpy(shortest_word, words[i], sizeof(shortest_word) - 1);
            shortest_word[sizeof(shortest_word) - 1] = '\0';
        }
    }
    
    if (max_len > 0) {
        printf("Longest word: %s (%zu letters)\n", longest_word, max_len);
    }
    if (min_len < 64) {
        printf("Shortest word: %s (%zu letters)\n", shortest_word, min_len);
    }
    
    return 0;
}