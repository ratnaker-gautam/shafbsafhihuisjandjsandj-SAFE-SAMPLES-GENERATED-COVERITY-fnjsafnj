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
        if (word_len > 0 && word_len <= MAX_INPUT) {
            strncpy(words[word_count], input + start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    printf("Words found: %d\n", word_count);
    
    for (int j = 0; j < word_count; j++) {
        int k = 0;
        char reversed[MAX_INPUT + 1];
        int word_len = strlen(words[j]);
        
        if (word_len > MAX_INPUT) {
            fprintf(stderr, "Word too long\n");
            return 1;
        }
        
        for (int pos = word_len - 1; pos >= 0; pos--) {
            if (k >= MAX_INPUT) break;
            reversed[k++] = words[j][pos];
        }
        reversed[k] = '\0';
        
        printf("Word %d: '%s' (reversed: '%s')\n", j + 1, words[j], reversed);
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
    
    char longest_word[MAX_INPUT + 1] = "";
    int max_len = 0;
    
    for (int m = 0; m < word_count; m++) {
        int current_len = strlen(words[m]);
        if (current_len > max_len && current_len <= MAX_INPUT) {
            max_len = current_len;
            strncpy(longest_word, words[m], MAX_INPUT);
            longest_word[MAX_INPUT] = '\0';
        }
    }
    
    if (max_len > 0) {
        printf("Longest word: '%s' (%d characters)\n", longest_word, max_len);
    }
    
    return 0;
}