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
    int i, j, k;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    
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
        
        j = 0;
        while (i < input_len && j < MAX_WORD_LEN - 1 && !isspace((unsigned char)input[i])) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        
        if (j > 0) {
            words[word_count][j] = '\0';
            
            for (k = 0; k < j; k++) {
                words[word_count][k] = tolower((unsigned char)words[word_count][k]);
            }
            
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    
    int unique_count = 0;
    char unique_words[MAX_WORDS][MAX_WORD_LEN];
    int frequencies[MAX_WORDS] = {0};
    
    for (i = 0; i < word_count; i++) {
        int found = 0;
        for (j = 0; j < unique_count; j++) {
            if (strcmp(words[i], unique_words[j]) == 0) {
                frequencies[j]++;
                found = 1;
                break;
            }
        }
        
        if (!found && unique_count < MAX_WORDS) {
            strncpy(unique_words[unique_count], words[i], MAX_WORD_LEN - 1);
            unique_words[unique_count][MAX_WORD_LEN - 1] = '\0';
            frequencies[unique_count] = 1;
            unique_count++;
        }
    }
    
    printf("\nWord frequencies:\n");
    for (i = 0; i < unique_count; i++) {
        printf("%s: %d\n", unique_words[i], frequencies[i]);
    }
    
    char longest_word[MAX_WORD_LEN] = "";
    char shortest_word[MAX_WORD_LEN] = "";
    int max_len = 0;
    int min_len = MAX_WORD_LEN;
    
    for (i = 0; i < unique_count; i++) {
        int len = strlen(unique_words[i]);
        if (len > max_len) {
            max_len = len;
            strncpy(longest_word, unique_words[i], MAX_WORD_LEN - 1);
            longest_word[MAX_WORD_LEN - 1] = '\0';
        }
        if (len < min_len) {
            min_len = len;
            strncpy(shortest_word, unique_words[i], MAX_WORD_LEN - 1);
            shortest_word[MAX_WORD_LEN - 1] = '\0';
        }
    }
    
    printf("\nLongest word: %s (%d characters)\n", longest_word, max_len);
    printf("Shortest word: %s (%d characters)\n", shortest_word, min_len);
    
    int total_chars = 0;
    for (i = 0; i < word_count; i++) {
        total_chars += strlen(words[i]);
    }
    
    printf("Average word length: %.2f characters\n", word_count > 0 ? (double)total_chars / word_count : 0.0);
    
    return 0;
}