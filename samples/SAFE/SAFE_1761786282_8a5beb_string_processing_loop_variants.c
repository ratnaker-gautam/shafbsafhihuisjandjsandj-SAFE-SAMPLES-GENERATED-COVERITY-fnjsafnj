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
        
        int word_len = strlen(words[k]);
        for (i = 0; i < word_len; i++) {
            putchar(words[k][i]);
        }
        putchar('\n');
    }
    
    char reversed[MAX_INPUT_LEN + 1];
    int rev_index = 0;
    
    for (k = word_count - 1; k >= 0; k--) {
        int word_len = strlen(words[k]);
        for (i = 0; i < word_len; i++) {
            if (rev_index < MAX_INPUT_LEN) {
                reversed[rev_index] = words[k][i];
                rev_index++;
            }
        }
        if (k > 0 && rev_index < MAX_INPUT_LEN) {
            reversed[rev_index] = ' ';
            rev_index++;
        }
    }
    
    if (rev_index < MAX_INPUT_LEN) {
        reversed[rev_index] = '\0';
    } else {
        reversed[MAX_INPUT_LEN] = '\0';
    }
    
    printf("Reversed word order: ");
    for (i = 0; i < rev_index; i++) {
        putchar(reversed[i]);
    }
    putchar('\n');
    
    int total_chars = 0;
    for (k = 0; k < word_count; k++) {
        total_chars += strlen(words[k]);
    }
    
    printf("Total characters in words: %d\n", total_chars);
    
    return 0;
}