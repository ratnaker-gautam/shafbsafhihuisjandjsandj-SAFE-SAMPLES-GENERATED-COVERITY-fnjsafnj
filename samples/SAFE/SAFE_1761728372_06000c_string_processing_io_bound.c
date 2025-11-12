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
    int char_count = 0;
    int line_count = 0;
    
    printf("Enter text (max %d characters):\n", MAX_INPUT_LEN);
    
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
        printf("Empty input provided\n");
        return 0;
    }
    
    if (len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *ptr = input;
    int in_word = 0;
    int current_word_len = 0;
    
    while (*ptr != '\0' && word_count < MAX_WORDS) {
        if (isspace((unsigned char)*ptr)) {
            if (in_word) {
                if (current_word_len > 0 && current_word_len <= MAX_WORD_LEN) {
                    words[word_count][current_word_len] = '\0';
                    word_count++;
                }
                in_word = 0;
                current_word_len = 0;
            }
            if (*ptr == '\n') {
                line_count++;
            }
        } else {
            if (!in_word) {
                in_word = 1;
                current_word_len = 0;
            }
            if (current_word_len < MAX_WORD_LEN) {
                words[word_count][current_word_len] = *ptr;
                current_word_len++;
            }
        }
        char_count++;
        ptr++;
    }
    
    if (in_word && current_word_len > 0 && current_word_len <= MAX_WORD_LEN && word_count < MAX_WORDS) {
        words[word_count][current_word_len] = '\0';
        word_count++;
    }
    
    printf("\nText analysis results:\n");
    printf("Characters: %d\n", char_count);
    printf("Words: %d\n", word_count);
    printf("Lines: %d\n", line_count + 1);
    
    if (word_count > 0) {
        printf("\nWords found:\n");
        for (int i = 0; i < word_count; i++) {
            if (i < word_count) {
                printf("%d: %s\n", i + 1, words[i]);
            }
        }
        
        printf("\nWord lengths:\n");
        for (int i = 0; i < word_count; i++) {
            size_t word_len = strlen(words[i]);
            if (word_len > 0) {
                printf("%s: %zu characters\n", words[i], word_len);
            }
        }
    }
    
    char search_word[MAX_WORD_LEN + 1];
    printf("\nEnter a word to search for: ");
    
    if (fgets(search_word, sizeof(search_word), stdin) == NULL) {
        fprintf(stderr, "Error reading search word\n");
        return 1;
    }
    
    len = strlen(search_word);
    if (len > 0 && search_word[len - 1] == '\n') {
        search_word[len - 1] = '\0';
        len--;
    }
    
    if (len > 0) {
        int found_count = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], search_word) == 0) {
                found_count++;
            }
        }
        printf("The word '%s' appears %d time(s)\n", search_word, found_count);
    }
    
    return 0;
}