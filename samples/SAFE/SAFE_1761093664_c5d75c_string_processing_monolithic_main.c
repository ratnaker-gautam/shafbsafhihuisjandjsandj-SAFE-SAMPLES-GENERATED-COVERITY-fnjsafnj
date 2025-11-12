//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: string_processing
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
    int word_lengths[MAX_WORDS];
    int total_chars = 0;
    
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
    
    char *ptr = input;
    while (*ptr && word_count < MAX_WORDS) {
        while (*ptr && isspace((unsigned char)*ptr)) {
            ptr++;
        }
        
        if (!*ptr) break;
        
        char *word_start = ptr;
        while (*ptr && !isspace((unsigned char)*ptr)) {
            ptr++;
        }
        
        size_t word_len = ptr - word_start;
        if (word_len >= sizeof(words[0])) {
            word_len = sizeof(words[0]) - 1;
        }
        
        if (word_len > 0) {
            memcpy(words[word_count], word_start, word_len);
            words[word_count][word_len] = '\0';
            word_lengths[word_count] = (int)word_len;
            total_chars += (int)word_len;
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    printf("\nText analysis results:\n");
    printf("Total words: %d\n", word_count);
    printf("Total characters (letters only): %d\n", total_chars);
    printf("Average word length: %.2f\n", (double)total_chars / word_count);
    
    printf("\nWords sorted by length:\n");
    int sorted_indices[MAX_WORDS];
    for (int i = 0; i < word_count; i++) {
        sorted_indices[i] = i;
    }
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = i + 1; j < word_count; j++) {
            if (word_lengths[sorted_indices[i]] > word_lengths[sorted_indices[j]]) {
                int temp = sorted_indices[i];
                sorted_indices[i] = sorted_indices[j];
                sorted_indices[j] = temp;
            }
        }
    }
    
    for (int i = 0; i < word_count; i++) {
        int idx = sorted_indices[i];
        printf("%d. %s (%d characters)\n", i + 1, words[idx], word_lengths[idx]);
    }
    
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
        
        printf("'%s' appears %d time%s\n", words[i], count, count == 1 ? "" : "s");
        processed[i] = 1;
    }
    
    return 0;
}