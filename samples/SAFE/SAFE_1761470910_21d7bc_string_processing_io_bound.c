//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
    int in_word = 0;
    int word_len = 0;
    
    while (*ptr != '\0' && word_count < MAX_WORDS) {
        if (isalpha((unsigned char)*ptr)) {
            if (!in_word) {
                in_word = 1;
                word_len = 0;
            }
            
            if (word_len < MAX_WORD_LEN) {
                words[word_count][word_len] = *ptr;
                word_len++;
            }
        } else {
            if (in_word) {
                if (word_len > 0 && word_len <= MAX_WORD_LEN) {
                    words[word_count][word_len] = '\0';
                    word_count++;
                }
                in_word = 0;
            }
        }
        ptr++;
    }
    
    if (in_word && word_len > 0 && word_len <= MAX_WORD_LEN && word_count < MAX_WORDS) {
        words[word_count][word_len] = '\0';
        word_count++;
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    printf("Found %d words:\n", word_count);
    for (int i = 0; i < word_count; i++) {
        printf("%d: %s\n", i + 1, words[i]);
    }
    
    printf("\nProcessing words...\n");
    
    for (int i = 0; i < word_count; i++) {
        size_t wlen = strlen(words[i]);
        for (size_t j = 0; j < wlen; j++) {
            if (islower((unsigned char)words[i][j])) {
                words[i][j] = toupper((unsigned char)words[i][j]);
            } else if (isupper((unsigned char)words[i][j])) {
                words[i][j] = tolower((unsigned char)words[i][j]);
            }
        }
    }
    
    printf("Processed words (case swapped):\n");
    for (int i = 0; i < word_count; i++) {
        printf("%d: %s\n", i + 1, words[i]);
    }
    
    return 0;
}