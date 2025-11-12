//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    char temp[MAX_WORD_LEN + 1];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN);
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
        printf("Empty input string\n");
        return 0;
    }
    
    if (len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        int j = 0;
        while (i < len && j < MAX_WORD_LEN && !isspace((unsigned char)input[i])) {
            temp[j++] = input[i++];
        }
        
        if (j > 0) {
            temp[j] = '\0';
            strncpy(words[word_count], temp, MAX_WORD_LEN);
            words[word_count][MAX_WORD_LEN] = '\0';
            word_count++;
        }
    }
    
    printf("Found %d words:\n", word_count);
    
    for (int k = 0; k < word_count; k++) {
        int word_len = strlen(words[k]);
        char processed[MAX_WORD_LEN + 1];
        int p = 0;
        
        for (int m = 0; m < word_len && p < MAX_WORD_LEN; m++) {
            if (isalpha((unsigned char)words[k][m])) {
                processed[p++] = tolower((unsigned char)words[k][m]);
            }
        }
        processed[p] = '\0';
        
        if (p > 0) {
            printf("Word %d: '%s' -> '%s'\n", k + 1, words[k], processed);
        } else {
            printf("Word %d: '%s' -> (no letters)\n", k + 1, words[k]);
        }
    }
    
    printf("\nProcessing complete. Statistics:\n");
    printf("Total words processed: %d\n", word_count);
    
    int total_chars = 0;
    int k = 0;
    do {
        if (k < word_count) {
            total_chars += strlen(words[k]);
        }
        k++;
    } while (k < word_count);
    
    printf("Total characters in words: %d\n", total_chars);
    
    if (word_count > 0) {
        int shortest_len = strlen(words[0]);
        int longest_len = shortest_len;
        
        for (int k = 1; k < word_count; k++) {
            int current_len = strlen(words[k]);
            if (current_len < shortest_len) {
                shortest_len = current_len;
            }
            if (current_len > longest_len) {
                longest_len = current_len;
            }
        }
        
        printf("Shortest word length: %d\n", shortest_len);
        printf("Longest word length: %d\n", longest_len);
    }
    
    return 0;
}