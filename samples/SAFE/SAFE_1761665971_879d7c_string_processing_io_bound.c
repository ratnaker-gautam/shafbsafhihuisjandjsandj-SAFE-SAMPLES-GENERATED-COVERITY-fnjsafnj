//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
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
        printf("Empty input\n");
        return 0;
    }
    
    if (len > MAX_INPUT_LEN) {
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
        if (word_len > MAX_WORD_LEN) {
            fprintf(stderr, "Word too long\n");
            return 1;
        }
        
        strncpy(words[word_count], word_start, word_len);
        words[word_count][word_len] = '\0';
        word_count++;
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    for (int i = 0; i < word_count; i++) {
        size_t current_len = strlen(words[i]);
        if (current_len == 0) continue;
        
        for (size_t j = 0; j < current_len; j++) {
            words[i][j] = toupper((unsigned char)words[i][j]);
        }
        
        printf("Word %d: %s (length: %zu)\n", i + 1, words[i], current_len);
    }
    
    printf("\nWord statistics:\n");
    printf("Total words: %d\n", word_count);
    
    size_t total_chars = 0;
    size_t longest_len = 0;
    size_t shortest_len = MAX_WORD_LEN;
    
    for (int i = 0; i < word_count; i++) {
        size_t current_len = strlen(words[i]);
        total_chars += current_len;
        
        if (current_len > longest_len) {
            longest_len = current_len;
        }
        if (current_len < shortest_len) {
            shortest_len = current_len;
        }
    }
    
    printf("Total characters: %zu\n", total_chars);
    printf("Average word length: %.2f\n", word_count > 0 ? (double)total_chars / word_count : 0.0);
    printf("Longest word: %zu characters\n", longest_len);
    printf("Shortest word: %zu characters\n", shortest_len);
    
    return 0;
}