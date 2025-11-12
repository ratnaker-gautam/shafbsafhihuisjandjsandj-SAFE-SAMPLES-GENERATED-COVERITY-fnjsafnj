//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_INPUT_LEN];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
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
    
    i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        j = i;
        while (j < len && !isspace((unsigned char)input[j])) {
            j++;
        }
        
        size_t word_len = j - i;
        if (word_len >= sizeof(words[0])) {
            word_len = sizeof(words[0]) - 1;
        }
        
        for (k = 0; k < word_len; k++) {
            words[word_count][k] = input[i + k];
        }
        words[word_count][word_len] = '\0';
        word_count++;
        
        i = j;
    }
    
    printf("Found %d words:\n", word_count);
    for (i = 0; i < word_count; i++) {
        printf("%d: \"%s\"\n", i + 1, words[i]);
    }
    
    printf("\nProcessing words:\n");
    for (i = 0; i < word_count; i++) {
        size_t current_len = strlen(words[i]);
        int vowel_count = 0;
        int consonant_count = 0;
        
        for (j = 0; j < current_len; j++) {
            char c = tolower((unsigned char)words[i][j]);
            if (c >= 'a' && c <= 'z') {
                if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                    vowel_count++;
                } else {
                    consonant_count++;
                }
            }
        }
        
        printf("Word \"%s\": %d vowels, %d consonants\n", words[i], vowel_count, consonant_count);
    }
    
    printf("\nReversed words:\n");
    for (i = 0; i < word_count; i++) {
        size_t current_len = strlen(words[i]);
        for (j = current_len; j > 0; j--) {
            putchar(words[i][j - 1]);
        }
        printf("\n");
    }
    
    return 0;
}