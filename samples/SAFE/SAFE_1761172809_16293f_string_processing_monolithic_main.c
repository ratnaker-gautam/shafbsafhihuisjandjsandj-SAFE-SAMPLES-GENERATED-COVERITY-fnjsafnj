//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
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
    int word_length = 0;
    int in_word = 0;
    
    printf("Enter text to process: ");
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
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha((unsigned char)input[i])) {
            if (!in_word) {
                in_word = 1;
                word_length = 0;
            }
            if (word_count < MAX_WORDS && word_length < 63) {
                words[word_count][word_length++] = tolower((unsigned char)input[i]);
            }
        } else {
            if (in_word) {
                if (word_count < MAX_WORDS && word_length > 0) {
                    words[word_count][word_length] = '\0';
                    word_count++;
                }
                in_word = 0;
                word_length = 0;
            }
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    printf("Found %d words:\n", word_count);
    for (int i = 0; i < word_count; i++) {
        printf("%d: %s\n", i + 1, words[i]);
    }
    
    int unique_count = 0;
    printf("\nUnique words:\n");
    for (int i = 0; i < word_count; i++) {
        int is_unique = 1;
        for (int j = 0; j < i; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            printf("%s\n", words[i]);
            unique_count++;
        }
    }
    
    printf("\nTotal unique words: %d\n", unique_count);
    
    int freq_count = 0;
    printf("\nWord frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        int counted = 0;
        for (int j = 0; j < i; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                counted = 1;
                break;
            }
        }
        if (!counted) {
            int count = 1;
            for (int j = i + 1; j < word_count; j++) {
                if (strcmp(words[i], words[j]) == 0) {
                    count++;
                }
            }
            printf("%s: %d\n", words[i], count);
            freq_count++;
        }
    }
    
    return 0;
}