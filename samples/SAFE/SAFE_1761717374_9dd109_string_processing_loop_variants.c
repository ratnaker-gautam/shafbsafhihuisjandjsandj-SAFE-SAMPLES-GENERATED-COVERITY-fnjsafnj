//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
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
        for (int inner = outer + 1; inner < word_count; inner++) {
            if (strcmp(words[outer], words[inner]) > 0) {
                char temp[MAX_INPUT + 1];
                strncpy(temp, words[outer], sizeof(temp) - 1);
                temp[sizeof(temp) - 1] = '\0';
                strncpy(words[outer], words[inner], sizeof(words[outer]) - 1);
                words[outer][sizeof(words[outer]) - 1] = '\0';
                strncpy(words[inner], temp, sizeof(words[inner]) - 1);
                words[inner][sizeof(words[inner]) - 1] = '\0';
            }
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
    
    printf("Word lengths:\n");
    int k = 0;
    while (k < word_count) {
        size_t length = strlen(words[k]);
        if (length > 0) {
            printf("%s: %zu\n", words[k], length);
        }
        k++;
    }
    
    return 0;
}