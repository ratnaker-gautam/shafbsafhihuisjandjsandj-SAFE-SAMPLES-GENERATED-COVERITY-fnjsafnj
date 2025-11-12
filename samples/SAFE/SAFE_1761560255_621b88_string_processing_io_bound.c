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
        printf("Empty input\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
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
        while (i < input_len && j < MAX_WORD_LEN && !isspace((unsigned char)input[i])) {
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
    
    printf("\nProcessing words...\n");
    printf("Found %d words:\n", word_count);
    
    for (i = 0; i < word_count; i++) {
        printf("Word %d: \"%s\" (length: %zu)\n", 
               i + 1, words[i], strlen(words[i]));
    }
    
    printf("\nWord frequency analysis:\n");
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
        
        printf("'%s' appears %d time%s\n", 
               words[i], count, count == 1 ? "" : "s");
        processed[i] = 1;
    }
    
    printf("\nReversed words:\n");
    for (i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        printf("'%s' reversed: '", words[i]);
        
        for (j = len - 1; j >= 0; j--) {
            putchar(words[i][j]);
        }
        printf("'\n");
    }
    
    printf("\nWords sorted by length:\n");
    int indices[MAX_WORDS];
    for (i = 0; i < word_count; i++) {
        indices[i] = i;
    }
    
    for (i = 0; i < word_count - 1; i++) {
        for (j = 0; j < word_count - i - 1; j++) {
            size_t len1 = strlen(words[indices[j]]);
            size_t len2 = strlen(words[indices[j + 1]]);
            if (len1 > len2) {
                int temp = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = temp;
            }
        }
    }
    
    for (i = 0; i < word_count; i++) {
        printf("%s ", words[indices[i]]);
    }
    printf("\n");
    
    return 0;
}