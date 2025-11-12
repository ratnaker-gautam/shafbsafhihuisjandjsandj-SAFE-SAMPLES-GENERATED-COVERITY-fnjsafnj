//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 1024
#define MAX_WORDS 512

int main() {
    char input[MAX_INPUT];
    char words[MAX_WORDS][64];
    int word_count = 0;
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        int start = i;
        while (i < len && !isspace(input[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len > 0 && word_len < 64) {
            strncpy(words[word_count], input + start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    printf("Words found: %d\n", word_count);
    
    for (int j = 0; j < word_count; j++) {
        printf("Word %d: ", j + 1);
        
        int k = 0;
        do {
            if (k < strlen(words[j])) {
                putchar(words[j][k]);
                k++;
            }
        } while (k < strlen(words[j]));
        
        printf(" (length: %zu)\n", strlen(words[j]));
    }
    
    printf("\nProcessing words:\n");
    int j = 0;
    while (j < word_count) {
        char processed[64];
        int k = 0;
        int m = 0;
        
        for (k = 0; words[j][k] != '\0' && m < 63; k++) {
            if (isalpha(words[j][k])) {
                processed[m++] = tolower(words[j][k]);
            }
        }
        processed[m] = '\0';
        
        if (m > 0) {
            printf("Processed word %d: %s\n", j + 1, processed);
        }
        
        j++;
    }
    
    printf("\nWord lengths:\n");
    int total_chars = 0;
    for (int idx = 0; idx < word_count; idx++) {
        size_t word_len = strlen(words[idx]);
        total_chars += word_len;
        printf("Word %d: %zu characters\n", idx + 1, word_len);
    }
    
    if (word_count > 0) {
        printf("Average word length: %.2f\n", (float)total_chars / word_count);
    }
    
    return 0;
}