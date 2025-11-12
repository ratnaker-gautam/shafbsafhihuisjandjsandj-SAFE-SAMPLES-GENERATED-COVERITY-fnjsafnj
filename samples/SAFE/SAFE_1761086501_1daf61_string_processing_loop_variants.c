//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: string_processing
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
    
    if (len > MAX_INPUT) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        int start = i;
        while (i < len && !isspace((unsigned char)input[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len > MAX_INPUT) {
            fprintf(stderr, "Word too long\n");
            return 1;
        }
        
        if (word_count < MAX_WORDS) {
            strncpy(words[word_count], &input[start], word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    printf("Found %d words:\n", word_count);
    for (int j = 0; j < word_count; j++) {
        printf("%d: \"%s\"\n", j + 1, words[j]);
    }
    
    printf("\nProcessing words:\n");
    int j = 0;
    do {
        if (j >= word_count) {
            break;
        }
        
        char processed[MAX_INPUT + 1];
        int k = 0;
        int l = 0;
        
        while (words[j][k] != '\0' && l < MAX_INPUT) {
            if (isalpha((unsigned char)words[j][k])) {
                processed[l] = toupper((unsigned char)words[j][k]);
                l++;
            }
            k++;
        }
        processed[l] = '\0';
        
        printf("Word %d processed: %s -> %s\n", j + 1, words[j], processed);
        j++;
    } while (j < word_count);
    
    printf("\nWord lengths:\n");
    for (int m = 0; m < word_count; m++) {
        size_t length = strlen(words[m]);
        printf("Word %d: %zu characters\n", m + 1, length);
    }
    
    return 0;
}