//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][64];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter text to process: ");
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
        return 1;
    }
    
    if (input_len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < input_len && word_count < MAX_WORDS) {
        while (i < input_len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        j = i;
        while (j < input_len && !isspace((unsigned char)input[j])) {
            j++;
        }
        
        int word_len = j - i;
        if (word_len > 0 && word_len < 64) {
            if (word_count < MAX_WORDS) {
                strncpy(words[word_count], &input[i], word_len);
                words[word_count][word_len] = '\0';
                word_count++;
            }
        }
        
        i = j;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    printf("\nProcessing results:\n");
    printf("Total words: %d\n", word_count);
    
    printf("\nWord frequency analysis:\n");
    int processed[MAX_WORDS] = {0};
    
    for (i = 0; i < word_count; i++) {
        if (processed[i]) {
            continue;
        }
        
        int count = 1;
        for (j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                count++;
                processed[j] = 1;
            }
        }
        
        printf("'%s': %d occurrence(s)\n", words[i], count);
        processed[i] = 1;
    }
    
    printf("\nWords in reverse order:\n");
    for (i = word_count - 1; i >= 0; i--) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    printf("\nLongest word(s):\n");
    int max_len = 0;
    for (i = 0; i < word_count; i++) {
        int len = strlen(words[i]);
        if (len > max_len) {
            max_len = len;
        }
    }
    
    for (i = 0; i < word_count; i++) {
        if (strlen(words[i]) == max_len) {
            printf("'%s' (length: %d)\n", words[i], max_len);
        }
    }
    
    printf("\nCharacter frequency in entire text:\n");
    int char_count[256] = {0};
    for (i = 0; i < input_len; i++) {
        unsigned char c = input[i];
        if (c < 256) {
            char_count[c]++;
        }
    }
    
    for (i = 0; i < 256; i++) {
        if (char_count[i] > 0 && isprint(i)) {
            printf("'%c': %d\n", (char)i, char_count[i]);
        }
    }
    
    return 0;
}