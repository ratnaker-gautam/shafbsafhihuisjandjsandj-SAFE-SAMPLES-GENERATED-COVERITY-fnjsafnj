//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_INPUT_LEN + 1];
    int word_count = 0;
    int i, j, k;
    int in_word = 0;
    int word_start = 0;
    char temp[MAX_INPUT_LEN + 1];
    
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
        printf("Empty input\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha((unsigned char)input[i])) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                int word_len = i - word_start;
                if (word_len > 0 && word_count < MAX_WORDS) {
                    if (word_len > MAX_INPUT_LEN) {
                        fprintf(stderr, "Word too long\n");
                        return 1;
                    }
                    strncpy(words[word_count], &input[word_start], word_len);
                    words[word_count][word_len] = '\0';
                    word_count++;
                }
                in_word = 0;
            }
        }
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    for (i = 0; i < word_count - 1; i++) {
        for (j = 0; j < word_count - i - 1; j++) {
            if (strcmp(words[j], words[j + 1]) > 0) {
                strcpy(temp, words[j]);
                strcpy(words[j], words[j + 1]);
                strcpy(words[j + 1], temp);
            }
        }
    }
    
    printf("Sorted words:\n");
    for (i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    printf("\nWord statistics:\n");
    printf("Total words: %d\n", word_count);
    
    int unique_count = 0;
    for (i = 0; i < word_count; i++) {
        int is_unique = 1;
        for (j = 0; j < i; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            unique_count++;
        }
    }
    printf("Unique words: %d\n", unique_count);
    
    int longest_len = 0;
    for (i = 0; i < word_count; i++) {
        int len = strlen(words[i]);
        if (len > longest_len) {
            longest_len = len;
        }
    }
    printf("Longest word length: %d\n", longest_len);
    
    return 0;
}