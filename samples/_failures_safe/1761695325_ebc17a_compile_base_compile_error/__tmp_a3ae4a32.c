//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 100
#define MAX_WORD_LEN 50

struct WordInfo {
    char word[MAX_WORD_LEN];
    int count;
    int length;
    int is_capitalized;
};

struct TextStats {
    struct WordInfo words[MAX_WORDS];
    int word_count;
    int total_chars;
    int capitalized_count;
    int longest_word_length;
};

struct TextProcessor {
    struct TextStats stats;
    char input_buffer[1024];
};

void init_text_processor(struct TextProcessor *tp) {
    tp->stats.word_count = 0;
    tp->stats.total_chars = 0;
    tp->stats.capitalized_count = 0;
    tp->stats.longest_word_length = 0;
    memset(tp->input_buffer, 0, sizeof(tp->input_buffer));
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(tp->stats.words[i].word, 0, MAX_WORD_LEN);
        tp->stats.words[i].count = 0;
        tp->stats.words[i].length = 0;
        tp->stats.words[i].is_capitalized = 0;
    }
}

int is_valid_char(char c) {
    return isalpha(c) || c == ' ' || c == '\0' || c == '\n';
}

int find_word_index(struct TextProcessor *tp, const char *word) {
    for (int i = 0; i < tp->stats.word_count; i++) {
        if (strcmp(tp->stats.words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int add_word(struct TextProcessor *tp, const char *word, int length) {
    if (tp->stats.word_count >= MAX_WORDS || length >= MAX_WORD_LEN) {
        return 0;
    }
    
    int index = find_word_index(tp, word);
    if (index >= 0) {
        tp->stats.words[index].count++;
        return 1;
    }
    
    if (tp->stats.word_count < MAX_WORDS) {
        strncpy(tp->stats.words[tp->stats.word_count].word, word, MAX_WORD_LEN - 1);
        tp->stats.words[tp->stats.word_count].word[MAX_WORD_LEN - 1] = '\0';
        tp->stats.words[tp->stats.word_count].count = 1;
        tp->stats.words[tp->stats.word_count].length = length;
        tp->stats.words[tp->stats.word_count].is_capitalized = isupper(word[0]) ? 1 : 0;
        tp->stats.word_count++;
        return 1;
    }
    return 0;
}

int process_text(struct TextProcessor *tp, const char *text) {
    if (text == NULL) {
        return 0;
    }
    
    size_t text_len = strlen(text);
    if (text_len == 0 || text_len >= sizeof(tp->input_buffer)) {
        return 0;
    }
    
    for (size_t i = 0; i < text_len; i++) {
        if (!is_valid_char(text[i])) {
            return 0;
        }
    }
    
    char buffer[MAX_WORD_LEN];
    int buf_index = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= text_len; i++) {
        char c = text[i];
        
        if (isalpha(c)) {
            if (buf_index < MAX_WORD_LEN - 1) {
                buffer[buf_index++] = c;
                in_word = 1;
            } else {
                return 0;
            }
        } else if (in_word) {
            buffer[buf_index] = '\0';
            int word_len = buf_index;
            
            if (word_len > 0) {
                if (!add_word(tp, buffer, word_len)) {
                    return 0;
                }
                
                tp->stats.total_chars += word_len;
                if (isupper(buffer[0])) {
                    tp->stats.capitalized_count++;
                }
                if (word_len > tp->stats.longest_word_length) {
                    tp->stats.longest_word_length = word_len;
                }
            }
            
            buf_index = 0;
            in_word = 0;
        }
    }
    
    return 1;
}

void print_stats(struct TextProcessor *tp) {
    printf("Text Statistics:\n");
    printf("Total words: %d\n", tp->stats.word_count);
    printf("Total characters in words: %d\n", tp->stats.total_chars);
    printf("Capitalized words: %d\n", tp->stats.capitalized_count);
    printf("Longest word length: %d\n\n", tp->stats.longest_word_length);
    
    printf("Word frequency:\n");
    for (int i = 0; i < tp->stats.word_count; i++) {
        printf("'%s': %d occurrences\n", 
               tp->stats.words[i].word, 
               tp->stats.words[i].count);
    }
}

int main(void) {
    struct TextProcessor processor;
    init_text_processor(&processor);
    
    printf("Enter text to analyze (letters and spaces only): ");
    
    if (fgets(processor.input_buffer, sizeof(