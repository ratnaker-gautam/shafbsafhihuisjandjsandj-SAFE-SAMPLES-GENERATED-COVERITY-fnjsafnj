//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN);
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
    
    if (len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        j = 0;
        while (i < len && !isspace(input[i]) && j < MAX_WORD_LEN) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        
        if (j > 0) {
            words[word_count][j] = '\0';
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("Original words:\n");
    for (k = 0; k < word_count; k++) {
        printf("%d: %s\n", k + 1, words[k]);
    }
    
    printf("\nProcessing words:\n");
    for (k = 0; k < word_count; k++) {
        size_t word_len = strlen(words[k]);
        
        printf("Word %d: ", k + 1);
        
        int m = 0;
        do {
            if (m < word_len) {
                putchar(toupper(words[k][m]));
            }
            m++;
        } while (m <= word_len);
        
        printf(" (length: %zu) ", word_len);
        
        int is_palindrome = 1;
        size_t left = 0;
        size_t right = word_len - 1;
        for (; left < right; left++, right--) {
            if (tolower(words[k][left]) != tolower(words[k][right])) {
                is_palindrome = 0;
                break;
            }
        }
        
        if (is_palindrome && word_len > 0) {
            printf("[palindrome]");
        }
        
        printf("\n");
    }
    
    printf("\nWord statistics:\n");
    printf("Total words: %d\n", word_count);
    
    int total_chars = 0;
    k = 0;
    while (k < word_count) {
        total_chars += strlen(words[k]);
        k++;
    }
    
    printf("Total characters in words: %d\n", total_chars);
    printf("Average word length: %.2f\n", word_count > 0 ? (float)total_chars / word_count : 0.0);
    
    return 0;
}