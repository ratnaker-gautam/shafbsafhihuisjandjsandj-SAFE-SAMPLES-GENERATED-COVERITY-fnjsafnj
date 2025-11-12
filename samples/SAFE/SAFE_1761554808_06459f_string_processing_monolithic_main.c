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
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return 0;
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
        if (word_len >= 64) {
            fprintf(stderr, "Word too long\n");
            return 1;
        }
        
        for (k = 0; k < word_len; k++) {
            words[word_count][k] = tolower((unsigned char)input[i + k]);
        }
        words[word_count][word_len] = '\0';
        word_count++;
        
        i = j;
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    printf("Processing %d words...\n", word_count);
    
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
    
    int freq[MAX_WORDS] = {0};
    for (i = 0; i < word_count; i++) {
        for (j = 0; j < word_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                freq[i]++;
            }
        }
    }
    
    printf("Word frequencies:\n");
    for (i = 0; i < word_count; i++) {
        int already_printed = 0;
        for (j = 0; j < i; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                already_printed = 1;
                break;
            }
        }
        if (!already_printed) {
            printf("'%s': %d\n", words[i], freq[i]);
        }
    }
    
    char longest_word[64] = "";
    int max_len = 0;
    for (i = 0; i < word_count; i++) {
        int len = strlen(words[i]);
        if (len > max_len) {
            max_len = len;
            strncpy(longest_word, words[i], sizeof(longest_word) - 1);
            longest_word[sizeof(longest_word) - 1] = '\0';
        }
    }
    
    if (max_len > 0) {
        printf("Longest word: '%s' (%d characters)\n", longest_word, max_len);
    }
    
    return 0;
}