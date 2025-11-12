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
        return 1;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    
    int total_length = 0;
    int shortest_len = 64;
    int longest_len = 0;
    char shortest_words[MAX_WORDS][64];
    char longest_words[MAX_WORDS][64];
    int shortest_count = 0;
    int longest_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int len = strlen(words[i]);
        total_length += len;
        
        if (len < shortest_len) {
            shortest_len = len;
            shortest_count = 0;
        }
        
        if (len > longest_len) {
            longest_len = len;
            longest_count = 0;
        }
        
        if (len == shortest_len && shortest_count < MAX_WORDS) {
            strcpy(shortest_words[shortest_count], words[i]);
            shortest_count++;
        }
        
        if (len == longest_len && longest_count < MAX_WORDS) {
            strcpy(longest_words[longest_count], words[i]);
            longest_count++;
        }
    }
    
    double avg_length = (double)total_length / word_count;
    
    printf("Average word length: %.2f\n", avg_length);
    
    printf("Shortest words (%d characters): ", shortest_len);
    for (int i = 0; i < shortest_count; i++) {
        printf("%s", shortest_words[i]);
        if (i < shortest_count - 1) printf(", ");
    }
    printf("\n");
    
    printf("Longest words (%d characters): ", longest_len);
    for (int i = 0; i < longest_count; i++) {
        printf("%s", longest_words[i]);
        if (i < longest_count - 1) printf(", ");
    }
    printf("\n");
    
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
        
        printf("'%s': %d occurrence%s\n", words[i], count, count == 1 ? "" : "s");
        processed[i] = 1;
    }
    
    return 0;
}