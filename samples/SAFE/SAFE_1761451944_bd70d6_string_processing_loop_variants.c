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
        while (i < len && j < MAX_WORD_LEN && !isspace((unsigned char)input[i])) {
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
        
        int word_len = strlen(words[k]);
        for (i = 0; i < word_len; i++) {
            putchar(words[k][i]);
        }
        putchar('\n');
    }
    
    char reversed[MAX_INPUT_LEN + 1];
    int rev_idx = 0;
    
    for (i = word_count - 1; i >= 0; i--) {
        int word_len = strlen(words[i]);
        if (rev_idx + word_len + 1 > MAX_INPUT_LEN) {
            break;
        }
        
        for (j = 0; j < word_len; j++) {
            reversed[rev_idx] = words[i][j];
            rev_idx++;
        }
        
        if (i > 0) {
            reversed[rev_idx] = ' ';
            rev_idx++;
        }
    }
    reversed[rev_idx] = '\0';
    
    printf("Reversed words: %s\n", reversed);
    
    int total_chars = 0;
    int char_count[256] = {0};
    
    for (i = 0; i < len; i++) {
        unsigned char c = (unsigned char)input[i];
        if (c < 256) {
            char_count[c]++;
            total_chars++;
        }
    }
    
    printf("Character frequencies:\n");
    for (i = 0; i < 256; i++) {
        if (char_count[i] > 0 && isprint(i)) {
            printf("'%c': %d\n", (char)i, char_count[i]);
        }
    }
    
    return 0;
}