//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 1000
#define MAX_LINE_LEN 256

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int length;
    int is_capitalized;
};

struct TextProcessor {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_words;
    int longest_word;
    int shortest_word;
};

void init_processor(struct TextProcessor *proc) {
    proc->word_count = 0;
    proc->total_words = 0;
    proc->longest_word = 0;
    proc->shortest_word = MAX_WORD_LEN;
    for (int i = 0; i < MAX_WORDS; i++) {
        proc->words[i].count = 0;
        proc->words[i].word[0] = '\0';
        proc->words[i].length = 0;
        proc->words[i].is_capitalized = 0;
    }
}

int is_valid_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

int find_word_index(struct TextProcessor *proc, const char *word) {
    for (int i = 0; i < proc->word_count; i++) {
        if (strcmp(proc->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void add_word(struct TextProcessor *proc, const char *word, int len) {
    if (len <= 0 || len >= MAX_WORD_LEN || proc->word_count >= MAX_WORDS) {
        return;
    }
    
    char clean_word[MAX_WORD_LEN];
    int clean_len = 0;
    int capitalized = isupper((unsigned char)word[0]);
    
    for (int i = 0; i < len && clean_len < MAX_WORD_LEN - 1; i++) {
        if (is_valid_char(word[i])) {
            clean_word[clean_len++] = tolower((unsigned char)word[i]);
        }
    }
    clean_word[clean_len] = '\0';
    
    if (clean_len == 0) {
        return;
    }
    
    int index = find_word_index(proc, clean_word);
    if (index >= 0) {
        proc->words[index].count++;
        if (capitalized) {
            proc->words[index].is_capitalized = 1;
        }
    } else {
        strncpy(proc->words[proc->word_count].word, clean_word, MAX_WORD_LEN - 1);
        proc->words[proc->word_count].word[MAX_WORD_LEN - 1] = '\0';
        proc->words[proc->word_count].count = 1;
        proc->words[proc->word_count].length = clean_len;
        proc->words[proc->word_count].is_capitalized = capitalized;
        proc->word_count++;
    }
    
    proc->total_words++;
    if (clean_len > proc->longest_word) {
        proc->longest_word = clean_len;
    }
    if (clean_len < proc->shortest_word) {
        proc->shortest_word = clean_len;
    }
}

void process_text(struct TextProcessor *proc, const char *text) {
    if (text == NULL) {
        return;
    }
    
    char buffer[MAX_LINE_LEN];
    int buf_pos = 0;
    
    for (int i = 0; text[i] != '\0' && buf_pos < MAX_LINE_LEN - 1; i++) {
        if (is_valid_char(text[i])) {
            buffer[buf_pos++] = text[i];
        } else if (buf_pos > 0) {
            buffer[buf_pos] = '\0';
            add_word(proc, buffer, buf_pos);
            buf_pos = 0;
        }
    }
    
    if (buf_pos > 0) {
        buffer[buf_pos] = '\0';
        add_word(proc, buffer, buf_pos);
    }
}

void print_stats(const struct TextProcessor *proc) {
    printf("Text Analysis Results:\n");
    printf("Total words processed: %d\n", proc->total_words);
    printf("Unique words: %d\n", proc->word_count);
    printf("Longest word length: %d\n", proc->longest_word);
    printf("Shortest word length: %d\n", proc->shortest_word);
    printf("\nWord Frequency:\n");
    
    for (int i = 0; i < proc->word_count; i++) {
        printf("%-20s: %d occurrences", proc->words[i].word, proc->words[i].count);
        if (proc->words[i].is_capitalized) {
            printf(" (capitalized)");
        }
        printf("\n");
    }
}

int main(void) {
    struct TextProcessor processor;
    init_processor(&processor);
    
    char input_text[] = "The quick brown fox jumps over the lazy dog. The dog was not amused by the fox's antics. Quick thinking saved the day!";
    
    printf("Processing text...\n");
    process_text(&processor, input_text);
    print_stats(&processor);
    
    return 0;
}