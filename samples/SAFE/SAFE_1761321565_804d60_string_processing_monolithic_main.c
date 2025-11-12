//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_INPUT_LEN + 1];
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
        
        j = i;
        while (j < len && !isspace((unsigned char)input[j])) {
            j++;
        }
        
        int word_len = j - i;
        if (word_len > 0 && word_len <= MAX_INPUT_LEN) {
            if (word_count < MAX_WORDS) {
                strncpy(words[word_count], input + i, word_len);
                words[word_count][word_len] = '\0';
                word_count++;
            }
        }
        
        i = j;
    }
    
    printf("Found %d words:\n", word_count);
    for (i = 0; i < word_count; i++) {
        printf("%d: \"%s\"\n", i + 1, words[i]);
    }
    
    printf("\nProcessing words...\n");
    for (i = 0; i < word_count; i++) {
        int word_len = strlen(words[i]);
        for (j = 0; j < word_len; j++) {
            if (islower((unsigned char)words[i][j])) {
                words[i][j] = toupper((unsigned char)words[i][j]);
            } else if (isupper((unsigned char)words[i][j])) {
                words[i][j] = tolower((unsigned char)words[i][j]);
            }
        }
    }
    
    printf("Processed words (case swapped):\n");
    for (i = 0; i < word_count; i++) {
        printf("%d: \"%s\"\n", i + 1, words[i]);
    }
    
    printf("\nReversed word order:\n");
    for (i = word_count - 1; i >= 0; i--) {
        printf("%d: \"%s\"\n", word_count - i, words[i]);
    }
    
    return 0;
}