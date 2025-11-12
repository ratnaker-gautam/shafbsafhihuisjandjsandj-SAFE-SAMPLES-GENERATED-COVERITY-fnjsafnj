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
    char temp[MAX_WORD_LEN + 1];
    
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
    
    int i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        int j = 0;
        while (i < len && j < MAX_WORD_LEN && !isspace(input[i])) {
            temp[j] = input[i];
            j++;
            i++;
        }
        
        if (j > 0) {
            temp[j] = '\0';
            strncpy(words[word_count], temp, MAX_WORD_LEN);
            words[word_count][MAX_WORD_LEN] = '\0';
            word_count++;
        }
    }
    
    printf("Found %d words:\n", word_count);
    for (int idx = 0; idx < word_count; idx++) {
        printf("%d: %s\n", idx + 1, words[idx]);
    }
    
    printf("\nProcessing words:\n");
    int k = 0;
    do {
        if (k >= word_count) {
            break;
        }
        
        int word_len = strlen(words[k]);
        int vowel_count = 0;
        
        for (int m = 0; m < word_len; m++) {
            char ch = tolower(words[k][m]);
            if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
                vowel_count++;
            }
        }
        
        printf("Word '%s' has %d vowels\n", words[k], vowel_count);
        k++;
    } while (k < word_count);
    
    printf("\nReversed words:\n");
    for (int idx = word_count - 1; idx >= 0; idx--) {
        int word_len = strlen(words[idx]);
        char reversed[MAX_WORD_LEN + 1];
        
        int pos = 0;
        for (int m = word_len - 1; m >= 0; m--) {
            if (pos < MAX_WORD_LEN) {
                reversed[pos] = words[idx][m];
                pos++;
            }
        }
        reversed[pos] = '\0';
        
        printf("%s ", reversed);
    }
    printf("\n");
    
    return 0;
}