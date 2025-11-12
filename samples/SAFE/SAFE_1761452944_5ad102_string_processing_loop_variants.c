//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    size_t i = 0;
    while (i < input_len && word_count < MAX_WORDS) {
        while (i < input_len && isspace(input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        size_t word_start = i;
        while (i < input_len && !isspace(input[i])) {
            i++;
        }
        
        size_t word_len = i - word_start;
        if (word_len >= MAX_WORD_LEN) {
            word_len = MAX_WORD_LEN - 1;
        }
        
        if (word_len > 0) {
            strncpy(words[word_count], &input[word_start], word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    printf("Words found: %d\n", word_count);
    
    for (int j = 0; j < word_count; j++) {
        size_t k = 0;
        while (words[j][k] != '\0') {
            words[j][k] = toupper(words[j][k]);
            k++;
        }
    }
    
    printf("Uppercase words:\n");
    int m = 0;
    do {
        if (word_count > 0) {
            printf("%s\n", words[m]);
        }
        m++;
    } while (m < word_count);
    
    printf("Reversed order:\n");
    for (int n = word_count - 1; n >= 0; n--) {
        size_t len = strlen(words[n]);
        if (len > 0) {
            for (int p = len - 1; p >= 0; p--) {
                putchar(words[n][p]);
            }
            putchar('\n');
        }
    }
    
    return 0;
}