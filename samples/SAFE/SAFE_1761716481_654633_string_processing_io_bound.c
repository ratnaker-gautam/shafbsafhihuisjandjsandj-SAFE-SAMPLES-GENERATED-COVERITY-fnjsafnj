//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
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
    
    i = 0;
    while (i < input_len && word_count < MAX_WORDS) {
        while (i < input_len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        j = 0;
        while (i < input_len && !isspace((unsigned char)input[i]) && j < MAX_WORD_LEN - 1) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        words[word_count][j] = '\0';
        
        if (j > 0) {
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    printf("\nProcessing words...\n");
    printf("Found %d words:\n", word_count);
    
    for (i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        int vowel_count = 0;
        int consonant_count = 0;
        
        for (j = 0; j < len; j++) {
            char c = tolower((unsigned char)words[i][j]);
            if (c >= 'a' && c <= 'z') {
                if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                    vowel_count++;
                } else {
                    consonant_count++;
                }
            }
        }
        
        printf("Word %d: \"%s\" (length: %zu, vowels: %d, consonants: %d)\n", 
               i + 1, words[i], len, vowel_count, consonant_count);
    }
    
    printf("\nWord frequency analysis:\n");
    for (i = 0; i < word_count; i++) {
        int frequency = 1;
        if (strlen(words[i]) == 0) {
            continue;
        }
        
        for (j = 0; j < i; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                frequency = 0;
                break;
            }
        }
        
        if (frequency > 0) {
            for (j = i + 1; j < word_count; j++) {
                if (strcmp(words[i], words[j]) == 0) {
                    frequency++;
                }
            }
            printf("'%s' appears %d time(s)\n", words[i], frequency);
        }
    }
    
    return 0;
}