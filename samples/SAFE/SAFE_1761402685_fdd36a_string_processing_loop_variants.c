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
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input string\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < input_len && word_count < MAX_WORDS) {
        while (i < input_len && isspace(input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        j = 0;
        while (i < input_len && !isspace(input[i]) && j < MAX_WORD_LEN) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        
        if (j > 0) {
            words[word_count][j] = '\0';
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    printf("Words found (%d):\n", word_count);
    for (k = 0; k < word_count; k++) {
        printf("%d: %s\n", k + 1, words[k]);
    }
    
    printf("\nProcessing words:\n");
    int processed = 0;
    k = 0;
    do {
        size_t len = strlen(words[k]);
        int vowel_count = 0;
        
        for (j = 0; j < len; j++) {
            char c = tolower(words[k][j]);
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                vowel_count++;
            }
        }
        
        printf("Word '%s' has %d vowels\n", words[k], vowel_count);
        processed++;
        k++;
    } while (processed < word_count);
    
    printf("\nReversed words:\n");
    for (k = 0; k < word_count; k++) {
        size_t len = strlen(words[k]);
        if (len > 1) {
            char temp;
            for (i = 0, j = len - 1; i < j; i++, j--) {
                temp = words[k][i];
                words[k][i] = words[k][j];
                words[k][j] = temp;
            }
        }
        printf("%s ", words[k]);
    }
    printf("\n");
    
    return 0;
}