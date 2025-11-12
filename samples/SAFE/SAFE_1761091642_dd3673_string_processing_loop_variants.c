//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 1024
#define MAX_WORDS 512

int main() {
    char input[MAX_INPUT + 1];
    char words[MAX_WORDS][64];
    int word_count = 0;
    
    printf("Enter text: ");
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
            strncpy(words[word_count], &input[start], word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    printf("Found %d words:\n", word_count);
    
    for (int j = 0; j < word_count; j++) {
        int k = 0;
        char processed[64];
        
        while (words[j][k] != '\0' && k < 63) {
            processed[k] = tolower((unsigned char)words[j][k]);
            k++;
        }
        processed[k] = '\0';
        
        int unique = 1;
        for (int m = 0; m < j; m++) {
            char temp[64];
            int n = 0;
            while (words[m][n] != '\0' && n < 63) {
                temp[n] = tolower((unsigned char)words[m][n]);
                n++;
            }
            temp[n] = '\0';
            
            if (strcmp(processed, temp) == 0) {
                unique = 0;
                break;
            }
        }
        
        if (unique) {
            printf("%s\n", words[j]);
        }
    }
    
    return 0;
}