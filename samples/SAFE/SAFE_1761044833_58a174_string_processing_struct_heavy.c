//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: string_processing
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
};

struct TextStats {
    struct WordInfo words[MAX_WORDS];
    int word_count;
    int total_chars;
    int longest_word_len;
};

struct TextProcessor {
    struct TextStats stats;
    char buffer[1024];
};

void init_text_processor(struct TextProcessor *tp) {
    tp->stats.word_count = 0;
    tp->stats.total_chars = 0;
    tp->stats.longest_word_len = 0;
    memset(tp->buffer, 0, sizeof(tp->buffer));
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_' || c == '-';
}

int add_word(struct TextProcessor *tp, const char *word, int len) {
    if (len <= 0 || len >= MAX_WORD_LEN || tp->stats.word_count >= MAX_WORDS) {
        return 0;
    }

    for (int i = 0; i < tp->stats.word_count; i++) {
        if (strncmp(tp->stats.words[i].word, word, len) == 0 && 
            tp->stats.words[i].word[len] == '\0') {
            tp->stats.words[i].count++;
            return 1;
        }
    }

    struct WordInfo *wi = &tp->stats.words[tp->stats.word_count];
    strncpy(wi->word, word, len);
    wi->word[len] = '\0';
    wi->count = 1;
    wi->length = len;
    tp->stats.word_count++;

    if (len > tp->stats.longest_word_len) {
        tp->stats.longest_word_len = len;
    }

    tp->stats.total_chars += len;
    return 1;
}

void process_text(struct TextProcessor *tp, const char *text) {
    if (text == NULL) return;

    const char *p = text;
    char current_word[MAX_WORD_LEN];
    int word_len = 0;

    while (*p != '\0') {
        if (is_word_char(*p)) {
            if (word_len < MAX_WORD_LEN - 1) {
                current_word[word_len++] = (char)tolower((unsigned char)*p);
            }
        } else {
            if (word_len > 0) {
                current_word[word_len] = '\0';
                add_word(tp, current_word, word_len);
                word_len = 0;
            }
        }
        p++;
    }

    if (word_len > 0) {
        current_word[word_len] = '\0';
        add_word(tp, current_word, word_len);
    }
}

void print_stats(const struct TextProcessor *tp) {
    printf("Text Statistics:\n");
    printf("Total words: %d\n", tp->stats.word_count);
    printf("Total characters in words: %d\n", tp->stats.total_chars);
    printf("Longest word length: %d\n\n", tp->stats.longest_word_len);

    printf("Word frequencies:\n");
    for (int i = 0; i < tp->stats.word_count; i++) {
        const struct WordInfo *wi = &tp->stats.words[i];
        printf("%-*s : %d\n", tp->stats.longest_word_len, wi->word, wi->count);
    }
}

int main() {
    struct TextProcessor processor;
    init_text_processor(&processor);

    printf("Enter text (max %zu characters): ", sizeof(processor.buffer) - 1);
    
    if (fgets(processor.buffer, sizeof(processor.buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(processor.buffer);
    if (len > 0 && processor.buffer[len - 1] == '\n') {
        processor.buffer[len - 1] = '\0';
    }

    if (strlen(processor.buffer) == 0) {
        printf("No input provided.\n");
        return 1;
    }

    process_text(&processor, processor.buffer);
    print_stats(&processor);

    return 0;
}