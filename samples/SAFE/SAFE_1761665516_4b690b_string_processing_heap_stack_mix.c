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

char *extract_next_word(const char **text) {
    if (text == NULL || *text == NULL) return NULL;
    
    const char *start = *text;
    while (*start && !is_word_char(*start)) {
        start++;
    }
    
    if (*start == '\0') {
        *text = start;
        return NULL;
    }
    
    const char *end = start;
    while (*end && is_word_char(*end)) {
        end++;
    }
    
    size_t word_len = end - start;
    if (word_len == 0) return NULL;
    
    char *word = malloc(word_len + 1);
    if (word == NULL) return NULL;
    
    memcpy(word, start, word_len);
    word[word_len] = '\0';
    
    *text = end;
    return word;
}

int find_word_index(struct WordCount *words, int count, const char *word) {
    if (words == NULL || word == NULL) return -1;
    
    for (int i = 0; i < count; i++) {
        if (words[i].word != NULL && strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int compare_word_counts(const void *a, const void *b) {
    const struct WordCount *wa = (const struct WordCount *)a;
    const struct WordCount *wb = (const struct WordCount *)b;
    
    if (wa->count != wb->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text for word frequency analysis: ");
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
    
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    const char *text_ptr = input;
    char *word;
    
    while ((word = extract_next_word(&text_ptr)) != NULL && word_count < MAX_WORDS) {
        char *lower_word = to_lower_case(word);
        free(word);
        
        if (lower_word == NULL) continue;
        
        int existing_index = find_word_index(words, word_count, lower_word);
        
        if (existing_index >= 0) {
            words[existing_index].count++;
            free(lower_word);
        } else {
            words[word_count].word = lower_word;
            words[word_count].count = 1;
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    qsort(words, word_count, sizeof(struct WordCount), compare_word_counts);
    
    printf("\nWord frequencies (sorted by count):\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    for (int i = 0; i < word_count; i++) {
        free(words[i].word);
    }
    
    return 0;
}