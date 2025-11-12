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
    int i, j, k;
    
    printf("Enter text: ");
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
        printf("No input provided\n");
        return 0;
    }
    
    if (len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        j = i;
        while (j < len && !isspace((unsigned char)input[j])) {
            j++;
        }
        
        int word_len = j - i;
        if (word_len >= 64) {
            fprintf(stderr, "Word too long\n");
            return 1;
        }
        
        if (word_len > 0) {
            for (k = 0; k < word_len; k++) {
                words[word_count][k] = input[i + k];
            }
            words[word_count][word_len] = '\0';
            word_count++;
        }
        
        i = j;
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    for (i = 0; i < word_count - 1; i++) {
        for (j = 0; j < word_count - i - 1; j++) {
            if (strcmp(words[j], words[j + 1]) > 0) {
                char temp[64];
                if (strlen(words[j]) < 64 && strlen(words[j + 1]) < 64) {
                    strcpy(temp, words[j]);
                    strcpy(words[j], words[j + 1]);
                    strcpy(words[j + 1], temp);
                }
            }
        }
    }
    
    printf("Sorted words:\n");
    for (i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    int unique_count = 0;
    printf("\nUnique words:\n");
    for (i = 0; i < word_count; i++) {
        if (i == 0 || strcmp(words[i], words[i - 1]) != 0) {
            printf("%s\n", words[i]);
            unique_count++;
        }
    }
    
    printf("\nStatistics:\n");
    printf("Total words: %d\n", word_count);
    printf("Unique words: %d\n", unique_count);
    
    return 0;
}