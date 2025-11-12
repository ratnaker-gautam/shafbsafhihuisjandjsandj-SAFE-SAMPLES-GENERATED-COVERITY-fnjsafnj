//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 256
#define MAX_WORDS 50

int main() {
    char input[MAX_INPUT + 1];
    char words[MAX_WORDS][MAX_INPUT + 1];
    int word_count = 0;
    
    printf("Enter a sentence: ");
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
        printf("Empty input\n");
        return 0;
    }
    
    if (len >= MAX_INPUT) {
        fprintf(stderr, "Input too long\n");
        return 1;
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
        if (word_len > 0 && word_len <= MAX_INPUT) {
            strncpy(words[word_count], &input[start], word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    printf("Words found: %d\n", word_count);
    
    for (int outer = 0; outer < word_count; outer++) {
        int processed = 0;
        for (int inner = 0; inner < outer; inner++) {
            if (strcmp(words[outer], words[inner]) == 0) {
                processed = 1;
                break;
            }
        }
        
        if (!processed) {
            int count = 0;
            char *current = words[outer];
            while (*current) {
                if (isalpha(*current)) {
                    count++;
                }
                current++;
            }
            printf("'%s' has %d letters\n", words[outer], count);
        }
    }
    
    char reversed[MAX_INPUT + 1];
    int rev_idx = 0;
    for (int idx = len - 1; idx >= 0; idx--) {
        if (rev_idx < MAX_INPUT) {
            reversed[rev_idx++] = input[idx];
        }
    }
    reversed[rev_idx] = '\0';
    
    printf("Reversed: %s\n", reversed);
    
    return 0;
}