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
    return isalnum((unsigned char)c) || c == '_';
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

    proc->stats.total_chars += len;
    return 1;
}

int process_text(struct TextProcessor *proc, const char *text) {
    if (text == NULL) {
        return 0;
    }

    const char *p = text;
    while (*p != '\0') {
        while (*p != '\0' && !is_word_char(*p)) {
            p++;
        }

        if (*p == '\0') {
            break;
        }

        const char *start = p;
        while (*p != '\0' && is_word_char(*p)) {
            p++;
        }

        int word_len = p - start;
        if (word_len > 0) {
            if (!add_word(proc, start, word_len)) {
                return 0;
            }
        }
    }
    return 1;
}

void print_stats(const struct TextProcessor *proc) {
    printf("Text Statistics:\n");
    printf("Total words: %d\n", proc->stats.word_count);
    printf("Total characters in words: %d\n", proc->stats.total_chars);
    printf("Longest word length: %d\n", proc->stats.longest_word_len);
    printf("\nWord frequency:\n");

    for (int i = 0; i < proc->stats.word_count; i++) {
        const struct WordInfo *wi = &proc->stats.words[i];
        printf("%-*s : %d\n", proc->stats.longest_word_len, wi->word, wi->count);
    }
}

int main(void) {
    struct TextProcessor processor;
    init_processor(&processor);

    printf("Enter text (max %zu characters):\n", sizeof(processor.buffer) - 1);
    
    if (fgets(processor.buffer, sizeof(processor.buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(processor.buffer);
    if (len > 0 && processor.buffer[len - 1] == '\n') {
        processor.buffer[len - 1] = '\0';
    }

    if (strlen(processor.buffer) == 0) {
        fprintf(stderr, "No input provided\n");
        return 1;
    }

    if (!process_text(&processor, processor.buffer)) {
        fprintf(stderr, "Error processing text\n");
        return 1;
    }

    print_stats(&processor);
    return 0;
}