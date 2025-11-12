//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 1024
#define MAX_WORDS 512

int main() {
    char input[MAX_INPUT];
    char words[MAX_WORDS][64];
    int word_count = 0;
    
    printf("Enter text: ");
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
        return 1;
    }
    
    int i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        int start = i;
        while (i < len && !isspace(input[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len > 0 && word_len < 64) {
            strncpy(words[word_count], input + start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    printf("Words found: %d\n", word_count);
    
    for (int outer = 0; outer < word_count; outer++) {
        int min_idx = outer;
        for (int inner = outer + 1; inner < word_count; inner++) {
            if (strcmp(words[inner], words[min_idx]) < 0) {
                min_idx = inner;
            }
        }
        
        if (min_idx != outer) {
            char temp[64];
            strcpy(temp, words[outer]);
            strcpy(words[outer], words[min_idx]);
            strcpy(words[min_idx], temp);
        }
    }
    
    printf("Sorted words:\n");
    int j = 0;
    do {
        if (j < word_count) {
            printf("%s\n", words[j]);
            j++;
        }
    } while (j < word_count);
    
    int total_chars = 0;
    int k = 0;
    while (k < word_count) {
        total_chars += strlen(words[k]);
        k++;
    }
    
    printf("Total characters in words: %d\n", total_chars);
    
    return 0;
}