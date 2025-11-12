//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][64];
    int word_count = 0;
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    if (len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *ptr = input;
    while (*ptr && word_count < MAX_WORDS) {
        while (*ptr && isspace((unsigned char)*ptr)) {
            ptr++;
        }
        
        if (!*ptr) break;
        
        char *start = ptr;
        while (*ptr && !isspace((unsigned char)*ptr)) {
            ptr++;
        }
        
        size_t word_len = ptr - start;
        if (word_len >= sizeof(words[0])) {
            fprintf(stderr, "Word too long\n");
            return 1;
        }
        
        memcpy(words[word_count], start, word_len);
        words[word_count][word_len] = '\0';
        word_count++;
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    printf("Found %d words:\n", word_count);
    for (int i = 0; i < word_count; i++) {
        printf("%d: \"%s\"\n", i + 1, words[i]);
    }
    
    int total_chars = 0;
    for (int i = 0; i < word_count; i++) {
        total_chars += strlen(words[i]);
    }
    
    printf("Statistics:\n");
    printf("Total words: %d\n", word_count);
    printf("Total characters (excluding spaces): %d\n", total_chars);
    printf("Average word length: %.2f\n", word_count > 0 ? (float)total_chars / word_count : 0.0);
    
    char longest_word[64] = "";
    char shortest_word[64] = "";
    size_t max_len = 0;
    size_t min_len = 64;
    
    for (int i = 0; i < word_count; i++) {
        size_t current_len = strlen(words[i]);
        if (current_len > max_len) {
            max_len = current_len;
            strncpy(longest_word, words[i], sizeof(longest_word) - 1);
            longest_word[sizeof(longest_word) - 1] = '\0';
        }
        if (current_len < min_len) {
            min_len = current_len;
            strncpy(shortest_word, words[i], sizeof(shortest_word) - 1);
            shortest_word[sizeof(shortest_word) - 1] = '\0';
        }
    }
    
    printf("Longest word: \"%s\" (%zu characters)\n", longest_word, max_len);
    printf("Shortest word: \"%s\" (%zu characters)\n", shortest_word, min_len);
    
    return 0;
}