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
    int longest_word;
};

struct TextProcessor {
    struct TextStats stats;
    char buffer[1024];
};

void init_processor(struct TextProcessor *proc) {
    proc->stats.word_count = 0;
    proc->stats.total_chars = 0;
    proc->stats.longest_word = 0;
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
    
    if (len > proc->stats.longest_word) {
        proc->stats.longest_word = len;
    }
    
    proc->stats.total_chars += len;
    return 1;
}

int process_text(struct TextProcessor *proc, const char *text) {
    if (text == NULL) {
        return 0;
    }

    int len = strlen(text);
    if (len <= 0 || len >= sizeof(proc->buffer)) {
        return 0;
    }

    int in_word = 0;
    int word_start = 0;
    
    for (int i = 0; i <= len; i++) {
        char c = text[i];
        
        if (is_word_char(c) && !in_word) {
            in_word = 1;
            word_start = i;
        } else if (!is_word_char(c) && in_word) {
            int word_len = i - word_start;
            if (word_len > 0) {
                add_word(proc, text + word_start, word_len);
            }
            in_word = 0;
        }
    }
    
    return proc->stats.word_count > 0;
}

void print_stats(const struct TextProcessor *proc) {
    printf("Text Statistics:\n");
    printf("Total words: %d\n", proc->stats.word_count);
    printf("Total characters in words: %d\n", proc->stats.total_chars);
    printf("Longest word length: %d\n", proc->stats.longest_word);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < proc->stats.word_count; i++) {
        const struct WordInfo *wi = &proc->stats.words[i];
        printf("%-20s: %d occurrences\n", wi->word, wi->count);
    }
}

int main(void) {
    struct TextProcessor processor;
    init_processor(&processor);
    
    printf("Enter text to analyze (max %zu characters):\n", sizeof(processor.buffer) - 1);
    
    if (fgets(processor.buffer, sizeof(processor.buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(processor.buffer);
    if (input_len > 0 && processor.buffer[input_len - 1] == '\n') {
        processor.buffer[input_len - 1] = '\0';
    }
    
    if (strlen(processor.buffer) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    if (!process_text(&processor, processor.buffer)) {
        printf("No words found in input.\n");
        return 1;
    }
    
    print_stats(&processor);
    return 0;
}