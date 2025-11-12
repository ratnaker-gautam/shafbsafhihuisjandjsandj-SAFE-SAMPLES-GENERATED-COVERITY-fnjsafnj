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
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
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

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    return wb->count - wa->count;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    WordCount words[MAX_WORDS];
    int word_count = 0;
    int in_word = 0;
    int word_start = 0;
    
    for (int i = 0; i <= len && word_count < MAX_WORDS; i++) {
        if (i < len && is_valid_char(input[i])) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                char *word = sanitize_word(input, word_start, i);
                if (word) {
                    int index = find_word_index(words, word_count, word);
                    if (index >= 0) {
                        words[index].count++;
                        free(word);
                    } else {
                        words[word_count].word = word;
                        words[word_count].count = 1;
                        word_count++;
                    }
                }
                in_word = 0;
            }
        }
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 0;
    }
    
    qsort(words, word_count, sizeof(WordCount), compare_word_count);
    
    printf("\nWord frequency (top %d):\n", word_count > 10 ? 10 : word_count);
    int display_count = word_count > 10 ? 10 : word_count;
    for (int i = 0; i < display_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    for (int i = 0; i < word_count; i++) {
        free(words[i].word);
    }
    
    return 0;
}