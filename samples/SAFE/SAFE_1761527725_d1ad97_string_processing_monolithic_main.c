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
        
        j = 0;
        while (i < input_len && j < MAX_WORD_LEN - 1 && !isspace((unsigned char)input[i])) {
            words[word_count][j] = tolower((unsigned char)input[i]);
            j++;
            i++;
        }
        
        if (j > 0) {
            words[word_count][j] = '\0';
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    printf("\nProcessing results:\n");
    printf("Total words: %d\n", word_count);
    
    printf("\nWord frequency analysis:\n");
    for (i = 0; i < word_count; i++) {
        if (words[i][0] == '\0') {
            continue;
        }
        
        int count = 1;
        for (j = i + 1; j < word_count; j++) {
            if (words[j][0] != '\0' && strcmp(words[i], words[j]) == 0) {
                count++;
                words[j][0] = '\0';
            }
        }
        
        printf("'%s': %d\n", words[i], count);
    }
    
    printf("\nSorted unique words:\n");
    char temp[MAX_WORD_LEN];
    for (i = 0; i < word_count - 1; i++) {
        for (j = i + 1; j < word_count; j++) {
            if (words[i][0] != '\0' && words[j][0] != '\0' && strcmp(words[i], words[j]) > 0) {
                strncpy(temp, words[i], MAX_WORD_LEN - 1);
                temp[MAX_WORD_LEN - 1] = '\0';
                strncpy(words[i], words[j], MAX_WORD_LEN - 1);
                words[i][MAX_WORD_LEN - 1] = '\0';
                strncpy(words[j], temp, MAX_WORD_LEN - 1);
                words[j][MAX_WORD_LEN - 1] = '\0';
            }
        }
    }
    
    for (i = 0; i < word_count; i++) {
        if (words[i][0] != '\0') {
            printf("%s\n", words[i]);
        }
    }
    
    printf("\nWord statistics:\n");
    int total_chars = 0;
    int shortest_len = MAX_WORD_LEN;
    int longest_len = 0;
    
    for (i = 0; i < word_count; i++) {
        if (words[i][0] != '\0') {
            int len = strlen(words[i]);
            total_chars += len;
            if (len < shortest_len) shortest_len = len;
            if (len > longest_len) longest_len = len;
        }
    }
    
    printf("Average word length: %.2f\n", word_count > 0 ? (double)total_chars / word_count : 0.0);
    printf("Shortest word length: %d\n", shortest_len != MAX_WORD_LEN ? shortest_len : 0);
    printf("Longest word length: %d\n", longest_len);
    
    return 0;
}