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
    int longest_word;
};

struct TextProcessor {
    struct TextStats stats;
    char buffer[8192];
};

void init_processor(struct TextProcessor *proc) {
    proc->stats.word_count = 0;
    proc->stats.total_chars = 0;
    proc->stats.longest_word = 0;
    memset(proc->buffer, 0, sizeof(proc->buffer));
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(proc->stats.words[i].word, 0, MAX_WORD_LEN);
        proc->stats.words[i].count = 0;
        proc->stats.words[i].length = 0;
    }
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int find_word_index(struct TextStats *stats, const char *word) {
    for (int i = 0; i < stats->word_count; i++) {
        if (strcmp(stats->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int add_word(struct TextStats *stats, const char *word, int length) {
    if (stats->word_count >= MAX_WORDS || length >= MAX_WORD_LEN) {
        return 0;
    }
    
    int index = find_word_index(stats, word);
    if (index >= 0) {
        stats->words[index].count++;
        return 1;
    }
    
    if (stats->word_count < MAX_WORDS) {
        strncpy(stats->words[stats->word_count].word, word, MAX_WORD_LEN - 1);
        stats->words[stats->word_count].word[MAX_WORD_LEN - 1] = '\0';
        stats->words[stats->word_count].count = 1;
        stats->words[stats->word_count].length = length;
        stats->word_count++;
        
        if (length > stats->longest_word) {
            stats->longest_word = length;
        }
        return 1;
    }
    return 0;
}

void process_text(struct TextProcessor *proc, const char *text) {
    if (!text) return;
    
    size_t text_len = strlen(text);
    if (text_len >= sizeof(proc->buffer)) {
        return;
    }
    
    char temp[8192];
    strncpy(temp, text, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    char *ptr = temp;
    while (*ptr) {
        while (*ptr && !is_word_char(*ptr)) {
            ptr++;
        }
        
        if (!*ptr) break;
        
        char *word_start = ptr;
        while (*ptr && is_word_char(*ptr)) {
            ptr++;
        }
        
        int word_len = ptr - word_start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            char word[MAX_WORD_LEN];
            strncpy(word, word_start, word_len);
            word[word_len] = '\0';
            
            for (int i = 0; i < word_len; i++) {
                word[i] = tolower((unsigned char)word[i]);
            }
            
            if (add_word(&proc->stats, word, word_len)) {
                proc->stats.total_chars += word_len;
            }
        }
        
        if (!*ptr) break;
        ptr++;
    }
}

void print_stats(struct TextProcessor *proc) {
    printf("Text Statistics:\n");
    printf("Total words: %d\n", proc->stats.word_count);
    printf("Total characters in words: %d\n", proc->stats.total_chars);
    printf("Longest word length: %d\n", proc->stats.longest_word);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < proc->stats.word_count; i++) {
        printf("%-20s: %d\n", proc->stats.words[i].word, proc->stats.words[i].count);
    }
}

int main(void) {
    struct TextProcessor processor;
    init_processor(&processor);
    
    char input[4096];
    printf("Enter text to analyze (max 4095 characters): ");
    
    if (fgets(input, sizeof(input), stdin)) {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strlen(input) > 0) {
            process_text(&processor, input);
            print_stats(&processor);
        } else {
            printf("No input provided.\n");
        }
    } else {
        printf("Error reading input.\n");
    }
    
    return 0;
}