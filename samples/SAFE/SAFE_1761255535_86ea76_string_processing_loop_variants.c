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
    int i, j, k;
    
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
        printf("Empty input\n");
        return 0;
    }
    
    i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        j = i;
        while (j < len && !isspace((unsigned char)input[j])) {
            j++;
        }
        
        int word_len = j - i;
        if (word_len > 0 && word_len < 64) {
            strncpy(words[word_count], input + i, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
        
        i = j;
    }
    
    printf("Words found: %d\n", word_count);
    
    for (k = 0; k < word_count; k++) {
        int m = 0;
        int n = strlen(words[k]) - 1;
        
        while (m < n) {
            char temp = words[k][m];
            words[k][m] = words[k][n];
            words[k][n] = temp;
            m++;
            n--;
        }
    }
    
    printf("Reversed words:\n");
    for (i = 0; i < word_count; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    int freq[MAX_WORDS] = {0};
    int processed[MAX_WORDS] = {0};
    
    for (i = 0; i < word_count; i++) {
        if (processed[i]) continue;
        
        freq[i] = 1;
        for (j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                freq[i]++;
                processed[j] = 1;
            }
        }
    }
    
    printf("Word frequencies:\n");
    for (i = 0; i < word_count; i++) {
        if (!processed[i] && freq[i] > 0) {
            printf("%s: %d\n", words[i], freq[i]);
        }
    }
    
    return 0;
}