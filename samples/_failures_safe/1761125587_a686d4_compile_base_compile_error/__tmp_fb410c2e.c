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
    int unique_words;
};

struct TextProcessor {
    struct TextStats stats;
    char buffer[8192];
};

void init_processor(struct TextProcessor *proc) {
    proc->stats.word_count = 0;
    proc->stats.total_chars = 0;
    proc->stats.unique_words = 0;
    memset(proc->buffer, 0, sizeof(proc->buffer));
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(proc->stats.words[i].word, 0, MAX_WORD_LEN);
        proc->stats.words[i].count = 0;
        proc->stats.words[i].length = 0;
    }
}

int is_valid_char(char c) {
    return isalpha((unsigned char)c) || c == ' ' || c == '\n' || c == '\t';
}

int find_word_index(struct TextStats *stats, const char *word) {
    for (int i = 0; i < stats->unique_words; i++) {
        if (strcmp(stats->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int add_word(struct TextStats *stats, const char *word, int len) {
    if (stats->unique_words >= MAX_WORDS || len >= MAX_WORD_LEN || len <= 0) {
        return 0;
    }
    
    int idx = find_word_index(stats, word);
    if (idx >= 0) {
        stats->words[idx].count++;
        return 1;
    }
    
    if (stats->unique_words < MAX_WORDS) {
        strncpy(stats->words[stats->unique_words].word, word, MAX_WORD_LEN - 1);
        stats->words[stats->unique_words].word[MAX_WORD_LEN - 1] = '\0';
        stats->words[stats->unique_words].count = 1;
        stats->words[stats->unique_words].length = len;
        stats->unique_words++;
        return 1;
    }
    return 0;
}

int process_text(struct TextProcessor *proc, const char *input) {
    if (!input || strlen(input) >= sizeof(proc->buffer) - 1) {
        return 0;
    }
    
    strncpy(proc->buffer, input, sizeof(proc->buffer) - 1);
    proc->buffer[sizeof(proc->buffer) - 1] = '\0';
    
    char temp[MAX_WORD_LEN];
    int temp_idx = 0;
    int in_word = 0;
    
    for (int i = 0; proc->buffer[i] != '\0'; i++) {
        if (!is_valid_char(proc->buffer[i])) {
            return 0;
        }
        
        if (isalpha((unsigned char)proc->buffer[i])) {
            if (temp_idx < MAX_WORD_LEN - 1) {
                temp[temp_idx++] = tolower((unsigned char)proc->buffer[i]);
                in_word = 1;
            }
        } else {
            if (in_word && temp_idx > 0) {
                temp[temp_idx] = '\0';
                if (add_word(&proc->stats, temp, temp_idx)) {
                    proc->stats.word_count++;
                    proc->stats.total_chars += temp_idx;
                }
                temp_idx = 0;
                in_word = 0;
            }
        }
    }
    
    if (in_word && temp_idx > 0) {
        temp[temp_idx] = '\0';
        if (add_word(&proc->stats, temp, temp_idx)) {
            proc->stats.word_count++;
            proc->stats.total_chars += temp_idx;
        }
    }
    
    return 1;
}

void print_stats(struct TextProcessor *proc) {
    printf("Text Statistics:\n");
    printf("Total words: %d\n", proc->stats.word_count);
    printf("Total characters: %d\n", proc->stats.total_chars);
    printf("Unique words: %d\n", proc->stats.unique_words);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < proc->stats.unique_words; i++) {
        printf("%s: %d\n", proc->stats.words[i].word, proc->stats.words[i].count);
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
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (!process_text(&processor, input