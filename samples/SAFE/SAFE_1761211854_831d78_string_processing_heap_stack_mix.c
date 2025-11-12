//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

typedef struct {
    char *word;
    int count;
} WordCount;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

char *sanitize_word(const char *input, int start, int end) {
    if (start >= end || end - start >= 256) {
        return NULL;
    }
    
    char *word = malloc(end - start + 1);
    if (!word) {
        return NULL;
    }
    
    int j = 0;
    for (int i = start; i < end; i++) {
        if (is_valid_char(input[i])) {
            word[j++] = tolower((unsigned char)input[i]);
        }
    }
    word[j] = '\0';
    
    if (j == 0) {
        free(word);
        return NULL;
    }
    
    return word;
}

int find_word_index(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int compare_word_counts(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    return wb->count - wa->count;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text for word frequency analysis: ");
    if (!fgets(input, sizeof(input), stdin)) {
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
    
    WordCount words[MAX_WORDS];
    int word_count = 0;
    int in_word = 0;
    int word_start = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        int is_word_char = (i < input_len) && is_valid_char(c);
        
        if (!in_word && is_word_char) {
            in_word = 1;
            word_start = i;
        } else if (in_word && (!is_word_char || i == input_len)) {
            char *word = sanitize_word(input, word_start, i);
            if (word) {
                int index = find_word_index(words, word_count, word);
                if (index >= 0) {
                    words[index].count++;
                    free(word);
                } else if (word_count < MAX_WORDS) {
                    words[word_count].word = word;
                    words[word_count].count = 1;
                    word_count++;
                } else {
                    free(word);
                }
            }
            in_word = 0;
        }
    }
    
    if (word_count > 0) {
        qsort(words, word_count, sizeof(WordCount), compare_word_counts);
        
        printf("\nWord frequencies (top %d):\n", word_count);
        for (int i = 0; i < word_count; i++) {
            printf("%-20s: %d\n", words[i].word, words[i].count);
        }
        
        for (int i = 0; i < word_count; i++) {
            free(words[i].word);
        }
    } else {
        printf("No valid words found in input\n");
    }
    
    return 0;
}