//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORD_LEN 64

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_INPUT_LEN / 2][MAX_WORD_LEN + 1];
    int word_count = 0;
    
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
    
    char *pos = input;
    int in_word = 0;
    int word_start = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        if (isalnum((unsigned char)input[i])) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len > MAX_WORD_LEN) {
                    fprintf(stderr, "Word too long\n");
                    return 1;
                }
                
                if (word_count >= (int)(sizeof(words) / sizeof(words[0]))) {
                    fprintf(stderr, "Too many words\n");
                    return 1;
                }
                
                strncpy(words[word_count], &input[word_start], word_len);
                words[word_count][word_len] = '\0';
                word_count++;
                in_word = 0;
            }
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    printf("\nFound %d words:\n", word_count);
    for (int i = 0; i < word_count; i++) {
        printf("%d: \"%s\"\n", i + 1, words[i]);
    }
    
    printf("\nProcessing words...\n");
    
    int total_length = 0;
    int longest_word_index = 0;
    int shortest_word_index = 0;
    
    for (int i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        total_length += (int)len;
        
        if (strlen(words[i]) > strlen(words[longest_word_index])) {
            longest_word_index = i;
        }
        if (strlen(words[i]) < strlen(words[shortest_word_index])) {
            shortest_word_index = i;
        }
    }
    
    printf("Total characters in words: %d\n", total_length);
    printf("Average word length: %.2f\n", (float)total_length / word_count);
    printf("Longest word: \"%s\" (%zu characters)\n", 
           words[longest_word_index], strlen(words[longest_word_index]));
    printf("Shortest word: \"%s\" (%zu characters)\n", 
           words[shortest_word_index], strlen(words[shortest_word_index]));
    
    printf("\nWords in reverse order:\n");
    for (int i = word_count - 1; i >= 0; i--) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    return 0;
}