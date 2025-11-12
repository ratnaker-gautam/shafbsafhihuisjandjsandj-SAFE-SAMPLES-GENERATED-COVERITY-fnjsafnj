//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 50
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_UPPER(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - 'a' + 'A') : (c))
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) - 'A' + 'a') : (c))

typedef struct {
    char word[MAX_WORD_LEN];
    int length;
} WordInfo;

int split_into_words(const char *input, WordInfo words[], int max_words) {
    if (input == NULL || words == NULL || max_words <= 0) {
        return -1;
    }
    
    int word_count = 0;
    int i = 0;
    
    while (input[i] != '\0' && word_count < max_words) {
        while (IS_WHITESPACE(input[i]) && input[i] != '\0') {
            i++;
        }
        
        if (input[i] == '\0') {
            break;
        }
        
        int j = 0;
        while (!IS_WHITESPACE(input[i]) && input[i] != '\0' && j < MAX_WORD_LEN - 1) {
            words[word_count].word[j] = input[i];
            j++;
            i++;
        }
        words[word_count].word[j] = '\0';
        words[word_count].length = j;
        word_count++;
    }
    
    return word_count;
}

void process_words(WordInfo words[], int word_count) {
    if (words == NULL || word_count <= 0) {
        return;
    }
    
    for (int i = 0; i < word_count; i++) {
        if (words[i].length > 0) {
            for (int j = 0; j < words[i].length; j++) {
                if (j == 0) {
                    words[i].word[j] = TO_UPPER(words[i].word[j]);
                } else {
                    words[i].word[j] = TO_LOWER(words[i].word[j]);
                }
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordInfo words[MAX_WORDS];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int word_count = split_into_words(input, words, MAX_WORDS);
    if (word_count < 0) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    process_words(words, word_count);
    
    printf("Processed words:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i].word);
    }
    printf("\n");
    
    printf("Word statistics:\n");
    printf("Total words: %d\n", word_count);
    for (int i = 0; i < word_count; i++) {
        printf("Word %d: '%s' (length: %d)\n", i + 1, words[i].word, words[i].length);
    }
    
    return 0;
}