//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512
#define MAX_WORD_LEN 64

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

int find_word(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void to_lower_case(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    if (wa->count != wb->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len == 0 || input_len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Invalid input length\n");
        return 1;
    }
    
    int i = 0;
    while (input[i] != '\0' && word_count < MAX_WORDS) {
        while (input[i] != '\0' && !is_valid_char(input[i])) {
            i++;
        }
        
        if (input[i] == '\0') {
            break;
        }
        
        int start = i;
        while (input[i] != '\0' && is_valid_char(input[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len >= MAX_WORD_LEN) {
            word_len = MAX_WORD_LEN - 1;
        }
        
        char current_word[MAX_WORD_LEN];
        strncpy(current_word, &input[start], word_len);
        current_word[word_len] = '\0';
        to_lower_case(current_word);
        
        if (strlen(current_word) > 0) {
            int found_index = find_word(words, word_count, current_word);
            if (found_index >= 0) {
                words[found_index].count++;
            } else {
                if (word_count < MAX_WORDS) {
                    strncpy(words[word_count].word, current_word, MAX_WORD_LEN - 1);
                    words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                    words[word_count].count = 1;
                    word_count++;
                }
            }
        }
    }
    
    qsort(words, word_count, sizeof(WordCount), compare_word_count);
    
    for (int j = 0; j < word_count; j++) {
        if (printf("%s: %d\n", words[j].word, words[j].count) < 0) {
            fprintf(stderr, "Error writing output\n");
            return 1;
        }
    }
    
    return 0;
}