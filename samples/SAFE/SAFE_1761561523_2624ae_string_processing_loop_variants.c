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
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN);
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
    
    printf("Found %d words:\n", word_count);
    
    for (k = 0; k < word_count; k++) {
        printf("Word %d: ", k + 1);
        
        int m = 0;
        int word_len = strlen(words[k]);
        do {
            if (m < word_len) {
                putchar(words[k][m]);
                m++;
            }
        } while (m < word_len);
        
        printf("\n");
    }
    
    printf("\nProcessing words:\n");
    for (k = 0; k < word_count; k++) {
        int word_len = strlen(words[k]);
        char processed[MAX_WORD_LEN + 1];
        int p_idx = 0;
        
        for (int n = 0; n < word_len && p_idx < MAX_WORD_LEN; n++) {
            if (isalpha((unsigned char)words[k][n])) {
                processed[p_idx] = tolower((unsigned char)words[k][n]);
                p_idx++;
            }
        }
        processed[p_idx] = '\0';
        
        if (p_idx > 0) {
            printf("Processed word %d: ", k + 1);
            int q = 0;
            while (q < p_idx) {
                putchar(processed[q]);
                q++;
            }
            printf("\n");
        }
    }
    
    printf("\nWord statistics:\n");
    int total_chars = 0;
    int char_count[26] = {0};
    
    for (k = 0; k < word_count; k++) {
        int word_len = strlen(words[k]);
        total_chars += word_len;
        
        for (int r = 0; r < word_len; r++) {
            char c = tolower((unsigned char)words[k][r]);
            if (c >= 'a' && c <= 'z') {
                char_count[c - 'a']++;
            }
        }
    }
    
    printf("Total characters in words: %d\n", total_chars);
    printf("Character frequency:\n");
    
    for (int s = 0; s < 26; s++) {
        if (char_count[s] > 0) {
            printf("%c: %d\n", 'a' + s, char_count[s]);
        }
    }
    
    return 0;
}