//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: string_processing
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
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        j = 0;
        while (i < len && !isspace(input[i]) && j < MAX_WORD_LEN) {
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
            if (m < strlen(words[k])) {
                putchar(words[k][m]);
                m++;
            }
        } while (m < strlen(words[k]));
        
        printf("\n");
    }
    
    char processed[MAX_INPUT_LEN + 1];
    int pos = 0;
    
    for (k = 0; k < word_count && pos < MAX_INPUT_LEN; k++) {
        int word_len = strlen(words[k]);
        
        if (pos + word_len + 1 > MAX_INPUT_LEN) {
            break;
        }
        
        if (k > 0) {
            processed[pos] = ' ';
            pos++;
        }
        
        int n = 0;
        for (n = 0; n < word_len && pos < MAX_INPUT_LEN; n++) {
            processed[pos] = toupper(words[k][n]);
            pos++;
        }
    }
    
    processed[pos] = '\0';
    
    printf("Processed string: %s\n", processed);
    
    int vowel_counts[MAX_WORDS] = {0};
    
    for (k = 0; k < word_count; k++) {
        int count = 0;
        char *ptr = words[k];
        
        while (*ptr != '\0') {
            char ch = tolower(*ptr);
            if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
                count++;
            }
            ptr++;
        }
        
        vowel_counts[k] = count;
    }
    
    printf("Vowel counts per word:\n");
    k = 0;
    while (k < word_count) {
        printf("'%s': %d vowels\n", words[k], vowel_counts[k]);
        k++;
    }
    
    return 0;
}