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
    size_t longest_word;
};

struct TextProcessor {
    struct TextStats stats;
    char *buffer;
    size_t capacity;
};

void init_processor(struct TextProcessor *proc, size_t initial_capacity) {
    proc->stats.char_count = 0;
    proc->stats.word_count = 0;
    proc->stats.line_count = 0;
    proc->stats.sentence_count = 0;
    proc->stats.longest_word = 0;
    proc->capacity = initial_capacity > 0 ? initial_capacity : 64;
    proc->buffer = malloc(proc->capacity);
    if (proc->buffer) {
        proc->buffer[0] = '\0';
    }
}

void free_processor(struct TextProcessor *proc) {
    if (proc->buffer) {
        free(proc->buffer);
        proc->buffer = NULL;
    }
}

int resize_buffer(struct TextProcessor *proc, size_t new_size) {
    if (new_size == 0) return 0;
    char *new_buf = realloc(proc->buffer, new_size);
    if (!new_buf) return 0;
    proc->buffer = new_buf;
    proc->capacity = new_size;
    return 1;
}

int append_text(struct TextProcessor *proc, const char *text) {
    if (!text || !proc->buffer) return 0;
    size_t text_len = strlen(text);
    size_t current_len = strlen(proc->buffer);
    size_t required = current_len + text_len + 1;
    if (required > proc->capacity) {
        size_t new_cap = proc->capacity * 2;
        while (new_cap < required) new_cap *= 2;
        if (!resize_buffer(proc, new_cap)) return 0;
    }
    if (current_len + text_len < proc->capacity) {
        strcat(proc->buffer, text);
        return 1;
    }
    return 0;
}

void analyze_text(struct TextProcessor *proc) {
    if (!proc->buffer) return;
    struct TextStats *stats = &proc->stats;
    stats->char_count = strlen(proc->buffer);
    stats->word_count = 0;
    stats->line_count = 0;
    stats->sentence_count = 0;
    stats->longest_word = 0;
    
    int in_word = 0;
    size_t current_word_len = 0;
    size_t i = 0;
    
    while (proc->buffer[i]) {
        char c = proc->buffer[i];
        if (c == '\n') {
            stats->line_count++;
        }
        if (c == '.' || c == '!' || c == '?') {
            stats->sentence_count++;
        }
        if (isspace((unsigned char)c)) {
            if (in_word) {
                stats->word_count++;
                if (current_word_len > stats->longest_word) {
                    stats->longest_word = current_word_len;
                }
                current_word_len = 0;
                in_word = 0;
            }
        } else {
            in_word = 1;
            current_word_len++;
        }
        i++;
    }
    if (in_word) {
        stats->word_count++;
        if (current_word_len > stats->longest_word) {
            stats->longest_word = current_word_len;
        }
    }
    if (stats->line_count == 0 && stats->char_count > 0) {
        stats->line_count = 1;
    }
}

void print_stats(const struct TextStats *stats) {
    printf("Text Statistics:\n");
    printf("Characters: %zu\n", stats->char_count);
    printf("Words: %zu\n", stats->word_count);
    printf("Lines: %zu\n", stats->line_count);
    printf("Sentences: %zu\n", stats->sentence_count);
    printf("Longest word: %zu characters\n", stats->longest_word);
}

int main(void) {
    struct TextProcessor processor;
    init_processor(&processor, 256);
    
    printf("Enter text (empty line to finish):\n");
    char line[512];
    
    while (fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\n') break;
        if (!append_text(&processor, line)) {
            printf("Error: Failed to append text\n");
            free_processor(&processor);
            return 1;
        }
    }
    
    if (strlen(processor.buffer) == 0) {
        printf("No text provided.\n");
        free_processor(&processor);
        return 0;
    }
    
    analyze_text(&processor);
    print_stats(&processor.stats);
    
    free_processor(&processor);
    return 0;
}