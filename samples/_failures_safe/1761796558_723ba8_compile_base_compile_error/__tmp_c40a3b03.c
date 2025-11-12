//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct TextStats {
    size_t char_count;
    size_t word_count;
    size_t line_count;
    size_t sentence_count;
    size_t longest_word_length;
};

struct WordInfo {
    char word[64];
    size_t length;
    size_t frequency;
};

struct TextProcessor {
    struct TextStats stats;
    struct WordInfo *word_list;
    size_t word_list_size;
    size_t word_list_capacity;
};

void init_text_processor(struct TextProcessor *tp) {
    tp->stats.char_count = 0;
    tp->stats.word_count = 0;
    tp->stats.line_count = 0;
    tp->stats.sentence_count = 0;
    tp->stats.longest_word_length = 0;
    tp->word_list = NULL;
    tp->word_list_size = 0;
    tp->word_list_capacity = 0;
}

void cleanup_text_processor(struct TextProcessor *tp) {
    if (tp->word_list != NULL) {
        free(tp->word_list);
        tp->word_list = NULL;
    }
    tp->word_list_size = 0;
    tp->word_list_capacity = 0;
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

int add_word_to_list(struct TextProcessor *tp, const char *word, size_t length) {
    if (length >= sizeof(tp->word_list[0].word)) {
        return 0;
    }
    
    for (size_t i = 0; i < tp->word_list_size; i++) {
        if (strncmp(tp->word_list[i].word, word, length) == 0 && 
            tp->word_list[i].word[length] == '\0') {
            tp->word_list[i].frequency++;
            return 1;
        }
    }
    
    if (tp->word_list_size >= tp->word_list_capacity) {
        size_t new_capacity = tp->word_list_capacity == 0 ? 16 : tp->word_list_capacity * 2;
        struct WordInfo *new_list = realloc(tp->word_list, new_capacity * sizeof(struct WordInfo));
        if (new_list == NULL) {
            return 0;
        }
        tp->word_list = new_list;
        tp->word_list_capacity = new_capacity;
    }
    
    strncpy(tp->word_list[tp->word_list_size].word, word, length);
    tp->word_list[tp->word_list_size].word[length] = '\0';
    tp->word_list[tp->word_list_size].length = length;
    tp->word_list[tp->word_list_size].frequency = 1;
    tp->word_list_size++;
    
    return 1;
}

void process_text(struct TextProcessor *tp, const char *text) {
    if (text == NULL) {
        return;
    }
    
    size_t len = strlen(text);
    if (len == 0) {
        return;
    }
    
    int in_word = 0;
    size_t word_start = 0;
    size_t current_word_length = 0;
    
    for (size_t i = 0; i < len; i++) {
        char c = text[i];
        tp->stats.char_count++;
        
        if (c == '\n') {
            tp->stats.line_count++;
        }
        
        if (c == '.' || c == '!' || c == '?') {
            tp->stats.sentence_count++;
        }
        
        if (is_word_char(c)) {
            if (!in_word) {
                in_word = 1;
                word_start = i;
                current_word_length = 0;
            }
            current_word_length++;
        } else {
            if (in_word) {
                if (current_word_length > 0 && current_word_length < sizeof(tp->word_list[0].word)) {
                    char temp_word[sizeof(tp->word_list[0].word)];
                    strncpy(temp_word, &text[word_start], current_word_length);
                    temp_word[current_word_length] = '\0';
                    
                    if (add_word_to_list(tp, temp_word, current_word_length)) {
                        tp->stats.word_count++;
                        if (current_word_length > tp->stats.longest_word_length) {
                            tp->stats.longest_word_length = current_word_length;
                        }
                    }
                }
                in_word = 0;
                current_word_length = 0;
            }
        }
    }
    
    if (in_word && current_word_length > 0 && current_word_length < sizeof(tp->word_list[0].word)) {
        char temp_word[sizeof(tp->word_list[0].word)];
        strncpy(temp_word, &text[word_start], current_word_length);
        temp_word[current_word_length] = '\0';
        
        if (add_word_to_list(tp, temp_word, current_word_length)) {
            tp->stats.word_count++;
            if (current_word_length > tp->stats.longest_word_length) {
                tp->stats.longest_word_length = current_word_length;
            }
        }
    }
    
    if (len > 0 && text[len - 1] != '\n') {
        tp->stats.line_count++;
    }
}

void print_stats(const struct TextProcessor *tp) {
    printf("Text Statistics:\n");
    printf("Characters: %zu\n", tp->stats.char_count);
    printf("Words