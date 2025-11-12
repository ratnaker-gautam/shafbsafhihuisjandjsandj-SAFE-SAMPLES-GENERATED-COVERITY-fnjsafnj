//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    
    printf("Enter text: ");
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
    
    if (len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int in_word = 0;
    int word_start = 0;
    
    for (size_t i = 0; i <= len; i++) {
        if (i < len && isalpha((unsigned char)input[i])) {
            if (!in_word) {
                in_word = 1;
                word_start = i;
            }
        } else {
            if (in_word) {
                int word_length = i - word_start;
                if (word_length >= MAX_WORD_LEN) {
                    fprintf(stderr, "Word too long\n");
                    return 1;
                }
                
                if (word_count >= MAX_WORDS) {
                    fprintf(stderr, "Too many words\n");
                    return 1;
                }
                
                for (int j = 0; j < word_length; j++) {
                    words[word_count][j] = tolower((unsigned char)input[word_start + j]);
                }
                words[word_count][word_length] = '\0';
                word_count++;
                in_word = 0;
            }
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (strcmp(words[j], words[j + 1]) > 0) {
                char temp[MAX_WORD_LEN];
                strncpy(temp, words[j], sizeof(temp));
                strncpy(words[j], words[j + 1], sizeof(words[j]));
                strncpy(words[j + 1], temp, sizeof(words[j + 1]));
            }
        }
    }
    
    printf("Sorted words:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    int unique_count = 0;
    for (int i = 0; i < word_count; i++) {
        if (i == 0 || strcmp(words[i], words[i - 1]) != 0) {
            unique_count++;
        }
    }
    
    printf("Total words: %d\n", word_count);
    printf("Unique words: %d\n", unique_count);
    
    return 0;
}