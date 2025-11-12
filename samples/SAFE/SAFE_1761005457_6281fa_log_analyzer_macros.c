//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARN 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3

#define IS_VALID_LEVEL(c) ((c) == 'E' || (c) == 'W' || (c) == 'I' || (c) == 'D')
#define LEVEL_TO_INDEX(c) ((c) == 'E' ? 0 : (c) == 'W' ? 1 : (c) == 'I' ? 2 : 3)

struct log_entry {
    char timestamp[TIMESTAMP_LEN];
    char level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 4];
};

int parse_timestamp(const char* str, char* timestamp) {
    if (strlen(str) < TIMESTAMP_LEN - 1) return 0;
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (i == 4 || i == 7) {
            if (str[i] != '-') return 0;
        } else if (i == 10) {
            if (str[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (str[i] != ':') return 0;
        } else {
            if (!isdigit(str[i])) return 0;
        }
        timestamp[i] = str[i];
    }
    timestamp[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int read_log_entries(struct log_entry* entries, int max_entries) {
    int count = 0;
    char line[MAX_LINE_LEN];
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        char timestamp[TIMESTAMP_LEN];
        if (!parse_timestamp(line, timestamp)) {
            continue;
        }
        
        if (strlen(line) < TIMESTAMP_LEN + 3) continue;
        
        char level_char = line[TIMESTAMP_LEN + 1];
        if (!IS_VALID_LEVEL(level_char)) continue;
        
        strncpy(entries[count].timestamp, timestamp, TIMESTAMP_LEN - 1);
        entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
        entries[count].level = level_char;
        
        const char* msg_start = line + TIMESTAMP_LEN + 3;
        size_t msg_len = strlen(msg_start);
        if (msg_len > 0 && msg_start[msg_len - 1] == '\n') {
            msg_len--;
        }
        if (msg_len >= sizeof(entries[count].message)) {
            msg_len = sizeof(entries[count].message) - 1;
        }
        strncpy(entries[count].message, msg_start, msg_len);
        entries[count].message[msg_len] = '\0';
        
        count++;
    }
    
    return count;
}

void analyze_logs(struct log_entry* entries, int count) {
    int level_counts[LOG_LEVELS] = {0};
    char level_names[LOG_LEVELS] = {'E', 'W', 'I', 'D'};
    char* level_descriptions[LOG_LEVELS] = {"Error", "Warning", "Info", "Debug"};
    
    for (int i = 0; i < count; i++) {
        int level_idx = LEVEL_TO_INDEX(entries[i].level);
        if (level_idx >= 0 && level_idx < LOG_LEVELS) {
            level_counts[level_idx]++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("====================\n");
    printf("Total entries: %d\n", count);
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s (%c): %d entries\n", level_descriptions[i], level_names[i], level_counts[i]);
    }
    
    if (count > 0) {
        printf("\nRecent entries:\n");
        int show_count = count > 5 ? 5 : count;
        for (int i = count - show_count; i < count; i++) {
            printf("%s [%c] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
}

int main(void) {
    struct log_entry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS [LEVEL] message):\n");
    printf("Press Ctrl+D (Linux) or Ctrl+Z (Windows) to finish input.\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    analyze_logs(entries, count);
    
    return 0;
}