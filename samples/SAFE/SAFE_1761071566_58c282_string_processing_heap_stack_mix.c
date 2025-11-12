//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

struct WordInfo {
    char *word;
    int count;
};

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

int find_word_index(struct WordInfo *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int compare_word_info(const void *a, const void *b) {
    const struct WordInfo *wa = (const struct WordInfo *)a;
    const struct WordInfo *wb = (const struct WordInfo *)b;
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text: ");
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
        return 0;
    }
    
    struct WordInfo words_stack[MAX_WORDS];
    int word_count = 0;
    
    int start = -1;
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && is_valid_char(input[i])) {
            if (start == -1) {
                start = i;
            }
        } else {
            if (start != -1) {
                char *word = sanitize_word(input, start, i);
                if (word) {
                    int index = find_word_index(words_stack, word_count, word);
                    if (index >= 0) {
                        words_stack[index].count++;
                        free(word);
                    } else if (word_count < MAX_WORDS) {
                        words_stack[word_count].word = word;
                        words_stack[word_count].count = 1;
                        word_count++;
                    } else {
                        free(word);
                    }
                }
                start = -1;
            }
        }
    }
    
    struct WordInfo *words_heap = malloc(word_count * sizeof(struct WordInfo));
    if (!words_heap) {
        for (int i = 0; i < word_count; i++) {
            free(words_stack[i].word);
        }
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    for (int i = 0; i < word_count; i++) {
        words_heap[i] = words_stack[i];
    }
    
    qsort(words_heap, word_count, sizeof(struct WordInfo), compare_word_info);
    
    printf("Word frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words_heap[i].word, words_heap[i].count);
    }
    
    for (int i = 0; i < word_count; i++) {
        free(words_heap[i].word);
    }
    free(words_heap);
    
    return 0;
}