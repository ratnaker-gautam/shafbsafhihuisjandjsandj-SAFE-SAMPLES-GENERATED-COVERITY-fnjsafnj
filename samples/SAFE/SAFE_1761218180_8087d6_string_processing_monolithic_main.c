//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][64];
    int word_count = 0;
    int i, j, k;
    
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
    
    if (input_len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < input_len && word_count < MAX_WORDS) {
        while (i < input_len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        j = i;
        while (j < input_len && !isspace((unsigned char)input[j])) {
            j++;
        }
        
        int word_len = j - i;
        if (word_len >= 64) {
            fprintf(stderr, "Word too long\n");
            return 1;
        }
        
        for (k = 0; k < word_len; k++) {
            words[word_count][k] = tolower((unsigned char)input[i + k]);
        }
        words[word_count][word_len] = '\0';
        word_count++;
        
        i = j;
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 1;
    }
    
    printf("Processing %d words...\n", word_count);
    
    for (i = 0; i < word_count; i++) {
        int count = 1;
        if (words[i][0] == '\0') {
            continue;
        }
        
        for (j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                count++;
                words[j][0] = '\0';
            }
        }
        
        printf("Word: %s, Count: %d\n", words[i], count);
    }
    
    char longest_word[64] = "";
    char shortest_word[64] = "";
    int longest_len = 0;
    int shortest_len = 64;
    
    for (i = 0; i < word_count; i++) {
        if (words[i][0] == '\0') {
            continue;
        }
        
        int len = strlen(words[i]);
        if (len > longest_len) {
            longest_len = len;
            strncpy(longest_word, words[i], sizeof(longest_word) - 1);
            longest_word[sizeof(longest_word) - 1] = '\0';
        }
        
        if (len < shortest_len) {
            shortest_len = len;
            strncpy(shortest_word, words[i], sizeof(shortest_word) - 1);
            shortest_word[sizeof(shortest_word) - 1] = '\0';
        }
    }
    
    if (longest_len > 0) {
        printf("Longest word: %s (%d characters)\n", longest_word, longest_len);
    }
    if (shortest_len < 64) {
        printf("Shortest word: %s (%d characters)\n", shortest_word, shortest_len);
    }
    
    return 0;
}