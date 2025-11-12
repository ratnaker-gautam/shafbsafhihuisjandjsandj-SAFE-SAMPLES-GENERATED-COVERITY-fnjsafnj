//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 30

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int word_lengths[MAX_WORDS];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN);
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
    
    int i = 0;
    while (i < (int)input_len && word_count < MAX_WORDS) {
        while (i < (int)input_len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= (int)input_len) {
            break;
        }
        
        int start = i;
        int j = 0;
        while (i < (int)input_len && !isspace((unsigned char)input[i]) && j < MAX_WORD_LEN) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        
        if (j >= MAX_WORD_LEN) {
            fprintf(stderr, "Word too long\n");
            return 1;
        }
        
        words[word_count][j] = '\0';
        word_lengths[word_count] = j;
        word_count++;
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    printf("Words found: %d\n", word_count);
    
    for (int idx = 0; idx < word_count; idx++) {
        printf("Word %d: \"%s\" (length: %d)\n", idx + 1, words[idx], word_lengths[idx]);
    }
    
    int total_chars = 0;
    int k = 0;
    do {
        total_chars += word_lengths[k];
        k++;
    } while (k < word_count);
    
    printf("Total characters in words: %d\n", total_chars);
    
    char longest_word[MAX_WORD_LEN + 1];
    int max_len = 0;
    int m = 0;
    for (; m < word_count; m++) {
        if (word_lengths[m] > max_len) {
            max_len = word_lengths[m];
            strncpy(longest_word, words[m], MAX_WORD_LEN);
            longest_word[MAX_WORD_LEN] = '\0';
        }
    }
    
    if (max_len > 0) {
        printf("Longest word: \"%s\" (length: %d)\n", longest_word, max_len);
    }
    
    char reversed[MAX_INPUT_LEN + 1];
    int n = (int)input_len - 1;
    int pos = 0;
    while (n >= 0) {
        reversed[pos] = input[n];
        pos++;
        n--;
    }
    reversed[pos] = '\0';
    
    printf("Reversed string: \"%s\"\n", reversed);
    
    return 0;
}