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
    char buffer[8192];
};

void init_processor(struct TextProcessor *proc) {
    proc->stats.word_count = 0;
    proc->stats.total_chars = 0;
    proc->stats.longest_word_len = 0;
    memset(proc->buffer, 0, sizeof(proc->buffer));
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_' || c == '-';
}

int add_word(struct TextProcessor *proc, const char *word, int len) {
    if (len <= 0 || len >= MAX_WORD_LEN || proc->stats.word_count >= MAX_WORDS) {
        return 0;
    }

    for (int i = 0; i < proc->stats.word_count; i++) {
        if (strncmp(proc->stats.words[i].word, word, len) == 0 && 
            proc->stats.words[i].word[len] == '\0') {
            proc->stats.words[i].count++;
            return 1;
        }
    }

    struct WordInfo *wi = &proc->stats.words[proc->stats.word_count];
    strncpy(wi->word, word, len);
    wi->word[len] = '\0';
    wi->count = 1;
    wi->length = len;
    proc->stats.word_count++;

    if (len > proc->stats.longest_word_len) {
        proc->stats.longest_word_len = len;
    }

    return 1;
}

int process_text(struct TextProcessor *proc, const char *text) {
    if (text == NULL) {
        return 0;
    }

    int text_len = strlen(text);
    if (text_len >= sizeof(proc->buffer)) {
        return 0;
    }

    strncpy(proc->buffer, text, sizeof(proc->buffer) - 1);
    proc->buffer[sizeof(proc->buffer) - 1] = '\0';

    int in_word = 0;
    int word_start = 0;

    for (int i = 0; i <= text_len; i++) {
        if (is_word_char(proc->buffer[i])) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                int word_len = i - word_start;
                if (word_len > 0) {
                    add_word(proc, &proc->buffer[word_start], word_len);
                    proc->stats.total_chars += word_len;
                }
                in_word = 0;
            }
        }
    }

    return 1;
}

void print_stats(const struct TextProcessor *proc) {
    printf("Text Statistics:\n");
    printf("Total words: %d\n", proc->stats.word_count);
    printf("Total characters in words: %d\n", proc->stats.total_chars);
    printf("Longest word length: %d\n\n", proc->stats.longest_word_len);

    printf("Word frequency:\n");
    for (int i = 0; i < proc->stats.word_count; i++) {
        const struct WordInfo *wi = &proc->stats.words[i];
        printf("%-*s : %d\n", proc->stats.longest_word_len, wi->word, wi->count);
    }
}

int main(void) {
    struct TextProcessor processor;
    init_processor(&processor);

    char input[4096];
    printf("Enter text to analyze (max %zu characters):\n", sizeof(input) - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    if (strlen(input) == 0) {
        printf("No input provided.\n");
        return 1;
    }

    if (!process_text(&processor, input)) {
        fprintf(stderr, "Error processing text\n");
        return 1;
    }

    print_stats(&processor);

    return 0;
}