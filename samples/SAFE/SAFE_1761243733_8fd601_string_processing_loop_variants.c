//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 256
#define MAX_WORDS 50

int main() {
    char input[MAX_INPUT + 1];
    char words[MAX_WORDS][MAX_INPUT + 1];
    int word_count = 0;
    
    printf("Enter a string (max %d characters): ", MAX_INPUT);
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
    
    int i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        int start = i;
        while (i < len && !isspace((unsigned char)input[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len > MAX_INPUT) {
            word_len = MAX_INPUT;
        }
        
        strncpy(words[word_count], &input[start], word_len);
        words[word_count][word_len] = '\0';
        word_count++;
    }
    
    printf("Found %d words:\n", word_count);
    for (int j = 0; j < word_count; j++) {
        printf("%d: \"%s\"\n", j + 1, words[j]);
    }
    
    printf("\nProcessing words:\n");
    int k = 0;
    do {
        if (word_count == 0) break;
        
        char processed[MAX_INPUT + 1];
        int p_idx = 0;
        char *current = words[k];
        
        for (int m = 0; current[m] != '\0' && p_idx < MAX_INPUT; m++) {
            if (isalpha((unsigned char)current[m])) {
                processed[p_idx++] = tolower((unsigned char)current[m]);
            }
        }
        processed[p_idx] = '\0';
        
        printf("Word %d processed: %s\n", k + 1, processed);
        k++;
    } while (k < word_count);
    
    printf("\nWord lengths:\n");
    for (int n = 0; n < word_count; n++) {
        size_t length = strlen(words[n]);
        printf("'%s' has %zu characters\n", words[n], length);
    }
    
    return 0;
}