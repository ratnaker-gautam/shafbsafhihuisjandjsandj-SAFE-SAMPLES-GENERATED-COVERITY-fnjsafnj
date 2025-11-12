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
    
    if (input_len > MAX_INPUT_LEN) {
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
            word_len = 63;
        }
        
        if (word_len > 0) {
            for (k = 0; k < word_len; k++) {
                words[word_count][k] = tolower((unsigned char)input[i + k]);
            }
            words[word_count][word_len] = '\0';
            word_count++;
        }
        
        i = j;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    printf("\nProcessing results:\n");
    printf("Total words: %d\n", word_count);
    
    printf("\nWord frequency analysis:\n");
    int processed[MAX_WORDS] = {0};
    
    for (i = 0; i < word_count; i++) {
        if (processed[i]) {
            continue;
        }
        
        int count = 1;
        for (j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                count++;
                processed[j] = 1;
            }
        }
        
        printf("'%s': %d occurrence(s)\n", words[i], count);
        processed[i] = 1;
    }
    
    printf("\nSorted words (alphabetical):\n");
    char temp[64];
    for (i = 0; i < word_count - 1; i++) {
        for (j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) > 0) {
                strncpy(temp, words[i], sizeof(temp) - 1);
                temp[sizeof(temp) - 1] = '\0';
                strncpy(words[i], words[j], sizeof(words[i]) - 1);
                words[i][sizeof(words[i]) - 1] = '\0';
                strncpy(words[j], temp, sizeof(words[j]) - 1);
                words[j][sizeof(words[j]) - 1] = '\0';
            }
        }
    }
    
    for (i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    printf("\nWord statistics:\n");
    int total_chars = 0;
    int shortest_len = 1000;
    int longest_len = 0;
    
    for (i = 0; i < word_count; i++) {
        int len = strlen(words[i]);
        total_chars += len;
        if (len < shortest_len) {
            shortest_len = len;
        }
        if (len > longest_len) {
            longest_len = len;
        }
    }
    
    printf("Average word length: %.2f\n", (float)total_chars / word_count);
    printf("Shortest word length: %d\n", shortest_len);
    printf("Longest word length: %d\n", longest_len);
    
    return 0;
}