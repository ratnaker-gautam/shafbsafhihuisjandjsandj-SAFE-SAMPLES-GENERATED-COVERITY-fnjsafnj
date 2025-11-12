//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 256
#define MAX_WORDS 50

int main() {
    char input[MAX_INPUT];
    char words[MAX_WORDS][MAX_INPUT];
    int word_count = 0;
    
    printf("Enter a sentence: ");
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
    
    int i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        int start = i;
        while (i < len && !isspace(input[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len >= MAX_INPUT) {
            word_len = MAX_INPUT - 1;
        }
        
        strncpy(words[word_count], input + start, word_len);
        words[word_count][word_len] = '\0';
        word_count++;
    }
    
    printf("Words found: %d\n", word_count);
    
    for (int j = 0; j < word_count; j++) {
        int k = 0;
        while (words[j][k] != '\0') {
            words[j][k] = toupper((unsigned char)words[j][k]);
            k++;
        }
    }
    
    printf("Uppercase words:\n");
    int j = 0;
    do {
        if (word_count > 0) {
            printf("%s\n", words[j]);
            j++;
        }
    } while (j < word_count);
    
    printf("Reversed order:\n");
    for (int idx = word_count - 1; idx >= 0; idx--) {
        char reversed[MAX_INPUT];
        int pos = 0;
        int str_len = strlen(words[idx]);
        
        for (int rev_idx = str_len - 1; rev_idx >= 0; rev_idx--) {
            if (pos < MAX_INPUT - 1) {
                reversed[pos++] = words[idx][rev_idx];
            }
        }
        reversed[pos] = '\0';
        printf("%s\n", reversed);
    }
    
    return 0;
}