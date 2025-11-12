//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define LEVEL_ERROR 0
#define LEVEL_WARN 1
#define LEVEL_INFO 2
#define LEVEL_DEBUG 3

#define IS_VALID_LEVEL(l) ((l) >= LEVEL_ERROR && (l) <= LEVEL_DEBUG)

struct log_entry {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
};

const char* level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};
const char* level_colors[] = {"\033[1;31m", "\033[1;33m", "\033[1;32m", "\033[1;34m"};

int parse_log_level(const char* str) {
    if (strncmp(str, "ERROR", 5) == 0) return LEVEL_ERROR;
    if (strncmp(str, "WARN", 4) == 0) return LEVEL_WARN;
    if (strncmp(str, "INFO", 4) == 0) return LEVEL_INFO;
    if (strncmp(str, "DEBUG", 5) == 0) return LEVEL_DEBUG;
    return -1;
}

int validate_timestamp(const char* ts) {
    if (strlen(ts) != TIMESTAMP_LEN - 1) return 0;
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (i == 4 || i == 7) {
            if (ts[i] != '-') return 0;
        } else if (i == 10) {
            if (ts[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (ts[i] != ':') return 0;
        } else {
            if (!isdigit(ts[i])) return 0;
        }
    }
    return 1;
}

int read_log_entries(struct log_entry entries[], int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        struct log_entry* entry = &entries[count];
        
        if (sscanf(line, "%19s", entry->timestamp) != 1) continue;
        if (!validate_timestamp(entry->timestamp)) continue;
        
        const char* level_start = line + TIMESTAMP_LEN;
        while (*level_start == ' ' && level_start < line + sizeof(line) - 1) level_start++;
        
        int level = parse_log_level(level_start);
        if (!IS_VALID_LEVEL(level)) continue;
        entry->level = level;
        
        const char* msg_start = level_start;
        while (*msg_start && *msg_start != ' ' && msg_start < line + sizeof(line) - 1) msg_start++;
        while (*msg_start == ' ' && msg_start < line + sizeof(line) - 1) msg_start++;
        
        size_t msg_len = strlen(msg_start);
        if (msg_len > 0 && msg_start[msg_len - 1] == '\n') msg_len--;
        if (msg_len >= sizeof(entry->message)) msg_len = sizeof(entry->message) - 1;
        
        if (msg_len > 0) {
            strncpy(entry->message, msg_start, msg_len);
            entry->message[msg_len] = '\0';
        } else {
            entry->message[0] = '\0';
        }
        
        count++;
    }
    
    return count;
}

void print_statistics(struct log_entry entries[], int count) {
    int level_counts[LOG_LEVELS] = {0};
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("\nLog Statistics:\n");
    printf("Total entries: %d\n", count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d (%.1f%%)\n", level_names[i], level_counts[i], 
               count > 0 ? (level_counts[i] * 100.0 / count) : 0.0);
    }
}

void display_entries(struct log_entry entries[], int count, int min_level) {
    printf("\nLog Entries (Level %s and above):\n", level_names[min_level]);
    printf("========================================\n");
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level <= min_level) {
            printf("%s%s\033[0m [%s] %s\n", 
                   level_colors[entries[i].level],
                   level_names[entries[i].level],
                   entries[i].timestamp,
                   entries[i].message);
        }
    }
}

int main(void) {
    struct log_entry entries[MAX_ENTRIES];
    
    printf("Log Analyzer - Enter log entries (Ctrl+D to end):\n");
    printf("Format