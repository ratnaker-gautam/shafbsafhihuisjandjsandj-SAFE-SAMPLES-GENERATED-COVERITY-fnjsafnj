//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][64];
    int word_count = 0;
    int word_length = 0;
    
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
    
    for (size_t i = 0; i <= input_len && word_count < MAX_WORDS; i++) {
        if (i < input_len && isalpha((unsigned char)input[i])) {
            if (word_length < 63) {
                words[word_count][word_length++] = tolower((unsigned char)input[i]);
            }
        } else {
            if (word_length > 0) {
                words[word_count][word_length] = '\0';
                word_count++;
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
    int total_length = 0;
    int longest_length = 0;
    int shortest_length = 1000;
    
    for (int i = 0; i < word_count; i++) {
        int current_length = strlen(words[i]);
        total_length += current_length;
        
        if (current_length > longest_length) {
            longest_length = current_length;
        }
        if (current_length < shortest_length) {
            shortest_length = current_length;
        }
        
        int is_unique = 1;
        for (int j = 0; j < i; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                is_unique = 0;
                break;
            }
        }
        
        if (is_unique) {
            unique_count++;
        }
    }
    
    printf("Unique words: %d\n", unique_count);
    printf("Total words: %d\n", word_count);
    printf("Average word length: %.2f\n", (double)total_length / word_count);
    printf("Longest word length: %d\n", longest_length);
    printf("Shortest word length: %d\n", shortest_length);
    
    printf("\nWord frequency:\n");
    for (int i = 0; i < word_count; i++) {
        int counted = 0;
        for (int j = 0; j < i; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                counted = 1;
                break;
            }
        }
        
        if (!counted) {
            int frequency = 1;
            for (int j = i + 1; j < word_count; j++) {
                if (strcmp(words[i], words[j]) == 0) {
                    frequency++;
                }
            }
            printf("%s: %d\n", words[i], frequency);
        }
    }
    
    return 0;
}