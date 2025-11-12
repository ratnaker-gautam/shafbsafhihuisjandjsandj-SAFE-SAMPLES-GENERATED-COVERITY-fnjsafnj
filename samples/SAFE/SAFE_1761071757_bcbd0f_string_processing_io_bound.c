//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

struct WordCount {
    char word[MAX_WORD_LEN];
    int count;
};

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

int find_word(struct WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int compare_word_count(const void *a, const void *b) {
    const struct WordCount *wa = (const struct WordCount *)a;
    const struct WordCount *wb = (const struct WordCount *)b;
    if (wa->count != wb->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
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
    
    if (len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int pos = 0;
    while (pos < len && word_count < MAX_WORDS) {
        while (pos < len && !is_word_char(input[pos])) {
            pos++;
        }
        
        if (pos >= len) {
            break;
        }
        
        int start = pos;
        while (pos < len && is_word_char(input[pos]) && (pos - start) < MAX_WORD_LEN - 1) {
            pos++;
        }
        
        char word[MAX_WORD_LEN];
        int word_len = pos - start;
        if (word_len >= MAX_WORD_LEN) {
            word_len = MAX_WORD_LEN - 1;
        }
        
        memcpy(word, &input[start], word_len);
        word[word_len] = '\0';
        to_lowercase(word);
        
        if (word_len > 0) {
            int found_idx = find_word(words, word_count, word);
            if (found_idx >= 0) {
                words[found_idx].count++;
            } else {
                if (word_count < MAX_WORDS) {
                    strncpy(words[word_count].word, word, MAX_WORD_LEN - 1);
                    words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                    words[word_count].count = 1;
                    word_count++;
                }
            }
        }
    }
    
    qsort(words, word_count, sizeof(struct WordCount), compare_word_count);
    
    printf("\nWord frequency analysis:\n");
    printf("------------------------\n");
    for (int i = 0; i < word_count; i++) {
        printf("%-20s: %d\n", words[i].word, words[i].count);
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
    }
    
    return 0;
}