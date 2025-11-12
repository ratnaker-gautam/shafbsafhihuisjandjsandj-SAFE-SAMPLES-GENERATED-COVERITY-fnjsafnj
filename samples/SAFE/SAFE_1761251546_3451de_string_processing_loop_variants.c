//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter a string: ");
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
        
        j = 0;
        while (i < len && !isspace((unsigned char)input[i]) && j < MAX_WORD_LEN) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        
        if (j > 0) {
            words[word_count][j] = '\0';
            word_count++;
        }
    }
    
    printf("Words found: %d\n", word_count);
    
    for (k = 0; k < word_count; k++) {
        printf("Word %d: ", k + 1);
        
        int m = 0;
        do {
            if (m < (int)strlen(words[k])) {
                putchar(words[k][m]);
                m++;
            }
        } while (m < (int)strlen(words[k]));
        
        printf(" (length: %zu)\n", strlen(words[k]));
    }
    
    printf("\nProcessing words:\n");
    int n = 0;
    for (n = 0; n < word_count; n++) {
        char processed[MAX_WORD_LEN + 1];
        int p_idx = 0;
        int found_upper = 0;
        
        int idx = 0;
        while (words[n][idx] != '\0' && p_idx < MAX_WORD_LEN) {
            if (isupper((unsigned char)words[n][idx])) {
                found_upper = 1;
                processed[p_idx] = tolower((unsigned char)words[n][idx]);
            } else {
                processed[p_idx] = words[n][idx];
            }
            p_idx++;
            idx++;
        }
        processed[p_idx] = '\0';
        
        printf("Original: %s -> ", words[n]);
        if (found_upper) {
            printf("Processed: %s\n", processed);
        } else {
            printf("No uppercase letters\n");
        }
    }
    
    printf("\nWord lengths analysis:\n");
    int length_counts[11] = {0};
    
    int w;
    for (w = 0; w < word_count; w++) {
        size_t wlen = strlen(words[w]);
        if (wlen < 10) {
            length_counts[wlen]++;
        } else {
            length_counts[10]++;
        }
    }
    
    int l;
    for (l = 1; l <= 10; l++) {
        if (l < 10) {
            printf("Length %d: %d words\n", l, length_counts[l]);
        } else {
            printf("Length 10+: %d words\n", length_counts[10]);
        }
    }
    
    return 0;
}