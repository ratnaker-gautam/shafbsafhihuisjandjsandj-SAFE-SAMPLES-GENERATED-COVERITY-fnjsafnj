//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter text: ");
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
        printf("No input provided\n");
        return 0;
    }
    
    i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        j = 0;
        while (i < len && j < MAX_WORD_LEN - 1 && !isspace((unsigned char)input[i])) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        words[word_count][j] = '\0';
        
        if (j > 0) {
            word_count++;
        }
    }
    
    printf("Words found: %d\n", word_count);
    
    for (k = 0; k < word_count; k++) {
        int is_palindrome = 1;
        size_t word_len = strlen(words[k]);
        
        if (word_len > 0) {
            for (i = 0, j = word_len - 1; i < j; i++, j--) {
                if (tolower((unsigned char)words[k][i]) != tolower((unsigned char)words[k][j])) {
                    is_palindrome = 0;
                    break;
                }
            }
        } else {
            is_palindrome = 0;
        }
        
        printf("Word %d: \"%s\"", k + 1, words[k]);
        if (is_palindrome) {
            printf(" (palindrome)");
        }
        printf("\n");
    }
    
    int total_chars = 0;
    for (k = 0; k < word_count; k++) {
        total_chars += strlen(words[k]);
    }
    
    printf("Total characters in words: %d\n", total_chars);
    
    return 0;
}