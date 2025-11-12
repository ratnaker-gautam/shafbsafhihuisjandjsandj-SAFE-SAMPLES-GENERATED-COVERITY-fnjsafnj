//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 256
#define MAX_WORDS 50

int main() {
    char input[MAX_INPUT];
    char words[MAX_WORDS][MAX_INPUT];
    int word_count = 0;
    
    printf("Enter a sentence: ");
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
        printf("Empty input\n");
        return 0;
    }
    
    int i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        int start = i;
        while (i < len && !isspace(input[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len >= MAX_INPUT) {
            word_len = MAX_INPUT - 1;
        }
        
        strncpy(words[word_count], input + start, word_len);
        words[word_count][word_len] = '\0';
        word_count++;
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    printf("Words found: %d\n", word_count);
    
    for (int outer = 0; outer < word_count; outer++) {
        for (int inner = 0; inner < word_count - 1; inner++) {
            if (strcmp(words[inner], words[inner + 1]) > 0) {
                char temp[MAX_INPUT];
                strncpy(temp, words[inner], sizeof(temp) - 1);
                temp[sizeof(temp) - 1] = '\0';
                strncpy(words[inner], words[inner + 1], sizeof(words[inner]) - 1);
                words[inner][sizeof(words[inner]) - 1] = '\0';
                strncpy(words[inner + 1], temp, sizeof(words[inner + 1]) - 1);
                words[inner + 1][sizeof(words[inner + 1]) - 1] = '\0';
            }
        }
    }
    
    printf("Sorted words:\n");
    int idx = 0;
    do {
        printf("%s\n", words[idx]);
        idx++;
    } while (idx < word_count);
    
    printf("Word lengths:\n");
    int j = 0;
    while (j < word_count) {
        size_t wlen = strlen(words[j]);
        if (wlen < MAX_INPUT) {
            printf("'%s': %zu characters\n", words[j], wlen);
        }
        j++;
    }
    
    int total_chars = 0;
    for (int k = 0; k < word_count; k++) {
        total_chars += strlen(words[k]);
    }
    
    if (word_count > 0) {
        double avg_length = (double)total_chars / word_count;
        printf("Average word length: %.2f\n", avg_length);
    }
    
    return 0;
}