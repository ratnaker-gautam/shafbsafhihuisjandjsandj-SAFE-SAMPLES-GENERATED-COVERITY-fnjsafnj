//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARN 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3

#define IS_VALID_LEVEL(l) ((l) >= LOG_LEVEL_ERROR && (l) <= LOG_LEVEL_DEBUG)

struct log_entry {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
};

static const char* level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};

int parse_timestamp(const char* str, char* timestamp) {
    if (!str || !timestamp) return 0;
    if (strlen(str) < TIMESTAMP_LEN - 1) return 0;
    
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        char c = str[i];
        if (i == 4 || i == 7) {
            if (c != '-') return 0;
        } else if (i == 10) {
            if (c != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (c != ':') return 0;
        } else {
            if (!isdigit(c)) return 0;
        }
        timestamp[i] = c;
    }
    timestamp[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int parse_log_level(const char* str) {
    if (!str) return -1;
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strncmp(str, level_names[i], strlen(level_names[i])) == 0) {
            return i;
        }
    }
    return -1;
}

int read_log_entries(struct log_entry* entries, int max_entries) {
    if (!entries || max_entries <= 0) return 0;
    
    char line[MAX_LINE_LEN];
    int count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Press Ctrl+D (Linux) or Ctrl+Z (Windows) to finish.\n");
    
    while (count < max_entries && fgets(line, sizeof(line), stdin)) {
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        struct log_entry entry;
        memset(&entry, 0, sizeof(entry));
        
        if (!parse_timestamp(line, entry.timestamp)) {
            printf("Invalid timestamp format. Use: YYYY-MM-DD HH:MM:SS\n");
            continue;
        }
        
        char* level_start = line + TIMESTAMP_LEN;
        while (*level_start == ' ') level_start++;
        
        int level = parse_log_level(level_start);
        if (!IS_VALID_LEVEL(level)) {
            printf("Invalid log level. Use: ERROR, WARN, INFO, DEBUG\n");
            continue;
        }
        entry.level = level;
        
        char* message_start = level_start;
        while (*message_start && !isspace(*message_start)) message_start++;
        while (*message_start && isspace(*message_start)) message_start++;
        
        if (strlen(message_start) > 0) {
            size_t msg_len = strlen(message_start);
            if (msg_len > 0 && message_start[msg_len - 1] == '\n') {
                message_start[msg_len - 1] = '\0';
            }
            strncpy(entry.message, message_start, sizeof(entry.message) - 1);
            entry.message[sizeof(entry.message) - 1] = '\0';
        }
        
        entries[count] = entry;
        count++;
    }
    
    return count;
}

void analyze_logs(const struct log_entry* entries, int count) {
    if (!entries || count <= 0) return;
    
    int level_counts[LOG_LEVELS] = {0};
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    printf("Total entries: %d\n", count);
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d (%.1f%%)\n", 
               level_names[i], 
               level_counts[i],
               count > 0 ? (level_counts[i] * 100.0 / count) : 0.0);
    }
    
    printf("\nRecent entries:\n");
    int show_count = count < 5 ? count : 5;
    for (int i = 0; i < show_count; i++) {
        int idx = count - 1 - i;
        printf("%s [%s] %s\n", 
               entries[idx].timestamp,
               level_names[entries[idx].level],
               entries[idx].message);
    }
}

int main(void) {
    struct log_entry entries[MAX_ENTRIES];
    
    int count = read