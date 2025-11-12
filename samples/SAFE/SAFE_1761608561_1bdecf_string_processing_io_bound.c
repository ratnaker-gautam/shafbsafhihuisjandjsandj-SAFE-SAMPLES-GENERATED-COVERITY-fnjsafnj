//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < input_len && word_count < MAX_WORDS) {
        while (i < input_len && isspace(input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        j = 0;
        while (i < input_len && !isspace(input[i]) && j < MAX_WORD_LEN) {
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
        return 1;
    }
    
    printf("\nProcessing words...\n");
    printf("Found %d words:\n", word_count);
    
    for (i = 0; i < word_count; i++) {
        int len = strlen(words[i]);
        int vowel_count = 0;
        int consonant_count = 0;
        
        for (j = 0; j < len; j++) {
            char c = tolower(words[i][j]);
            if (c >= 'a' && c <= 'z') {
                if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                    vowel_count++;
                } else {
                    consonant_count++;
                }
            }
        }
        
        printf("Word %d: \"%s\" (length: %d, vowels: %d, consonants: %d)\n", 
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
            printf("\"%s\" appears %d time(s)\n", words[i], frequency);
        }
    }
    
    printf("\nWords sorted by length:\n");
    int sorted_indices[MAX_WORDS];
    for (i = 0; i < word_count; i++) {
        sorted_indices[i] = i;
    }
    
    for (i = 0; i < word_count - 1; i++) {
        for (j = 0; j < word_count - i - 1; j++) {
            int len1 = strlen(words[sorted_indices[j]]);
            int len2 = strlen(words[sorted_indices[j + 1]]);
            if (len1 > len2) {
                int temp = sorted_indices[j];
                sorted_indices[j] = sorted_indices[j + 1];
                sorted_indices[j + 1] = temp;
            }
        }
    }
    
    for (i = 0; i < word_count; i++) {
        printf("%s ", words[sorted_indices[i]]);
    }
    printf("\n");
    
    return 0;
}