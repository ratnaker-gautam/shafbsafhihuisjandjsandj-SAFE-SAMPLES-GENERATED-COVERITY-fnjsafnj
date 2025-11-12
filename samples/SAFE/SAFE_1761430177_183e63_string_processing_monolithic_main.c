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
    if (input_len == 0 || (input_len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 0;
    }
    
    if (input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len >= sizeof(input)) {
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
        return 0;
    }
    
    for (i = 0; i < word_count; i++) {
        for (j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) > 0) {
                char temp[64];
                strncpy(temp, words[i], sizeof(temp) - 1);
                temp[sizeof(temp) - 1] = '\0';
                strncpy(words[i], words[j], sizeof(words[i]) - 1);
                words[i][sizeof(words[i]) - 1] = '\0';
                strncpy(words[j], temp, sizeof(words[j]) - 1);
                words[j][sizeof(words[j]) - 1] = '\0';
            }
        }
    }
    
    printf("Sorted unique words:\n");
    for (i = 0; i < word_count; i++) {
        if (i == 0 || strcmp(words[i], words[i - 1]) != 0) {
            int count = 1;
            for (j = i + 1; j < word_count; j++) {
                if (strcmp(words[i], words[j]) == 0) {
                    count++;
                } else {
                    break;
                }
            }
            printf("%s (appears %d time%s)\n", words[i], count, count == 1 ? "" : "s");
            i = j - 1;
        }
    }
    
    return 0;
}