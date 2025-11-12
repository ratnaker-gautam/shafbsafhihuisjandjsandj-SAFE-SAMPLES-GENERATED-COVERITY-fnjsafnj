//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 1000

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
    int unique_words;
    int longest_word_length;
};

struct TextProcessor {
    struct TextStats stats;
    char input_buffer[10000];
};

void init_text_processor(struct TextProcessor *tp) {
    tp->stats.word_count = 0;
    tp->stats.total_chars = 0;
    tp->stats.unique_words = 0;
    tp->stats.longest_word_length = 0;
    tp->input_buffer[0] = '\0';
    for (int i = 0; i < MAX_WORDS; i++) {
        tp->stats.words[i].word[0] = '\0';
        tp->stats.words[i].count = 0;
        tp->stats.words[i].length = 0;
        tp->stats.words[i].is_capitalized = 0;
    }
}

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

int find_word_index(struct TextProcessor *tp, const char *word) {
    for (int i = 0; i < tp->stats.word_count; i++) {
        if (strcmp(tp->stats.words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void add_word(struct TextProcessor *tp, const char *word, int length, int capitalized) {
    if (length <= 0 || length >= MAX_WORD_LEN) return;
    
    int index = find_word_index(tp, word);
    if (index >= 0) {
        tp->stats.words[index].count++;
        return;
    }
    
    if (tp->stats.word_count >= MAX_WORDS) return;
    
    struct WordInfo *wi = &tp->stats.words[tp->stats.word_count];
    strncpy(wi->word, word, MAX_WORD_LEN - 1);
    wi->word[MAX_WORD_LEN - 1] = '\0';
    wi->count = 1;
    wi->length = length;
    wi->is_capitalized = capitalized;
    
    tp->stats.word_count++;
    tp->stats.unique_words++;
    
    if (length > tp->stats.longest_word_length) {
        tp->stats.longest_word_length = length;
    }
}

void process_text(struct TextProcessor *tp, const char *text) {
    if (!text || strlen(text) >= sizeof(tp->input_buffer)) return;
    
    strncpy(tp->input_buffer, text, sizeof(tp->input_buffer) - 1);
    tp->input_buffer[sizeof(tp->input_buffer) - 1] = '\0';
    
    const char *p = tp->input_buffer;
    char current_word[MAX_WORD_LEN];
    int word_len = 0;
    int in_word = 0;
    
    while (*p) {
        if (is_word_char(*p)) {
            if (!in_word) {
                in_word = 1;
                word_len = 0;
            }
            if (word_len < MAX_WORD_LEN - 1) {
                current_word[word_len++] = *p;
            }
        } else {
            if (in_word && word_len > 0) {
                current_word[word_len] = '\0';
                int capitalized = isupper((unsigned char)current_word[0]) ? 1 : 0;
                add_word(tp, current_word, word_len, capitalized);
                tp->stats.total_chars += word_len;
                in_word = 0;
            }
        }
        p++;
    }
    
    if (in_word && word_len > 0) {
        current_word[word_len] = '\0';
        int capitalized = isupper((unsigned char)current_word[0]) ? 1 : 0;
        add_word(tp, current_word, word_len, capitalized);
        tp->stats.total_chars += word_len;
    }
}

void print_stats(struct TextProcessor *tp) {
    printf("Text Statistics:\n");
    printf("Total words: %d\n", tp->stats.word_count);
    printf("Unique words: %d\n", tp->stats.unique_words);
    printf("Total characters in words: %d\n", tp->stats.total_chars);
    printf("Longest word length: %d\n", tp->stats.longest_word_length);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < tp->stats.word_count; i++) {
        printf("%-20s: %d (length: %d, %s)\n", 
               tp->stats.words[i].word,
               tp->stats.words[i].count,
               tp->stats.words[i].length,
               tp->stats.words[i].is_capitalized ? "capitalized" : "lowercase");
    }
}

int main() {
    struct TextProcessor processor;
    init_text_processor(&processor);
    
    char input_text[] = "The quick brown fox jumps over the lazy dog. The quick brown fox is very quick!";
    
    printf("Processing text: %s\n\n",