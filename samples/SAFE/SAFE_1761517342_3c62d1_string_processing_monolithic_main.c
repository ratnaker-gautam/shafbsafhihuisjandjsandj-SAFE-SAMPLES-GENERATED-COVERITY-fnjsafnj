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
    int line_count = 0;
    
    printf("Enter text (max %d characters):\n", MAX_INPUT_LEN - 1);
    
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
    
    char_count = input_len;
    line_count = 1;
    
    char temp_word[MAX_WORD_LEN];
    int temp_index = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (isalnum((unsigned char)c) || c == '_') {
            if (temp_index < MAX_WORD_LEN - 1) {
                temp_word[temp_index++] = c;
            }
            in_word = 1;
        } else {
            if (in_word) {
                if (temp_index > 0 && word_count < MAX_WORDS) {
                    temp_word[temp_index] = '\0';
                    strncpy(words[word_count], temp_word, MAX_WORD_LEN - 1);
                    words[word_count][MAX_WORD_LEN - 1] = '\0';
                    word_count++;
                }
                temp_index = 0;
                in_word = 0;
            }
        }
    }
    
    printf("\nText Analysis Results:\n");
    printf("Characters: %d\n", char_count);
    printf("Lines: %d\n", line_count);
    printf("Words: %d\n", word_count);
    
    if (word_count > 0) {
        printf("\nWord List:\n");
        for (int i = 0; i < word_count; i++) {
            if (i < 20) {
                printf("%d. %s\n", i + 1, words[i]);
            } else if (i == 20) {
                printf("... (showing first 20 words)\n");
                break;
            }
        }
        
        int max_len = 0;
        int min_len = MAX_WORD_LEN;
        int total_len = 0;
        
        for (int i = 0; i < word_count; i++) {
            int len = strlen(words[i]);
            total_len += len;
            if (len > max_len) max_len = len;
            if (len < min_len) min_len = len;
        }
        
        printf("\nWord Statistics:\n");
        printf("Average word length: %.2f\n", word_count > 0 ? (double)total_len / word_count : 0.0);
        printf("Longest word: %d characters\n", max_len);
        printf("Shortest word: %d characters\n", min_len);
    }
    
    char search_term[MAX_WORD_LEN];
    printf("\nEnter a word to search for: ");
    if (fgets(search_term, sizeof(search_term), stdin) != NULL) {
        size_t search_len = strlen(search_term);
        if (search_len > 0 && search_term[search_len - 1] == '\n') {
            search_term[search_len - 1] = '\0';
            search_len--;
        }
        
        if (search_len > 0) {
            int occurrences = 0;
            for (int i = 0; i < word_count; i++) {
                if (strcmp(words[i], search_term) == 0) {
                    occurrences++;
                }
            }
            printf("The word '%s' appears %d time(s)\n", search_term, occurrences);
        }
    }
    
    return 0;
}