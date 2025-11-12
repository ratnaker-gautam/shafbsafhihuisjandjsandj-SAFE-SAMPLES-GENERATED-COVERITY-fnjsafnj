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
    int idx = 0;
    for (idx = 0; idx < word_count; idx++) {
        char temp[MAX_WORD_LEN + 1];
        size_t word_len = strlen(words[idx]);
        
        if (word_len > 0) {
            int pos = 0;
            for (pos = 0; pos < (int)word_len; pos++) {
                temp[pos] = toupper((unsigned char)words[idx][pos]);
            }
            temp[word_len] = '\0';
            
            printf("Uppercase: %s -> ", words[idx]);
            
            int n = 0;
            while (n < (int)word_len) {
                putchar(temp[n]);
                n++;
            }
            printf("\n");
        }
    }
    
    printf("\nWord statistics:\n");
    int total_chars = 0;
    int w;
    for (w = 0; w < word_count; w++) {
        total_chars += (int)strlen(words[w]);
    }
    
    if (word_count > 0) {
        double avg_length = (double)total_chars / word_count;
        printf("Average word length: %.2f\n", avg_length);
    } else {
        printf("No words to calculate average\n");
    }
    
    return 0;
}