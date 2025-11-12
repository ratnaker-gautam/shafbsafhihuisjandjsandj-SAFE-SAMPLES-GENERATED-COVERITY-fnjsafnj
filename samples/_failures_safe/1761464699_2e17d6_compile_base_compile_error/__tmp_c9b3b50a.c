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
    int count;
} WordCount;

int split_into_words(const char *input, char words[][MAX_WORD_LEN], int max_words) {
    int word_count = 0;
    int pos = 0;
    int in_word = 0;
    int word_start = 0;
    
    if (input == NULL) {
        return 0;
    }
    
    while (input[pos] != '\0' && word_count < max_words) {
        if (IS_WHITESPACE(input[pos])) {
            if (in_word) {
                int word_len = pos - word_start;
                if (word_len > 0 && word_len < MAX_WORD_LEN) {
                    strncpy(words[word_count], &input[word_start], word_len);
                    words[word_count][word_len] = '\0';
                    word_count++;
                }
                in_word = 0;
            }
        } else {
            if (!in_word) {
                word_start = pos;
                in_word = 1;
            }
        }
        pos++;
    }
    
    if (in_word && word_count < max_words) {
        int word_len = pos - word_start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            strncpy(words[word_count], &input[word_start], word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    return word_count;
}

void normalize_word(char *word) {
    int i = 0;
    int j = 0;
    char normalized[MAX_WORD_LEN];
    
    if (word == NULL) {
        return;
    }
    
    while (word[i] != '\0' && j < MAX_WORD_LEN - 1) {
        if (isalpha((unsigned char)word[i])) {
            normalized[j] = TO_LOWER(word[i]);
            j++;
        }
        i++;
    }
    normalized[j] = '\0';
    
    if (j > 0) {
        strncpy(word, normalized, MAX_WORD_LEN);
        word[MAX_WORD_LEN - 1] = '\0';
    } else {
        word[0] = '\0';
    }
}

int count_unique_words(char words[][MAX_WORD_LEN], int word_count, WordCount *unique_words) {
    int unique_count = 0;
    
    if (words == NULL || unique_words == NULL) {
        return 0;
    }
    
    for (int i = 0; i < word_count; i++) {
        char normalized[MAX_WORD_LEN];
        strncpy(normalized, words[i], MAX_WORD_LEN);
        normalized[MAX_WORD_LEN - 1] = '\0';
        normalize_word(normalized);
        
        if (normalized[0] == '\0') {
            continue;
        }
        
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(normalized, unique_words[j].word) == 0) {
                unique_words[j].count++;
                found = 1;
                break;
            }
        }
        
        if (!found && unique_count < MAX_WORDS) {
            strncpy(unique_words[unique_count].word, normalized, MAX_WORD_LEN);
            unique_words[unique_count].word[MAX_WORD_LEN - 1] = '\0';
            unique_words[unique_count].count = 1;
            unique_count++;
        }
    }
    
    return unique_count;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    WordCount unique_words[MAX_WORDS];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int word_count = split_into_words(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    int unique_count = count_unique_words(words, word_count, unique_words);
    
    printf("\nWord frequency analysis:\n");
    printf("Total words: %d\n", word_count);
    printf("Unique words: %d\n\n", unique_count);