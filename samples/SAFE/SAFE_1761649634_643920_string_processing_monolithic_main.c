//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main() {
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
            if (word_length < 63 && word_count < MAX_WORDS) {
                words[word_count][word_length++] = tolower((unsigned char)input[i]);
            }
        } else {
            if (in_word) {
                if (word_length > 0 && word_count < MAX_WORDS) {
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
    
    int freq[MAX_WORDS] = {0};
    printf("\nWord frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        if (freq[i] == 0) {
            int count = 1;
            for (int j = i + 1; j < word_count; j++) {
                if (strcmp(words[i], words[j]) == 0) {
                    count++;
                    freq[j] = 1;
                }
            }
            printf("%s: %d\n", words[i], count);
        }
    }
    
    char longest_word[64] = "";
    char shortest_word[64] = "";
    int shortest_len = 64;
    
    for (int i = 0; i < word_count; i++) {
        int len = strlen(words[i]);
        if (len > strlen(longest_word)) {
            strncpy(longest_word, words[i], sizeof(longest_word) - 1);
            longest_word[sizeof(longest_word) - 1] = '\0';
        }
        if (len < shortest_len && len > 0) {
            strncpy(shortest_word, words[i], sizeof(shortest_word) - 1);
            shortest_word[sizeof(shortest_word) - 1] = '\0';
            shortest_len = len;
        }
    }
    
    if (strlen(longest_word) > 0) {
        printf("\nLongest word: %s (%zu characters)\n", longest_word, strlen(longest_word));
    }
    if (strlen(shortest_word) > 0) {
        printf("Shortest word: %s (%d characters)\n", shortest_word, shortest_len);
    }
    
    return 0;
}