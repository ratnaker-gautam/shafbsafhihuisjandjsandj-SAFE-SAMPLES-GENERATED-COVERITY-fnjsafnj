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

int parse_timestamp(const char* str, char* timestamp) {
    if (!str || !timestamp) return 0;
    size_t str_len = strlen(str);
    if (str_len < TIMESTAMP_LEN - 1) return 0;
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (i == 4 || i == 7) {
            if (str[i] != '-') return 0;
        } else if (i == 10) {
            if (str[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (str[i] != ':') return 0;
        } else {
            if (!isdigit((unsigned char)str[i])) return 0;
        }
        timestamp[i] = str[i];
    }
    timestamp[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int parse_level(const char* str) {
    if (!str) return -1;
    if (strncmp(str, "ERROR", 5) == 0) return LEVEL_ERROR;
    if (strncmp(str, "WARN", 4) == 0) return LEVEL_WARN;
    if (strncmp(str, "INFO", 4) == 0) return LEVEL_INFO;
    if (strncmp(str, "DEBUG", 5) == 0) return LEVEL_DEBUG;
    return -1;
}

int read_log_entries(struct log_entry* entries, int max_entries) {
    if (!entries || max_entries <= 0) return 0;
    int count = 0;
    char line[MAX_LINE_LEN];
    
    while (count < max_entries && fgets(line, sizeof(line), stdin)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
            len--;
        }
        
        if (len < TIMESTAMP_LEN + 6) continue;
        
        struct log_entry entry;
        if (!parse_timestamp(line, entry.timestamp)) continue;
        
        const char* level_start = line + TIMESTAMP_LEN;
        while (*level_start == ' ') level_start++;
        
        int level = parse_level(level_start);
        if (!IS_VALID_LEVEL(level)) continue;
        
        const char* message_start = level_start;
        while (*message_start && *message_start != ' ') message_start++;
        while (*message_start == ' ') message_start++;
        
        if (strlen(message_start) >= sizeof(entry.message)) continue;
        
        entry.level = level;
        strncpy(entry.message, message_start, sizeof(entry.message) - 1);
        entry.message[sizeof(entry.message) - 1] = '\0';
        
        entries[count] = entry;
        count++;
    }
    
    return count;
}

void print_statistics(const struct log_entry* entries, int count) {
    if (!entries || count <= 0) return;
    int level_counts[LOG_LEVELS] = {0};
    const char* level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("Log Statistics:\n");
    printf("Total entries: %d\n", count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d (%.1f%%)\n", level_names[i], level_counts[i],
               count > 0 ? (level_counts[i] * 100.0 / count) : 0.0);
    }
}

void filter_by_level(const struct log_entry* entries, int count, int filter_level) {
    if (!entries || count <= 0 || !IS_VALID_LEVEL(filter_level)) return;
    
    const char* level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};
    printf("\nEntries with level %s:\n", level_names[filter_level]);
    
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (entries[i].level == filter_level) {
            printf("%s %s %s\n", entries[i].timestamp, level_names[filter_level], entries[i].message);
            found++;
        }
    }
    
    if (!found) {
        printf("No entries found.\n");
    }
}

int main(void) {
    struct log_entry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    int