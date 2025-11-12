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
            strncpy(words[word_count], input + start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 1;
    }
    
    printf("Processing words:\n");
    for (int j = 0; j < word_count; j++) {
        int k = 0;
        char processed[64];
        int has_alpha = 0;
        
        while (words[j][k] != '\0' && k < 63) {
            if (isalpha((unsigned char)words[j][k])) {
                processed[k] = tolower((unsigned char)words[j][k]);
                has_alpha = 1;
            } else {
                processed[k] = words[j][k];
            }
            k++;
        }
        processed[k] = '\0';
        
        if (has_alpha) {
            int m = 0;
            int n = k - 1;
            int is_palindrome = 1;
            
            while (m < n) {
                if (processed[m] != processed[n]) {
                    is_palindrome = 0;
                    break;
                }
                m++;
                n--;
            }
            
            printf("Word %d: '%s' - Length: %d, Palindrome: %s\n", 
                   j+1, words[j], k, is_palindrome ? "Yes" : "No");
        } else {
            printf("Word %d: '%s' - No alphabetic characters\n", j+1, words[j]);
        }
    }
    
    int total_chars = 0;
    int p = 0;
    do {
        if (p < word_count) {
            int q = 0;
            while (words[p][q] != '\0') {
                total_chars++;
                q++;
            }
        }
        p++;
    } while (p < word_count);
    
    printf("Total characters in all words: %d\n", total_chars);
    
    return 0;
}