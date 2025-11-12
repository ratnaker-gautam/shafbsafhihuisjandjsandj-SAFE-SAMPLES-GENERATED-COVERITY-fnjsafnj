//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 1000
#define MAX_TEXT_LEN 10000

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int first_position;
    int last_position;
};

struct TextProcessor {
    struct WordStats words[MAX_WORDS];
    int word_count;
    char processed_text[MAX_TEXT_LEN];
    int text_length;
    int total_words;
};

void init_text_processor(struct TextProcessor *tp) {
    tp->word_count = 0;
    tp->text_length = 0;
    tp->total_words = 0;
    memset(tp->processed_text, 0, sizeof(tp->processed_text));
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(tp->words[i].word, 0, sizeof(tp->words[i].word));
        tp->words[i].count = 0;
        tp->words[i].first_position = -1;
        tp->words[i].last_position = -1;
    }
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\t' || c == '\n' || c == '.' || c == ',' || c == '!' || c == '?';
}

int safe_strncpy(char *dest, const char *src, size_t n) {
    if (dest == NULL || src == NULL || n == 0) return 0;
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return 1;
}

int find_word_index(struct TextProcessor *tp, const char *word) {
    for (int i = 0; i < tp->word_count; i++) {
        if (strcmp(tp->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int add_word(struct TextProcessor *tp, const char *word, int position) {
    if (tp->word_count >= MAX_WORDS) return -1;
    if (strlen(word) >= MAX_WORD_LEN) return -1;
    
    int index = find_word_index(tp, word);
    if (index == -1) {
        index = tp->word_count;
        if (!safe_strncpy(tp->words[index].word, word, MAX_WORD_LEN)) return -1;
        tp->words[index].count = 1;
        tp->words[index].first_position = position;
        tp->words[index].last_position = position;
        tp->word_count++;
    } else {
        tp->words[index].count++;
        tp->words[index].last_position = position;
    }
    return index;
}

int process_text(struct TextProcessor *tp, const char *input) {
    if (input == NULL) return 0;
    
    size_t input_len = strlen(input);
    if (input_len >= MAX_TEXT_LEN) return 0;
    
    char buffer[MAX_TEXT_LEN];
    if (!safe_strncpy(buffer, input, sizeof(buffer))) return 0;
    
    char *ptr = buffer;
    int word_position = 0;
    int in_word = 0;
    char current_word[MAX_WORD_LEN];
    int word_len = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = buffer[i];
        
        if (!is_valid_char(c) && c != '\0') {
            return 0;
        }
        
        if (isalnum((unsigned char)c)) {
            if (!in_word) {
                in_word = 1;
                word_len = 0;
            }
            if (word_len < MAX_WORD_LEN - 1) {
                current_word[word_len++] = tolower((unsigned char)c);
            }
        } else {
            if (in_word) {
                current_word[word_len] = '\0';
                if (word_len > 0) {
                    if (add_word(tp, current_word, word_position) == -1) return 0;
                    word_position++;
                    tp->total_words++;
                }
                in_word = 0;
                word_len = 0;
            }
        }
        
        if (tp->text_length < MAX_TEXT_LEN - 1) {
            tp->processed_text[tp->text_length++] = c;
        }
    }
    
    tp->processed_text[tp->text_length] = '\0';
    return 1;
}

void print_word_stats(struct TextProcessor *tp) {
    printf("Text Analysis Results:\n");
    printf("Total words processed: %d\n", tp->total_words);
    printf("Unique words: %d\n", tp->word_count);
    printf("\nWord Statistics:\n");
    
    for (int i = 0; i < tp->word_count; i++) {
        printf("Word: %-15s Count: %3d First: %3d Last: %3d\n",
               tp->words[i].word,
               tp->words[i].count,
               tp->words[i].first_position,
               tp->words[i].last_position);
    }
}

int main(void) {
    struct TextProcessor processor;
    init_text_processor(&processor);
    
    char input_text[MAX_TEXT_LEN];
    printf