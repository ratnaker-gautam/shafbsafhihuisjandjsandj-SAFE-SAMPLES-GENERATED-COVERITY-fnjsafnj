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
    int char_count = 0;
    int word_lengths[MAX_WORDS] = {0};
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    
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
    
    if (input_len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char current_word[MAX_WORD_LEN];
    int word_pos = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (isalnum((unsigned char)c) || c == '\'' || c == '-') {
            if (word_pos < MAX_WORD_LEN - 1) {
                current_word[word_pos++] = c;
                in_word = 1;
            }
        } else {
            if (in_word && word_pos > 0) {
                if (word_count < MAX_WORDS) {
                    current_word[word_pos] = '\0';
                    strncpy(words[word_count], current_word, MAX_WORD_LEN - 1);
                    words[word_count][MAX_WORD_LEN - 1] = '\0';
                    word_lengths[word_count] = word_pos;
                    word_count++;
                    char_count += word_pos;
                }
                word_pos = 0;
                in_word = 0;
            }
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    printf("\nText analysis results:\n");
    printf("Total characters: %zu\n", input_len);
    printf("Total words: %d\n", word_count);
    printf("Average word length: %.2f\n", (float)char_count / word_count);
    
    printf("\nWords found:\n");
    for (int i = 0; i < word_count; i++) {
        if (i < 20) {
            printf("%d. %s (%d chars)\n", i + 1, words[i], word_lengths[i]);
        } else if (i == 20) {
            printf("... and %d more words\n", word_count - 20);
            break;
        }
    }
    
    int longest_idx = 0;
    int shortest_idx = 0;
    for (int i = 1; i < word_count; i++) {
        if (word_lengths[i] > word_lengths[longest_idx]) {
            longest_idx = i;
        }
        if (word_lengths[i] < word_lengths[shortest_idx]) {
            shortest_idx = i;
        }
    }
    
    printf("\nLongest word: %s (%d characters)\n", words[longest_idx], word_lengths[longest_idx]);
    printf("Shortest word: %s (%d characters)\n", words[shortest_idx], word_lengths[shortest_idx]);
    
    char search_term[MAX_WORD_LEN];
    printf("\nEnter a word to search for: ");
    
    if (fgets(search_term, sizeof(search_term), stdin) == NULL) {
        fprintf(stderr, "Error reading search term\n");
        return 1;
    }
    
    size_t search_len = strlen(search_term);
    if (search_len > 0 && search_term[search_len - 1] == '\n') {
        search_term[search_len - 1] = '\0';
        search_len--;
    }
    
    if (search_len == 0) {
        printf("No search term provided\n");
        return 1;
    }
    
    int found_count = 0;
    printf("Occurrences of '%s':\n", search_term);
    for (int i = 0; i < word_count; i++) {
        if (strcmp(words[i], search_term) == 0) {
            printf("  Found at position %d\n", i + 1);
            found_count++;
        }
    }
    
    if (found_count == 0) {
        printf("  No occurrences found\n");
    } else {
        printf("  Total occurrences: %d\n", found_count);
    }
    
    return 0;
}