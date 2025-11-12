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
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        int start = i;
        while (i < len && !isspace((unsigned char)input[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len > 0 && word_len < 64) {
            strncpy(words[word_count], &input[start], word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 1;
    }
    
    printf("Processing %d words:\n", word_count);
    
    for (int idx = 0; idx < word_count; idx++) {
        char processed[64];
        int j = 0;
        int k = 0;
        
        while (words[idx][j] != '\0' && k < 63) {
            char c = words[idx][j];
            if (isalpha((unsigned char)c)) {
                processed[k] = tolower((unsigned char)c);
                k++;
            }
            j++;
        }
        processed[k] = '\0';
        
        if (k > 0) {
            printf("Original: %s -> Processed: %s\n", words[idx], processed);
        } else {
            printf("Original: %s -> No alphabetic characters\n", words[idx]);
        }
    }
    
    int total_chars = 0;
    int idx = 0;
    do {
        total_chars += strlen(words[idx]);
        idx++;
    } while (idx < word_count);
    
    printf("Total characters in all words: %d\n", total_chars);
    
    int unique_count = 0;
    for (int i = 0; i < word_count; i++) {
        int is_unique = 1;
        for (int j = 0; j < i; j++) {
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
    
    return 0;
}