//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][64];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter text to process: ");
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
    
    if (input_len >= MAX_INPUT_LEN - 1) {
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
        
        j = i;
        while (j < input_len && !isspace((unsigned char)input[j])) {
            j++;
        }
        
        int word_len = j - i;
        if (word_len >= 64) {
            word_len = 63;
        }
        
        if (word_len > 0) {
            for (k = 0; k < word_len; k++) {
                words[word_count][k] = tolower((unsigned char)input[i + k]);
            }
            words[word_count][word_len] = '\0';
            word_count++;
        }
        
        i = j;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    printf("\nProcessing results:\n");
    printf("Total words: %d\n", word_count);
    
    printf("Unique words and frequencies:\n");
    int processed[MAX_WORDS] = {0};
    
    for (i = 0; i < word_count; i++) {
        if (processed[i]) {
            continue;
        }
        
        int count = 1;
        for (j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                count++;
                processed[j] = 1;
            }
        }
        
        printf("'%s': %d\n", words[i], count);
        processed[i] = 1;
    }
    
    printf("\nSorted by frequency (descending):\n");
    char temp_word[64];
    int temp_count;
    
    int frequencies[MAX_WORDS];
    char sorted_words[MAX_WORDS][64];
    
    for (i = 0; i < word_count; i++) {
        frequencies[i] = 0;
        sorted_words[i][0] = '\0';
    }
    
    int unique_count = 0;
    for (i = 0; i < word_count; i++) {
        if (processed[i] == 2) {
            continue;
        }
        
        strncpy(sorted_words[unique_count], words[i], 63);
        sorted_words[unique_count][63] = '\0';
        
        frequencies[unique_count] = 1;
        processed[i] = 2;
        
        for (j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                frequencies[unique_count]++;
                processed[j] = 2;
            }
        }
        unique_count++;
    }
    
    for (i = 0; i < unique_count - 1; i++) {
        for (j = 0; j < unique_count - i - 1; j++) {
            if (frequencies[j] < frequencies[j + 1]) {
                temp_count = frequencies[j];
                frequencies[j] = frequencies[j + 1];
                frequencies[j + 1] = temp_count;
                
                strncpy(temp_word, sorted_words[j], 63);
                temp_word[63] = '\0';
                strncpy(sorted_words[j], sorted_words[j + 1], 63);
                sorted_words[j][63] = '\0';
                strncpy(sorted_words[j + 1], temp_word, 63);
                sorted_words[j + 1][63] = '\0';
            }
        }
    }
    
    for (i = 0; i < unique_count; i++) {
        printf("'%s': %d\n", sorted_words[i], frequencies[i]);
    }
    
    return 0;
}