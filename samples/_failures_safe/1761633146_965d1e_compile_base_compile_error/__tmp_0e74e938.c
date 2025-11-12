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
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(proc->stats.words[i].word, 0, MAX_WORD_LEN);
        proc->stats.words[i].count = 0;
        proc->stats.words[i].length = 0;
    }
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_' || c == '-';
}

int find_word_index(struct TextStats *stats, const char *word) {
    for (int i = 0; i < stats->word_count; i++) {
        if (strcmp(stats->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int add_word(struct TextStats *stats, const char *word, int len) {
    if (stats->word_count >= MAX_WORDS || len >= MAX_WORD_LEN || len <= 0) {
        return -1;
    }
    
    int idx = find_word_index(stats, word);
    if (idx >= 0) {
        stats->words[idx].count++;
        return idx;
    }
    
    if (stats->word_count < MAX_WORDS) {
        strncpy(stats->words[stats->word_count].word, word, MAX_WORD_LEN - 1);
        stats->words[stats->word_count].word[MAX_WORD_LEN - 1] = '\0';
        stats->words[stats->word_count].count = 1;
        stats->words[stats->word_count].length = len;
        stats->word_count++;
        return stats->word_count - 1;
    }
    return -1;
}

void process_text(struct TextProcessor *proc, const char *text) {
    if (!text || !proc) return;
    
    int len = strlen(text);
    if (len <= 0 || len >= sizeof(proc->buffer) - 1) return;
    
    strncpy(proc->buffer, text, sizeof(proc->buffer) - 1);
    proc->buffer[sizeof(proc->buffer) - 1] = '\0';
    
    int i = 0;
    while (i < len && proc->buffer[i] != '\0') {
        while (i < len && !is_word_char(proc->buffer[i])) {
            i++;
        }
        
        if (i >= len || proc->buffer[i] == '\0') break;
        
        int start = i;
        while (i < len && is_word_char(proc->buffer[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            char temp_word[MAX_WORD_LEN];
            strncpy(temp_word, &proc->buffer[start], word_len);
            temp_word[word_len] = '\0';
            
            for (int j = 0; j < word_len; j++) {
                temp_word[j] = tolower((unsigned char)temp_word[j]);
            }
            
            int idx = add_word(&proc->stats, temp_word, word_len);
            if (idx >= 0) {
                proc->stats.total_chars += word_len;
                if (word_len > proc->stats.longest_word_len) {
                    proc->stats.longest_word_len = word_len;
                }
            }
        }
    }
}

void print_stats(struct TextStats *stats) {
    if (!stats || stats->word_count <= 0) {
        printf("No words processed.\n");
        return;
    }
    
    printf("Text Statistics:\n");
    printf("Total words: %d\n", stats->word_count);
    printf("Total characters in words: %d\n", stats->total_chars);
    printf("Longest word length: %d\n", stats->longest_word_len);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < stats->word_count; i++) {
        if (stats->words[i].count > 0) {
            printf("%-20s: %d\n", stats->words[i].word, stats->words[i].count);
        }
    }
}

int main() {
    struct TextProcessor processor;
    init_processor(&processor);
    
    char input[4096];
    printf("Enter text to analyze (max 4095 characters): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';