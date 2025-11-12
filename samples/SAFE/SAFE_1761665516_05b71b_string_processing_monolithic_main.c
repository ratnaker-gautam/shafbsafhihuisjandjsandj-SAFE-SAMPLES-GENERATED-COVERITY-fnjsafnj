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
    int i;
    
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
    
    for (i = 0; i <= input_len; i++) {
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
            }
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    printf("Found %d words:\n", word_count);
    for (i = 0; i < word_count; i++) {
        printf("%d: %s\n", i + 1, words[i]);
    }
    
    int freq[MAX_WORDS] = {0};
    int unique_count = 0;
    
    for (i = 0; i < word_count; i++) {
        int found = 0;
        int j;
        for (j = 0; j < unique_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                freq[j]++;
                found = 1;
                break;
            }
        }
        if (!found && unique_count < MAX_WORDS) {
            strncpy(words[unique_count], words[i], 63);
            words[unique_count][63] = '\0';
            freq[unique_count] = 1;
            unique_count++;
        }
    }
    
    printf("\nWord frequencies:\n");
    for (i = 0; i < unique_count; i++) {
        printf("%s: %d\n", words[i], freq[i]);
    }
    
    char longest[64] = "";
    char shortest[64] = "";
    int max_len = 0;
    int min_len = 64;
    
    for (i = 0; i < unique_count; i++) {
        int len = strlen(words[i]);
        if (len > max_len) {
            max_len = len;
            strncpy(longest, words[i], 63);
            longest[63] = '\0';
        }
        if (len < min_len) {
            min_len = len;
            strncpy(shortest, words[i], 63);
            shortest[63] = '\0';
        }
    }
    
    printf("\nLongest word: %s (%d letters)\n", longest, max_len);
    printf("Shortest word: %s (%d letters)\n", shortest, min_len);
    
    return 0;
}