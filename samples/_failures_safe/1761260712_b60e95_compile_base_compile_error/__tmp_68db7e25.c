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
    char buffer[1024];
};

void init_text_processor(struct TextProcessor *tp) {
    tp->stats.word_count = 0;
    tp->stats.total_chars = 0;
    tp->stats.unique_words = 0;
    tp->stats.longest_word_length = 0;
    memset(tp->buffer, 0, sizeof(tp->buffer));
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(tp->stats.words[i].word, 0, MAX_WORD_LEN);
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
    if (length <= 0 || length >= MAX_WORD_LEN || tp->stats.word_count >= MAX_WORDS) {
        return;
    }

    char normalized[MAX_WORD_LEN];
    strncpy(normalized, word, length);
    normalized[length] = '\0';
    
    for (int i = 0; normalized[i]; i++) {
        normalized[i] = tolower((unsigned char)normalized[i]);
    }

    int index = find_word_index(tp, normalized);
    if (index >= 0) {
        tp->stats.words[index].count++;
    } else {
        struct WordInfo *wi = &tp->stats.words[tp->stats.word_count];
        strncpy(wi->word, normalized, MAX_WORD_LEN - 1);
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
    tp->stats.total_chars += length;
}

void process_text(struct TextProcessor *tp, const char *text) {
    if (text == NULL) return;
    
    int len = strlen(text);
    if (len <= 0 || len >= sizeof(tp->buffer)) return;
    
    int in_word = 0;
    int word_start = 0;
    int word_length = 0;
    
    for (int i = 0; i <= len; i++) {
        char c = text[i];
        if (is_word_char(c) && !in_word) {
            in_word = 1;
            word_start = i;
            word_length = 1;
        } else if (is_word_char(c) && in_word) {
            word_length++;
        } else if (in_word) {
            int capitalized = isupper((unsigned char)text[word_start]) ? 1 : 0;
            add_word(tp, &text[word_start], word_length, capitalized);
            in_word = 0;
            word_length = 0;
        }
    }
}

void print_stats(struct TextProcessor *tp) {
    printf("Text Statistics:\n");
    printf("Total words: %d\n", tp->stats.word_count);
    printf("Unique words: %d\n", tp->stats.unique_words);
    printf("Total characters: %d\n", tp->stats.total_chars);
    printf("Longest word length: %d\n\n", tp->stats.longest_word_length);
    
    printf("Word frequency (top 10):\n");
    for (int i = 0; i < (tp->stats.word_count < 10 ? tp->stats.word_count : 10); i++) {
        printf("%-20s: %d\n", tp->stats.words[i].word, tp->stats.words[i].count);
    }
}

int main() {
    struct TextProcessor processor;
    init_text_processor(&processor);
    
    printf("Enter text to analyze (max %zu characters):\n", sizeof(processor.buffer) - 1);
    
    if (fgets(processor.buffer, sizeof(processor.buffer), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(processor.buffer);
    if (len > 0 && processor.buffer[len - 1] == '\n') {
        processor.buffer[len - 1] = '\0';
    }
    
    if (strlen(processor