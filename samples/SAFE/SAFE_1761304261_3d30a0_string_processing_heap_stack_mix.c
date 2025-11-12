//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

struct WordCount {
    char *word;
    int count;
};

char *strdup_safe(const char *s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s);
    if (len >= MAX_INPUT_LEN) return NULL;
    char *dup = malloc(len + 1);
    if (dup == NULL) return NULL;
    strcpy(dup, s);
    return dup;
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

int process_input(const char *input, struct WordCount **words_ptr, int *word_count) {
    if (input == NULL || words_ptr == NULL || word_count == NULL) return 0;
    
    struct WordCount *words = *words_ptr;
    int count = *word_count;
    
    const char *p = input;
    while (*p != '\0') {
        while (*p != '\0' && !is_word_char(*p)) p++;
        if (*p == '\0') break;
        
        const char *start = p;
        while (*p != '\0' && is_word_char(*p)) p++;
        
        size_t word_len = p - start;
        if (word_len == 0 || word_len >= MAX_INPUT_LEN) continue;
        
        char word_buf[MAX_INPUT_LEN];
        if (word_len >= sizeof(word_buf)) continue;
        strncpy(word_buf, start, word_len);
        word_buf[word_len] = '\0';
        
        for (char *c = word_buf; *c; c++) {
            *c = tolower((unsigned char)*c);
        }
        
        int found = 0;
        for (int i = 0; i < count; i++) {
            if (strcmp(words[i].word, word_buf) == 0) {
                words[i].count++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            if (count >= MAX_WORDS) break;
            
            char *new_word = strdup_safe(word_buf);
            if (new_word == NULL) return 0;
            
            struct WordCount *new_words = realloc(words, (count + 1) * sizeof(struct WordCount));
            if (new_words == NULL) {
                free(new_word);
                return 0;
            }
            
            words = new_words;
            words[count].word = new_word;
            words[count].count = 1;
            count++;
        }
    }
    
    *words_ptr = words;
    *word_count = count;
    return 1;
}

void cleanup_words(struct WordCount *words, int count) {
    if (words == NULL) return;
    for (int i = 0; i < count; i++) {
        free(words[i].word);
    }
    free(words);
}

int compare_word_count(const void *a, const void *b) {
    const struct WordCount *wa = (const struct WordCount *)a;
    const struct WordCount *wb = (const struct WordCount *)b;
    if (wb->count != wa->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN];
    struct WordCount *words = NULL;
    int word_count = 0;
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
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
    
    if (!process_input(input, &words, &word_count)) {
        printf("Error processing input\n");
        cleanup_words(words, word_count);
        return 1;
    }
    
    if (word_count > 0) {
        qsort(words, word_count, sizeof(struct WordCount), compare_word_count);
        
        printf("Word frequencies:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%s: %d\n", words[i].word, words[i].count);
        }
    } else {
        printf("No words found\n");
    }
    
    cleanup_words(words, word_count);
    return 0;
}