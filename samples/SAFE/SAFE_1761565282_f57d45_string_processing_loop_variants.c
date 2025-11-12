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
        printf("No input provided\n");
        return 0;
    }
    
    i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        j = 0;
        while (i < len && !isspace((unsigned char)input[i]) && j < 63) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        words[word_count][j] = '\0';
        
        if (j > 0) {
            word_count++;
        }
    }
    
    printf("Words found: %d\n", word_count);
    
    for (k = 0; k < word_count; k++) {
        printf("Word %d: ", k + 1);
        
        int word_len = strlen(words[k]);
        int m = 0;
        do {
            if (m < word_len) {
                putchar(words[k][m]);
            }
            m++;
        } while (m <= word_len);
        
        printf(" (length: %d)\n", word_len);
    }
    
    char reversed[MAX_INPUT] = {0};
    int pos = 0;
    
    for (k = word_count - 1; k >= 0; k--) {
        int word_len = strlen(words[k]);
        if (pos + word_len + 1 >= MAX_INPUT) break;
        
        for (int n = 0; n < word_len; n++) {
            reversed[pos++] = words[k][n];
        }
        
        if (k > 0 && pos < MAX_INPUT - 1) {
            reversed[pos++] = ' ';
        }
    }
    reversed[pos] = '\0';
    
    if (pos > 0) {
        printf("Reversed word order: %s\n", reversed);
    }
    
    int total_chars = 0;
    int char_count[256] = {0};
    
    for (k = 0; k < word_count; k++) {
        int word_len = strlen(words[k]);
        total_chars += word_len;
        
        for (int p = 0; p < word_len; p++) {
            unsigned char c = (unsigned char)words[k][p];
            if (c < 256) {
                char_count[c]++;
            }
        }
    }
    
    printf("Total characters in words: %d\n", total_chars);
    
    printf("Character frequencies:\n");
    for (int c = 0; c < 256; c++) {
        if (char_count[c] > 0 && isprint(c)) {
            printf("'%c': %d\n", c, char_count[c]);
        }
    }
    
    return 0;
}