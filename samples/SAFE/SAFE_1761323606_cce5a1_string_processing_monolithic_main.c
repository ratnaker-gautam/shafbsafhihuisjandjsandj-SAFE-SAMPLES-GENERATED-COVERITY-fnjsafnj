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
        if (word_len > 0 && word_len < 64) {
            if (word_count < MAX_WORDS) {
                strncpy(words[word_count], &input[i], word_len);
                words[word_count][word_len] = '\0';
                word_count++;
            }
        }
        
        i = j;
    }
    
    printf("\nProcessing results:\n");
    printf("Total words: %d\n", word_count);
    
    if (word_count > 0) {
        printf("\nWords in reverse order:\n");
        for (i = word_count - 1; i >= 0; i--) {
            printf("%s\n", words[i]);
        }
        
        printf("\nWord statistics:\n");
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
        
        int longest_index = 0;
        int shortest_index = 0;
        for (i = 1; i < word_count; i++) {
            if (strlen(words[i]) > strlen(words[longest_index])) {
                longest_index = i;
            }
            if (strlen(words[i]) < strlen(words[shortest_index])) {
                shortest_index = i;
            }
        }
        printf("Longest word: %s (%zu characters)\n", words[longest_index], strlen(words[longest_index]));
        printf("Shortest word: %s (%zu characters)\n", words[shortest_index], strlen(words[shortest_index]));
        
        printf("\nWords sorted alphabetically:\n");
        for (i = 0; i < word_count - 1; i++) {
            for (j = i + 1; j < word_count; j++) {
                if (strcmp(words[i], words[j]) > 0) {
                    char temp[64];
                    strcpy(temp, words[i]);
                    strcpy(words[i], words[j]);
                    strcpy(words[j], temp);
                }
            }
        }
        
        for (i = 0; i < word_count; i++) {
            printf("%s\n", words[i]);
        }
    }
    
    return 0;
}