//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
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

char *to_lower_case(const char *str) {
    if (str == NULL) return NULL;
    
    size_t len = strlen(str);
    if (len == 0) return NULL;
    
    char *result = malloc(len + 1);
    if (result == NULL) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = tolower((unsigned char)str[i]);
    }
    result[len] = '\0';
    return result;
}

int is_word_char(char c) {
    return isalpha((unsigned char)c);
}

char *extract_next_word(const char **text_ptr) {
    if (text_ptr == NULL || *text_ptr == NULL) return NULL;
    
    const char *text = *text_ptr;
    
    while (*text && !is_word_char(*text)) {
        text++;
    }
    
    if (*text == '\0') {
        *text_ptr = text;
        return NULL;
    }
    
    const char *start = text;
    while (*text && is_word_char(*text)) {
        text++;
    }
    
    size_t word_len = text - start;
    if (word_len == 0) return NULL;
    
    char *word = malloc(word_len + 1);
    if (word == NULL) return NULL;
    
    memcpy(word, start, word_len);
    word[word_len] = '\0';
    
    *text_ptr = text;
    return word;
}

int find_word_index(struct WordInfo *words, int count, const char *word) {
    if (words == NULL || word == NULL) return -1;
    
    for (int i = 0; i < count; i++) {
        if (words[i].word != NULL && strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void cleanup_words(struct WordInfo *words, int count) {
    if (words == NULL) return;
    
    for (int i = 0; i < count; i++) {
        if (words[i].word != NULL) {
            free(words[i].word);
        }
    }
}

int compare_word_info(const void *a, const void *b) {
    const struct WordInfo *wa = (const struct WordInfo *)a;
    const struct WordInfo *wb = (const struct WordInfo *)b;
    
    if (wa->count != wb->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    struct WordInfo words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    for (int i = 0; i < MAX_WORDS; i++) {
        words[i].word = NULL;
        words[i].count = 0;
    }
    
    const char *text_ptr = input;
    
    while (word_count < MAX_WORDS) {
        char *raw_word = extract_next_word(&text_ptr);
        if (raw_word == NULL) break;
        
        char *word = to_lower_case(raw_word);
        free(raw_word);
        
        if (word == NULL) continue;
        
        int existing_index = find_word_index(words, word_count, word);
        
        if (existing_index >= 0) {
            words[existing_index].count++;
            free(word);
        } else {
            words[word_count].word = word;
            words[word_count].count = 1;
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    qsort(words, word_count, sizeof(struct WordInfo), compare_word_info);
    
    printf("\nWord frequency analysis:\n");
    printf("------------------------\n");
    
    for (int i = 0; i < word_count; i++) {
        if (words[i].word != NULL) {
            printf("%-15s: %d\n", words[i].word, words[i].count);
        }
    }
    
    cleanup_words(words, word_count);
    return 0;
}