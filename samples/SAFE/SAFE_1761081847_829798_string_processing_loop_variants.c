//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 30

int main() {
    char input[MAX_INPUT + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    
    printf("Enter a sentence (max %d characters): ", MAX_INPUT);
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
    
    if (len > MAX_INPUT) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        int j = 0;
        while (i < len && j < MAX_WORD_LEN && !isspace((unsigned char)input[i])) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        
        if (j > 0) {
            words[word_count][j] = '\0';
            word_count++;
        }
    }
    
    printf("Found %d words:\n", word_count);
    for (int idx = 0; idx < word_count; idx++) {
        printf("%d: %s\n", idx + 1, words[idx]);
    }
    
    printf("\nProcessing words:\n");
    int k = 0;
    do {
        if (k >= word_count) {
            break;
        }
        
        int has_vowel = 0;
        int m = 0;
        while (words[k][m] != '\0') {
            char ch = tolower((unsigned char)words[k][m]);
            if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
                has_vowel = 1;
                break;
            }
            m++;
        }
        
        printf("Word '%s' %s vowels\n", words[k], has_vowel ? "contains" : "lacks");
        k++;
    } while (k < word_count);
    
    printf("\nWord lengths:\n");
    for (int idx = 0; idx < word_count; idx++) {
        size_t length = 0;
        char *ptr = words[idx];
        while (*ptr != '\0') {
            length++;
            ptr++;
        }
        printf("'%s': %zu characters\n", words[idx], length);
    }
    
    return 0;
}