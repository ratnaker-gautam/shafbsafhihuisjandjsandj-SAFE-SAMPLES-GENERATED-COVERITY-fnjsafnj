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
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
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
        
        if (i >= len) break;
        
        j = i;
        while (j < len && !isspace((unsigned char)input[j])) {
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
        return 0;
    }
    
    printf("\nProcessing results:\n");
    printf("Total words: %d\n", word_count);
    
    printf("\nWords in alphabetical order:\n");
    for (i = 0; i < word_count - 1; i++) {
        for (j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) > 0) {
                char temp[64];
                strncpy(temp, words[i], sizeof(temp));
                strncpy(words[i], words[j], sizeof(words[i]));
                strncpy(words[j], temp, sizeof(words[j]));
            }
        }
    }
    
    for (i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    printf("\nWord frequency:\n");
    int processed[MAX_WORDS] = {0};
    
    for (i = 0; i < word_count; i++) {
        if (processed[i]) continue;
        
        int count = 1;
        for (j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                count++;
                processed[j] = 1;
            }
        }
        
        printf("%s: %d\n", words[i], count);
        processed[i] = 1;
    }
    
    return 0;
}