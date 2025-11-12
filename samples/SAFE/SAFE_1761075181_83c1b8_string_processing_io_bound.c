//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: string_processing
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
                    if (word_count >= MAX_INPUT_LEN / 2) {
                        break;
                    }
                }
                in_word = 0;
            }
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("\nFound %d words:\n", word_count);
    for (int i = 0; i < word_count; i++) {
        printf("%d: %s\n", i + 1, words[i]);
    }
    
    printf("\nWord frequency analysis:\n");
    int processed[MAX_INPUT_LEN / 2] = {0};
    
    for (int i = 0; i < word_count; i++) {
        if (processed[i]) {
            continue;
        }
        
        int count = 1;
        for (int j = i + 1; j < word_count; j++) {
            if (!processed[j] && strcmp(words[i], words[j]) == 0) {
                count++;
                processed[j] = 1;
            }
        }
        
        printf("'%s': %d occurrence(s)\n", words[i], count);
        processed[i] = 1;
    }
    
    printf("\nWord lengths:\n");
    for (int i = 0; i < word_count; i++) {
        size_t word_len = strlen(words[i]);
        printf("'%s': %zu character(s)\n", words[i], word_len);
    }
    
    return 0;
}