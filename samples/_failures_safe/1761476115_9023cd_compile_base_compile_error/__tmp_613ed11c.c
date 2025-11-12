//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

int is_valid_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = safe_strlen(input, MAX_INPUT_LEN + 1);
    if (len > MAX_INPUT_LEN) return 0;
    return 1;
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
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void process_sentence(const char *sentence, WordCount *words, int *word_count) {
    char buffer[MAX_INPUT_LEN];
    char temp_word[MAX_WORD_LEN];
    size_t sentence_len = safe_strlen(sentence, MAX_INPUT_LEN);
    
    if (sentence_len >= sizeof(buffer)) return;
    strncpy(buffer, sentence, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    to_lower_case(buffer);
    
    size_t pos = 0;
    while (pos < sentence_len && buffer[pos] != '\0') {
        while (pos < sentence_len && IS_WHITESPACE(buffer[pos])) {
            pos++;
        }
        
        if (pos >= sentence_len || buffer[pos] == '\0') break;
        
        size_t word_start = pos;
        while (pos < sentence_len && !IS_WHITESPACE(buffer[pos]) && !IS_TERMINATOR(buffer[pos]) && buffer[pos] != '\0') {
            pos++;
        }
        
        size_t word_len = pos - word_start;
        if (word_len > 0 && word_len < MAX_WORD_LEN - 1) {
            strncpy(temp_word, &buffer[word_start], word_len);
            temp_word[word_len] = '\0';
            
            int existing_index = find_word(words, *word_count, temp_word);
            if (existing_index != -1) {
                words[existing_index].count++;
            } else if (*word_count < MAX_WORDS) {
                strncpy(words[*word_count].word, temp_word, MAX_WORD_LEN - 1);
                words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                words[*word_count].count = 1;
                (*word_count)++;
            }
        }
        
        if (pos < sentence_len && IS_TERMINATOR(buffer[pos])) {
            pos++;
            break;
        }
    }
}

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    if (wb->count > wa->count) return 1;
    if (wb->count < wa->count) return -1;
    return 0;
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t input_len = safe_strlen(input, sizeof(input));
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        printf("Invalid input.\n");
        return 1;
    }
    
    if (input[0] == '\0') {
        printf("No input provided.\n");
        return 1;
    }
    
    char *sentence_start = input;
    char *sentence_end;
    
    while (*sentence_start != '\0') {
        sentence_end = sentence_start;
        while (*sentence_end != '\0' && !IS_TERMINATOR(*sentence_end)) {
            sentence_end++;
        }
        
        if (*sentence_end != '\0') {
            sentence_end++;
        }
        
        size_t sentence_len = sentence_end - sentence_start;
        if (sentence_len > 0) {
            char sentence[MAX_INPUT_LEN];
            if (sentence_len < sizeof(sentence)) {
                strncpy(sentence, sentence_start, sentence_len);
                sentence[sentence_len] = '\0