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
    int in_word = 0;
    
    printf("Enter text to analyze: ");
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
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha((unsigned char)input[i])) {
            if (!in_word) {
                in_word = 1;
                word_length = 0;
            }
            if (word_count < MAX_WORDS && word_length < 63) {
                words[word_count][word_length++] = tolower((unsigned char)input[i]);
            }
        } else {
            if (in_word) {
                if (word_count < MAX_WORDS && word_length > 0) {
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
        return 0;
    }
    
    printf("\nWord frequency analysis:\n");
    printf("Total words: %d\n", word_count);
    
    int processed[MAX_WORDS] = {0};
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        if (processed[i]) continue;
        
        int count = 1;
        for (int j = i + 1; j < word_count; j++) {
            if (!processed[j] && strcmp(words[i], words[j]) == 0) {
                count++;
                processed[j] = 1;
            }
        }
        
        printf("'%s': %d occurrence%s\n", words[i], count, count == 1 ? "" : "s");
        unique_count++;
        processed[i] = 1;
    }
    
    printf("Unique words: %d\n", unique_count);
    
    int total_length = 0;
    int shortest_len = 64;
    int longest_len = 0;
    
    for (int i = 0; i < word_count; i++) {
        int len = (int)strlen(words[i]);
        total_length += len;
        if (len < shortest_len) shortest_len = len;
        if (len > longest_len) longest_len = len;
    }
    
    double avg_length = (double)total_length / word_count;
    printf("Average word length: %.2f characters\n", avg_length);
    printf("Shortest word: %d characters\n", shortest_len);
    printf("Longest word: %d characters\n", longest_len);
    
    return 0;
}