//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
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
    
    printf("\nWords found (%d):\n", word_count);
    for (i = 0; i < word_count; i++) {
        printf("%d: %s\n", i + 1, words[i]);
    }
    
    printf("\nProcessing words...\n");
    for (i = 0; i < word_count; i++) {
        size_t word_len = strlen(words[i]);
        int vowel_count = 0;
        int consonant_count = 0;
        
        for (j = 0; j < word_len; j++) {
            char c = tolower((unsigned char)words[i][j]);
            if (c >= 'a' && c <= 'z') {
                if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                    vowel_count++;
                } else {
                    consonant_count++;
                }
            }
        }
        
        printf("Word '%s': %d vowels, %d consonants, %zu total letters\n", 
               words[i], vowel_count, consonant_count, word_len);
    }
    
    printf("\nReversed words:\n");
    for (i = 0; i < word_count; i++) {
        size_t word_len = strlen(words[i]);
        printf("%d: ", i + 1);
        
        for (j = word_len; j > 0; j--) {
            putchar(words[i][j - 1]);
        }
        putchar('\n');
    }
    
    printf("\nWord lengths:\n");
    int total_chars = 0;
    for (i = 0; i < word_count; i++) {
        size_t word_len = strlen(words[i]);
        total_chars += word_len;
        printf("'%s': %zu characters\n", words[i], word_len);
    }
    
    if (word_count > 0) {
        double avg_length = (double)total_chars / word_count;
        printf("Average word length: %.2f\n", avg_length);
    }
    
    return 0;
}