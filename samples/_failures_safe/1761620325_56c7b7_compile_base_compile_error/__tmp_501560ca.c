//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 100
#define MAX_TEXT_LEN 1024

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int first_position;
    int last_position;
};

struct TextProcessor {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_words;
    char processed_text[MAX_TEXT_LEN];
};

void init_text_processor(struct TextProcessor *tp) {
    tp->word_count = 0;
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
    return isalnum((unsigned char)c) || c == ' ' || c == '\t' || c == '\n';
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
    if (tp->word_count >= MAX_WORDS) return 0;
    if (!safe_strncpy(tp->words[tp->word_count].word, word, MAX_WORD_LEN)) return 0;
    tp->words[tp->word_count].count = 1;
    tp->words[tp->word_count].first_position = position;
    tp->words[tp->word_count].last_position = position;
    tp->word_count++;
    return 1;
}

void update_word(struct TextProcessor *tp, int index, int position) {
    tp->words[index].count++;
    tp->words[index].last_position = position;
}

int process_text(struct TextProcessor *tp, const char *text) {
    if (text == NULL) return 0;
    
    char buffer[MAX_TEXT_LEN];
    if (!safe_strncpy(buffer, text, sizeof(buffer))) return 0;
    
    char *ptr = buffer;
    int position = 0;
    int in_word = 0;
    char current_word[MAX_WORD_LEN];
    int word_len = 0;
    
    while (*ptr != '\0' && position < MAX_TEXT_LEN - 1) {
        if (!is_valid_char(*ptr)) {
            ptr++;
            continue;
        }
        
        if (isalnum((unsigned char)*ptr)) {
            if (!in_word) {
                in_word = 1;
                word_len = 0;
                memset(current_word, 0, sizeof(current_word));
            }
            if (word_len < MAX_WORD_LEN - 1) {
                current_word[word_len++] = tolower((unsigned char)*ptr);
            }
        } else {
            if (in_word && word_len > 0) {
                current_word[word_len] = '\0';
                int index = find_word_index(tp, current_word);
                if (index == -1) {
                    if (!add_word(tp, current_word, tp->total_words)) {
                        return 0;
                    }
                } else {
                    update_word(tp, index, tp->total_words);
                }
                tp->total_words++;
                in_word = 0;
            }
        }
        ptr++;
    }
    
    if (in_word && word_len > 0) {
        current_word[word_len] = '\0';
        int index = find_word_index(tp, current_word);
        if (index == -1) {
            if (!add_word(tp, current_word, tp->total_words)) {
                return 0;
            }
        } else {
            update_word(tp, index, tp->total_words);
        }
        tp->total_words++;
    }
    
    return 1;
}

void print_statistics(struct TextProcessor *tp) {
    printf("Text Analysis Results:\n");
    printf("Total unique words: %d\n", tp->word_count);
    printf("Total word occurrences: %d\n", tp->total_words);
    printf("\nWord Statistics:\n");
    
    for (int i = 0; i < tp->word_count; i++) {
        printf("Word: %-15s Count: %-3d First: %-3d Last: %-3d\n",
               tp->words[i].word,
               tp->words[i].count,
               tp->words[i].first_position,
               tp->words[i].last_position);
    }
}

int main() {
    struct TextProcessor processor;
    init_text_processor(&processor);
    
    char input_text[MAX_TEXT