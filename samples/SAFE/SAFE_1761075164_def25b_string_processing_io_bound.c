//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORD_LEN 64

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_INPUT_LEN / 2][MAX_WORD_LEN + 1];
    int word_count = 0;
    
    printf("Enter text to process (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    if (len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int in_word = 0;
    int word_pos = 0;
    
    for (size_t i = 0; i <= len; i++) {
        if (i < len && isalpha((unsigned char)input[i])) {
            if (!in_word) {
                in_word = 1;
                word_pos = 0;
            }
            if (word_pos < MAX_WORD_LEN) {
                words[word_count][word_pos++] = tolower((unsigned char)input[i]);
            }
        } else {
            if (in_word) {
                if (word_pos > 0) {
                    words[word_count][word_pos] = '\0';
                    word_count++;
                    if (word_count >= (int)(sizeof(words) / sizeof(words[0]))) {
                        break;
                    }
                }
                in_word = 0;
            }
        }
    }
    
    printf("\nProcessing complete. Found %d words:\n", word_count);
    
    for (int i = 0; i < word_count; i++) {
        printf("Word %d: %s\n", i + 1, words[i]);
    }
    
    if (word_count > 0) {
        printf("\nWord statistics:\n");
        printf("Total words: %d\n", word_count);
        
        int total_chars = 0;
        int shortest = MAX_WORD_LEN;
        int longest = 0;
        
        for (int i = 0; i < word_count; i++) {
            int word_len = (int)strlen(words[i]);
            total_chars += word_len;
            if (word_len < shortest) shortest = word_len;
            if (word_len > longest) longest = word_len;
        }
        
        printf("Average word length: %.2f\n", word_count > 0 ? (double)total_chars / word_count : 0.0);
        printf("Shortest word: %d characters\n", shortest);
        printf("Longest word: %d characters\n", longest);
    }
    
    return 0;
}