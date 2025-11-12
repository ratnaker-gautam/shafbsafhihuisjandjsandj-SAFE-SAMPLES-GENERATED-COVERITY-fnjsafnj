//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][64];
    int word_count = 0;
    int word_length = 0;
    int in_word = 0;
    
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
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha((unsigned char)input[i])) {
            if (!in_word) {
                in_word = 1;
                word_length = 0;
            }
            if (word_length < 63) {
                words[word_count][word_length++] = tolower((unsigned char)input[i]);
            }
        } else {
            if (in_word) {
                if (word_length > 0) {
                    words[word_count][word_length] = '\0';
                    word_count++;
                    if (word_count >= MAX_WORDS) {
                        break;
                    }
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
    
    int freq[MAX_WORDS] = {0};
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                freq[j]++;
                found = 1;
                break;
            }
        }
        if (!found && unique_count < MAX_WORDS) {
            strcpy(words[unique_count], words[i]);
            freq[unique_count] = 1;
            unique_count++;
        }
    }
    
    printf("\nWord frequencies:\n");
    for (int i = 0; i < unique_count; i++) {
        printf("%s: %d\n", words[i], freq[i]);
    }
    
    char longest_word[64] = "";
    char shortest_word[64] = "";
    int max_len = 0;
    int min_len = 64;
    
    for (int i = 0; i < unique_count; i++) {
        int len = strlen(words[i]);
        if (len > max_len) {
            max_len = len;
            strcpy(longest_word, words[i]);
        }
        if (len < min_len) {
            min_len = len;
            strcpy(shortest_word, words[i]);
        }
    }
    
    printf("\nLongest word: %s (%d characters)\n", longest_word, max_len);
    printf("Shortest word: %s (%d characters)\n", shortest_word, min_len);
    
    return 0;
}