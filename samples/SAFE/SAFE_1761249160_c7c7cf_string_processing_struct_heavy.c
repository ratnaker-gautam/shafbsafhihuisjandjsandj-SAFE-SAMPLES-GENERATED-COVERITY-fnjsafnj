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
    int total_words;
    int longest_word;
    int shortest_word;
};

struct TextProcessor {
    struct TextStats stats;
    char buffer[8192];
};

void init_processor(struct TextProcessor *proc) {
    proc->stats.word_count = 0;
    proc->stats.total_words = 0;
    proc->stats.longest_word = 0;
    proc->stats.shortest_word = MAX_WORD_LEN;
    memset(proc->buffer, 0, sizeof(proc->buffer));
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int add_word(struct TextProcessor *proc, const char *word, int len) {
    if (len <= 0 || len >= MAX_WORD_LEN || proc->stats.word_count >= MAX_WORDS) {
        return 0;
    }

    for (int i = 0; i < proc->stats.word_count; i++) {
        if (strncmp(proc->stats.words[i].word, word, len) == 0 && 
            proc->stats.words[i].word[len] == '\0') {
            proc->stats.words[i].count++;
            proc->stats.total_words++;
            return 1;
        }
    }

    if (proc->stats.word_count < MAX_WORDS) {
        struct WordInfo *wi = &proc->stats.words[proc->stats.word_count];
        strncpy(wi->word, word, len);
        wi->word[len] = '\0';
        wi->count = 1;
        wi->length = len;
        proc->stats.word_count++;
        proc->stats.total_words++;

        if (len > proc->stats.longest_word) {
            proc->stats.longest_word = len;
        }
        if (len < proc->stats.shortest_word) {
            proc->stats.shortest_word = len;
        }
        return 1;
    }
    return 0;
}

int process_text(struct TextProcessor *proc, const char *text) {
    if (text == NULL) {
        return 0;
    }

    size_t text_len = strlen(text);
    if (text_len == 0 || text_len >= sizeof(proc->buffer)) {
        return 0;
    }

    strncpy(proc->buffer, text, sizeof(proc->buffer) - 1);
    proc->buffer[sizeof(proc->buffer) - 1] = '\0';

    char *ptr = proc->buffer;
    while (*ptr) {
        while (*ptr && !is_word_char(*ptr)) {
            ptr++;
        }
        
        if (*ptr == '\0') {
            break;
        }

        char *word_start = ptr;
        while (*ptr && is_word_char(*ptr)) {
            ptr++;
        }

        int word_len = ptr - word_start;
        if (word_len > 0) {
            add_word(proc, word_start, word_len);
        }
    }
    return 1;
}

void print_stats(const struct TextProcessor *proc) {
    printf("Text Statistics:\n");
    printf("Total unique words: %d\n", proc->stats.word_count);
    printf("Total word occurrences: %d\n", proc->stats.total_words);
    printf("Longest word length: %d\n", proc->stats.longest_word);
    printf("Shortest word length: %d\n", proc->stats.shortest_word);
    printf("\nWord frequency:\n");

    for (int i = 0; i < proc->stats.word_count; i++) {
        const struct WordInfo *wi = &proc->stats.words[i];
        printf("%-20s: %d occurrences\n", wi->word, wi->count);
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

    if (!process_text(&processor, input)) {
        fprintf(stderr, "Error processing text\n");
        return 1;
    }

    print_stats(&processor);
    return 0;
}