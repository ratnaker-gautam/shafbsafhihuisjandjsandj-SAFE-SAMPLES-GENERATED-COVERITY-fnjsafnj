//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: string_processing
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
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        j = 0;
        while (i < len && !isspace((unsigned char)input[i]) && j < MAX_WORD_LEN) {
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
        printf("No words found\n");
        return 0;
    }
    
    printf("Words found (%d):\n", word_count);
    for (k = 0; k < word_count; k++) {
        printf("%d: \"%s\"\n", k + 1, words[k]);
    }
    
    printf("\nProcessing words:\n");
    for (k = 0; k < word_count; k++) {
        size_t word_len = strlen(words[k]);
        char processed[MAX_WORD_LEN + 1];
        int has_digit = 0;
        
        for (i = 0; i < word_len; i++) {
            if (isdigit((unsigned char)words[k][i])) {
                has_digit = 1;
                break;
            }
        }
        
        if (has_digit) {
            j = 0;
            for (i = 0; i < word_len && j < MAX_WORD_LEN; i++) {
                if (!isdigit((unsigned char)words[k][i])) {
                    processed[j] = words[k][i];
                    j++;
                }
            }
            processed[j] = '\0';
            printf("Word %d (filtered): \"%s\" -> \"%s\"\n", k + 1, words[k], processed);
        } else {
            j = 0;
            for (i = 0; i < word_len && j < MAX_WORD_LEN; i++) {
                processed[j] = toupper((unsigned char)words[k][i]);
                j++;
            }
            processed[j] = '\0';
            printf("Word %d (uppercase): \"%s\" -> \"%s\"\n", k + 1, words[k], processed);
        }
    }
    
    printf("\nWord statistics:\n");
    int total_chars = 0;
    int shortest_len = MAX_WORD_LEN;
    int longest_len = 0;
    
    k = 0;
    do {
        size_t current_len = strlen(words[k]);
        total_chars += current_len;
        
        if (current_len < shortest_len) {
            shortest_len = current_len;
        }
        if (current_len > longest_len) {
            longest_len = current_len;
        }
        
        k++;
    } while (k < word_count);
    
    printf("Total characters in words: %d\n", total_chars);
    printf("Shortest word length: %d\n", shortest_len);
    printf("Longest word length: %d\n", longest_len);
    printf("Average word length: %.2f\n", word_count > 0 ? (float)total_chars / word_count : 0.0);
    
    return 0;
}