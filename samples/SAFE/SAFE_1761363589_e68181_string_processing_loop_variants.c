//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main() {
    char input[MAX_INPUT_LEN];
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
        printf("No input provided\n");
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
            strncpy(words[word_count], &input[i], word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
        
        i = j;
    }
    
    printf("Found %d words:\n", word_count);
    
    for (k = 0; k < word_count; k++) {
        printf("%d: \"%s\"\n", k + 1, words[k]);
    }
    
    printf("\nProcessing words:\n");
    
    int m = 0;
    do {
        if (m >= word_count) break;
        
        char current[64];
        strncpy(current, words[m], sizeof(current) - 1);
        current[sizeof(current) - 1] = '\0';
        
        int n = 0;
        while (current[n] != '\0') {
            if (islower((unsigned char)current[n])) {
                current[n] = toupper((unsigned char)current[n]);
            } else if (isupper((unsigned char)current[n])) {
                current[n] = tolower((unsigned char)current[n]);
            }
            n++;
        }
        
        printf("Word %d transformed: %s -> %s\n", m + 1, words[m], current);
        m++;
    } while (m < word_count);
    
    printf("\nWord lengths:\n");
    
    for (int idx = 0; idx < word_count; idx++) {
        size_t length = strlen(words[idx]);
        printf("Word %d: %zu characters\n", idx + 1, length);
    }
    
    return 0;
}