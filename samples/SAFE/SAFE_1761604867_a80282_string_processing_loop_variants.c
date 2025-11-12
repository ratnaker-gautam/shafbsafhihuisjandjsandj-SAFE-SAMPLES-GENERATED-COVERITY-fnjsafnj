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
            fprintf(stderr, "Word too long\n");
            return 1;
        }
        
        strncpy(words[word_count], &input[start], word_len);
        words[word_count][word_len] = '\0';
        word_count++;
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    printf("Words found: %d\n", word_count);
    printf("Processing words...\n");
    
    for (int j = 0; j < word_count; j++) {
        char temp[MAX_INPUT];
        strncpy(temp, words[j], sizeof(temp) - 1);
        temp[sizeof(temp) - 1] = '\0';
        
        size_t k = 0;
        size_t temp_len = strlen(temp);
        while (k < temp_len) {
            temp[k] = toupper(temp[k]);
            k++;
        }
        
        int m = 0;
        int n = temp_len - 1;
        while (m < n) {
            char swap = temp[m];
            temp[m] = temp[n];
            temp[n] = swap;
            m++;
            n--;
        }
        
        printf("Word %d: %s -> %s\n", j + 1, words[j], temp);
    }
    
    int total_chars = 0;
    int idx = 0;
    do {
        if (idx < word_count) {
            total_chars += strlen(words[idx]);
        }
        idx++;
    } while (idx < word_count);
    
    printf("Total characters in all words: %d\n", total_chars);
    
    return 0;
}