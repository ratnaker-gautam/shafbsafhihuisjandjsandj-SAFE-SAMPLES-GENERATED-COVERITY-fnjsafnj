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

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
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

    if (len < MAX_WORD_LEN) {
        strncpy(tp->stats.words[tp->stats.word_count].word, word, len);
        tp->stats.words[tp->stats.word_count].word[len] = '\0';
        tp->stats.words[tp->stats.word_count].count = 1;
        tp->stats.words[tp->stats.word_count].length = len;
        tp->stats.word_count++;
        
        if (len > tp->stats.longest_word_len) {
            tp->stats.longest_word_len = len;
        }
        return 1;
    }
    return 0;
}

int process_text(struct TextProcessor *tp, const char *text) {
    if (text == NULL) {
        return 0;
    }

    int text_len = strlen(text);
    if (text_len <= 0 || text_len >= sizeof(tp->buffer)) {
        return 0;
    }

    strncpy(tp->buffer, text, sizeof(tp->buffer) - 1);
    tp->buffer[sizeof(tp->buffer) - 1] = '\0';

    int in_word = 0;
    int word_start = 0;
    int valid_chars = 0;

    for (int i = 0; i <= text_len; i++) {
        if (is_valid_char(tp->buffer[i])) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
            valid_chars++;
        } else {
            if (in_word) {
                int word_len = i - word_start;
                if (word_len > 0 && word_len < MAX_WORD_LEN) {
                    add_word(tp, tp->buffer + word_start, word_len);
                }
                in_word = 0;
            }
        }
    }

    tp->stats.total_chars = valid_chars;
    return 1;
}

void print_stats(const struct TextProcessor *tp) {
    printf("Text Statistics:\n");
    printf("Total characters: %d\n", tp->stats.total_chars);
    printf("Unique words: %d\n", tp->stats.word_count);
    printf("Longest word length: %d\n\n", tp->stats.longest_word_len);
    
    printf("Word frequencies:\n");
    for (int i = 0; i < tp->stats.word_count; i++) {
        printf("%-20s: %d\n", tp->stats.words[i].word, tp->stats.words[i].count);
    }
}

int main(void) {
    struct TextProcessor processor;
    init_text_processor(&processor);

    char input[1024];
    printf("Enter text to analyze (max %zu characters): ", sizeof(input) - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }

    if (!process_text(&processor, input)) {
        fprintf(stderr, "Error processing text\n");
        return 1;
    }

    print_stats(&processor);
    return 0;
}