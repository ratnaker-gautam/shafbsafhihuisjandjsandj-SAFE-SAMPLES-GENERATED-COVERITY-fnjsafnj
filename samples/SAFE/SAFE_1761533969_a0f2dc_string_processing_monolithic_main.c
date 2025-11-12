//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512
#define MAX_WORD_LEN 64

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    
    printf("Enter text: ");
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
    
    if (input_len >= sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *ptr = input;
    while (*ptr != '\0' && word_count < MAX_WORDS) {
        while (*ptr != '\0' && isspace((unsigned char)*ptr)) {
            ptr++;
        }
        
        if (*ptr == '\0') {
            break;
        }
        
        char *word_start = ptr;
        size_t word_len = 0;
        
        while (*ptr != '\0' && !isspace((unsigned char)*ptr) && word_len < MAX_WORD_LEN - 1) {
            ptr++;
            word_len++;
        }
        
        if (word_len >= MAX_WORD_LEN - 1) {
            fprintf(stderr, "Word too long\n");
            return 1;
        }
        
        if (word_len > 0) {
            memcpy(words[word_count], word_start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 1;
    }
    
    printf("Found %d words:\n", word_count);
    for (int i = 0; i < word_count; i++) {
        printf("%d: \"%s\"\n", i + 1, words[i]);
    }
    
    int total_chars = 0;
    int longest_word_len = 0;
    int shortest_word_len = MAX_WORD_LEN;
    
    for (int i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        total_chars += (int)len;
        
        if ((int)len > longest_word_len) {
            longest_word_len = (int)len;
        }
        
        if ((int)len < shortest_word_len) {
            shortest_word_len = (int)len;
        }
    }
    
    printf("\nStatistics:\n");
    printf("Total words: %d\n", word_count);
    printf("Total characters: %d\n", total_chars);
    printf("Average word length: %.2f\n", word_count > 0 ? (double)total_chars / word_count : 0.0);
    printf("Longest word length: %d\n", longest_word_len);
    printf("Shortest word length: %d\n", shortest_word_len);
    
    char search_word[MAX_WORD_LEN];
    printf("\nEnter a word to search for: ");
    if (fgets(search_word, sizeof(search_word), stdin) == NULL) {
        fprintf(stderr, "Error reading search word\n");
        return 1;
    }
    
    size_t search_len = strlen(search_word);
    if (search_len > 0 && search_word[search_len - 1] == '\n') {
        search_word[search_len - 1] = '\0';
        search_len--;
    }
    
    if (search_len == 0) {
        printf("No search word provided\n");
        return 1;
    }
    
    int found_count = 0;
    for (int i = 0; i < word_count; i++) {
        if (strcmp(words[i], search_word) == 0) {
            found_count++;
        }
    }
    
    if (found_count > 0) {
        printf("Word '%s' found %d time(s)\n", search_word, found_count);
    } else {
        printf("Word '%s' not found\n", search_word);
    }
    
    return 0;
}