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
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        int start = i;
        while (i < len && !isspace((unsigned char)input[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len > 0 && word_len < 64) {
            strncpy(words[word_count], input + start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 1;
    }
    
    printf("Processing words:\n");
    for (int j = 0; j < word_count; j++) {
        char processed[64];
        int k = 0;
        int l = 0;
        
        while (words[j][k] != '\0' && l < 63) {
            char c = words[j][k];
            if (isalpha((unsigned char)c)) {
                processed[l] = tolower((unsigned char)c);
                l++;
            }
            k++;
        }
        processed[l] = '\0';
        
        if (l > 0) {
            printf("Original: %s -> Processed: %s\n", words[j], processed);
        }
    }
    
    printf("Word statistics:\n");
    int total_chars = 0;
    int j = 0;
    do {
        total_chars += strlen(words[j]);
        j++;
    } while (j < word_count);
    
    printf("Total words: %d\n", word_count);
    printf("Average word length: %.2f\n", word_count > 0 ? (double)total_chars / word_count : 0.0);
    
    printf("Words in reverse order:\n");
    for (int j = word_count - 1; j >= 0; j--) {
        printf("%s ", words[j]);
    }
    printf("\n");
    
    return 0;
}