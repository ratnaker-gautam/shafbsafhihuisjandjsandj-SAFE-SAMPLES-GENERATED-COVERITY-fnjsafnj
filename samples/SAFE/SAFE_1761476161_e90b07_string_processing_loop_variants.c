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
    
    printf("Enter a string: ");
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
    
    int i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        int start = i;
        while (i < len && !isspace((unsigned char)input[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len >= MAX_INPUT) {
            word_len = MAX_INPUT - 1;
        }
        
        strncpy(words[word_count], &input[start], word_len);
        words[word_count][word_len] = '\0';
        word_count++;
    }
    
    printf("Words found: %d\n", word_count);
    
    for (int j = 0; j < word_count; j++) {
        int k = 0;
        char processed[MAX_INPUT];
        
        for (int pos = 0; words[j][pos] != '\0' && k < MAX_INPUT - 1; pos++) {
            if (isalnum((unsigned char)words[j][pos])) {
                processed[k++] = tolower((unsigned char)words[j][pos]);
            }
        }
        processed[k] = '\0';
        
        if (k > 0) {
            printf("Word %d (cleaned): %s\n", j + 1, processed);
        }
    }
    
    int total_chars = 0;
    int idx = 0;
    do {
        if (idx < word_count) {
            total_chars += strlen(words[idx]);
        }
        idx++;
    } while (idx < word_count);
    
    printf("Total characters in words: %d\n", total_chars);
    
    char longest[MAX_INPUT] = "";
    for (int m = 0; m < word_count; m++) {
        if (strlen(words[m]) > strlen(longest)) {
            strncpy(longest, words[m], MAX_INPUT - 1);
            longest[MAX_INPUT - 1] = '\0';
        }
    }
    
    if (strlen(longest) > 0) {
        printf("Longest word: %s\n", longest);
    }
    
    return 0;
}