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

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int split_into_words(const char *input, char words[][MAX_WORD_LEN]) {
    int word_count = 0;
    int pos = 0;
    int in_word = 0;
    
    for (int i = 0; input[i] != '\0'; i++) {
        if (IS_WHITESPACE(input[i])) {
            if (in_word) {
                words[word_count][pos] = '\0';
                word_count++;
                pos = 0;
                in_word = 0;
            }
        } else {
            if (pos < MAX_WORD_LEN - 1) {
                words[word_count][pos] = input[i];
                pos++;
                in_word = 1;
            }
        }
        
        if (word_count >= MAX_WORDS) {
            break;
        }
    }
    
    if (in_word && word_count < MAX_WORDS) {
        words[word_count][pos] = '\0';
        word_count++;
    }
    
    return word_count;
}

void normalize_word(char *word) {
    int read_pos = 0;
    int write_pos = 0;
    
    while (word[read_pos] != '\0') {
        if (isalpha((unsigned char)word[read_pos])) {
            word[write_pos] = TO_UPPER(word[read_pos]);
            write_pos++;
        }
        read_pos++;
    }
    word[write_pos] = '\0';
}

int find_word_index(WordCount *word_counts, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(word_counts[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN];
    WordCount word_counts[MAX_WORDS];
    int unique_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
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
    
    int word_count = split_into_words(input, words);
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    for (int i = 0; i < word_count; i++) {
        normalize_word(words[i]);
        
        if (strlen(words[i]) == 0) {
            continue;
        }
        
        int index = find_word_index(word_counts, unique_count, words[i]);
        if (index >= 0) {
            word_counts[index].count++;
        } else if (unique_count < MAX_WORDS) {
            strncpy(word_counts[unique_count].word, words[i], MAX_WORD_LEN - 1);
            word_counts[unique_count].word[MAX_WORD_LEN - 1] = '\0';
            word_counts[unique_count].count = 1;
            unique_count++;
        }
    }
    
    printf("Word frequency analysis:\n");
    for (int i = 0; i < unique_count; i++) {
        printf("%s: %d\n", word_counts[i].word, word_counts[i].count);
    }
    
    return 0;
}