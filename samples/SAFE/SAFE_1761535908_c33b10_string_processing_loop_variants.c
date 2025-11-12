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
        if (word_len >= 64) {
            word_len = 63;
        }
        
        strncpy(words[word_count], &input[start], word_len);
        words[word_count][word_len] = '\0';
        word_count++;
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 1;
    }
    
    printf("Processing words:\n");
    for (int j = 0; j < word_count; j++) {
        int k = 0;
        char processed[64];
        
        while (words[j][k] != '\0' && k < 63) {
            processed[k] = toupper(words[j][k]);
            k++;
        }
        processed[k] = '\0';
        
        printf("Original: %s -> Uppercase: %s\n", words[j], processed);
    }
    
    printf("\nWord statistics:\n");
    int total_chars = 0;
    int idx = 0;
    do {
        total_chars += strlen(words[idx]);
        idx++;
    } while (idx < word_count);
    
    printf("Total words: %d\n", word_count);
    printf("Average word length: %.2f\n", word_count > 0 ? (double)total_chars / word_count : 0.0);
    
    printf("\nWords in reverse order:\n");
    for (int j = word_count - 1; j >= 0; j--) {
        int pos = 0;
        char reversed[64];
        int word_len = strlen(words[j]);
        
        for (int k = word_len - 1; k >= 0; k--) {
            reversed[pos++] = words[j][k];
        }
        reversed[pos] = '\0';
        
        printf("%s ", reversed);
    }
    printf("\n");
    
    return 0;
}