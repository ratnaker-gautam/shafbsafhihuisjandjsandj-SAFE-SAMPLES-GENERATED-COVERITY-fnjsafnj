//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter text to analyze: ");
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
    
    if (input_len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
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
        
        if (j > 0) {
            words[word_count][j] = '\0';
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 1;
    }
    
    printf("\nWord analysis:\n");
    printf("Total words: %d\n", word_count);
    
    int unique_count = 0;
    for (i = 0; i < word_count; i++) {
        int is_unique = 1;
        for (j = 0; j < i; j++) {
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
    
    printf("\nWord frequencies:\n");
    for (i = 0; i < word_count; i++) {
        int counted = 0;
        for (j = 0; j < i; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                counted = 1;
                break;
            }
        }
        
        if (!counted) {
            int frequency = 1;
            for (j = i + 1; j < word_count; j++) {
                if (strcmp(words[i], words[j]) == 0) {
                    frequency++;
                }
            }
            printf("'%s': %d\n", words[i], frequency);
        }
    }
    
    printf("\nWords sorted by length:\n");
    for (i = 0; i < word_count - 1; i++) {
        for (j = 0; j < word_count - i - 1; j++) {
            size_t len1 = strlen(words[j]);
            size_t len2 = strlen(words[j + 1]);
            if (len1 > len2 || (len1 == len2 && strcmp(words[j], words[j + 1]) > 0)) {
                char temp[MAX_WORD_LEN];
                if (strlen(words[j]) < MAX_WORD_LEN && strlen(words[j + 1]) < MAX_WORD_LEN) {
                    strcpy(temp, words[j]);
                    strcpy(words[j], words[j + 1]);
                    strcpy(words[j + 1], temp);
                }
            }
        }
    }
    
    for (i = 0; i < word_count; i++) {
        printf("'%s' (length: %zu)\n", words[i], strlen(words[i]));
    }
    
    return 0;
}