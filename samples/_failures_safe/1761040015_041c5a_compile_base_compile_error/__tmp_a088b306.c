//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: string_processing
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
    char processed_text[MAX_TEXT_LEN];
    int total_words;
    int total_chars;
};

void init_text_processor(struct TextProcessor *tp) {
    tp->word_count = 0;
    tp->total_words = 0;
    tp->total_chars = 0;
    tp->processed_text[0] = '\0';
    for (int i = 0; i < MAX_WORDS; i++) {
        tp->words[i].word[0] = '\0';
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
    if (tp->word_count >= MAX_WORDS) return 0;
    if (safe_strncpy(tp->words[tp->word_count].word, word, MAX_WORD_LEN) == 0) return 0;
    tp->words[tp->word_count].count = 1;
    tp->words[tp->word_count].first_position = position;
    tp->words[tp->word_count].last_position = position;
    tp->word_count++;
    return 1;
}

void process_text(struct TextProcessor *tp, const char *input) {
    if (input == NULL) return;
    
    char buffer[MAX_TEXT_LEN];
    if (safe_strncpy(buffer, input, MAX_TEXT_LEN) == 0) return;
    
    char processed[MAX_TEXT_LEN] = {0};
    int proc_index = 0;
    int word_start = -1;
    int word_count = 0;
    
    for (int i = 0; buffer[i] != '\0' && proc_index < MAX_TEXT_LEN - 1; i++) {
        if (!is_valid_char(buffer[i])) continue;
        
        if (isalnum((unsigned char)buffer[i])) {
            if (word_start == -1) word_start = i;
            processed[proc_index++] = tolower((unsigned char)buffer[i]);
        } else {
            if (word_start != -1) {
                processed[proc_index++] = ' ';
                word_count++;
                
                char current_word[MAX_WORD_LEN];
                int word_len = i - word_start;
                if (word_len >= MAX_WORD_LEN) word_len = MAX_WORD_LEN - 1;
                
                safe_strncpy(current_word, &buffer[word_start], word_len + 1);
                current_word[word_len] = '\0';
                
                for (int j = 0; current_word[j] != '\0'; j++) {
                    current_word[j] = tolower((unsigned char)current_word[j]);
                }
                
                int word_index = find_word_index(tp, current_word);
                if (word_index == -1) {
                    add_word(tp, current_word, word_count);
                } else {
                    tp->words[word_index].count++;
                    tp->words[word_index].last_position = word_count;
                }
                
                word_start = -1;
            }
            processed[proc_index++] = buffer[i];
        }
    }
    
    if (word_start != -1) {
        word_count++;
        char current_word[MAX_WORD_LEN];
        int word_len = strlen(buffer) - word_start;
        if (word_len >= MAX_WORD_LEN) word_len = MAX_WORD_LEN - 1;
        
        safe_strncpy(current_word, &buffer[word_start], word_len + 1);
        current_word[word_len] = '\0';
        
        for (int j = 0; current_word[j] != '\0'; j++) {
            current_word[j] = tolower((unsigned char)current_word[j]);
        }
        
        int word_index = find_word_index(tp, current_word);
        if (word_index == -1) {
            add_word(tp, current_word, word_count);
        } else {
            tp->words[word_index].count++;
            tp->words[word_index].last_position = word_count;
        }
    }
    
    processed[