//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 19
#define LOG_LEVELS 4

#define LOG_INFO 0
#define LOG_WARN 1
#define LOG_ERROR 2
#define LOG_DEBUG 3

#define IS_VALID_LEVEL(l) ((l) >= 0 && (l) < LOG_LEVELS)

struct LogEntry {
    char timestamp[TIMESTAMP_LEN + 1];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
};

const char* level_names[] = {"INFO", "WARN", "ERROR", "DEBUG"};

int parse_log_level(const char* str) {
    if (str == NULL) return -1;
    if (strncmp(str, "INFO", 4) == 0) return LOG_INFO;
    if (strncmp(str, "WARN", 4) == 0) return LOG_WARN;
    if (strncmp(str, "ERROR", 5) == 0) return LOG_ERROR;
    if (strncmp(str, "DEBUG", 5) == 0) return LOG_DEBUG;
    return -1;
}

int validate_timestamp(const char* ts) {
    if (strlen(ts) != TIMESTAMP_LEN) return 0;
    for (int i = 0; i < TIMESTAMP_LEN; i++) {
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

int read_log_entries(struct LogEntry* entries, int max_entries) {
    int count = 0;
    char line[MAX_LINE_LEN];
    
    while (count < max_entries && fgets(line, sizeof(line), stdin) != NULL) {
        size_t len = strlen(line);
        if (len == 0 || line[0] == '\n') continue;
        
        struct LogEntry entry;
        memset(&entry, 0, sizeof(entry));
        
        if (len < TIMESTAMP_LEN + 6) continue;
        
        strncpy(entry.timestamp, line, TIMESTAMP_LEN);
        entry.timestamp[TIMESTAMP_LEN] = '\0';
        
        if (!validate_timestamp(entry.timestamp)) continue;
        
        const char* level_start = line + TIMESTAMP_LEN + 1;
        if (level_start >= line + len) continue;
        
        entry.level = parse_log_level(level_start);
        if (!IS_VALID_LEVEL(entry.level)) continue;
        
        const char* message_start = level_start;
        while (*message_start && *message_start != ' ') message_start++;
        while (*message_start && *message_start == ' ') message_start++;
        
        if (message_start < line + len && *message_start != '\0') {
            size_t msg_len = strlen(message_start);
            if (msg_len > 0 && message_start[msg_len - 1] == '\n') {
                msg_len--;
            }
            if (msg_len > sizeof(entry.message) - 1) {
                msg_len = sizeof(entry.message) - 1;
            }
            strncpy(entry.message, message_start, msg_len);
            entry.message[msg_len] = '\0';
        }
        
        entries[count] = entry;
        count++;
    }
    
    return count;
}

void generate_summary(const struct LogEntry* entries, int count) {
    int level_counts[LOG_LEVELS] = {0};
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("Log Summary:\n");
    printf("Total entries: %d\n", count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", level_names[i], level_counts[i]);
    }
    printf("\n");
}

void show_recent_entries(const struct LogEntry* entries, int count, int num_recent) {
    if (num_recent <= 0 || num_recent > count) {
        num_recent = count;
    }
    
    printf("Recent %d entries:\n", num_recent);
    int start = count - num_recent;
    if (start < 0) start = 0;
    
    for (int i = start; i < count; i++) {
        printf("[%s] %s: %s\n", entries[i].timestamp, 
               level_names[entries[i].level], entries[i].message);
    }
    printf("\n");
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Supported levels: INFO, WARN, ERROR, DEBUG\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to finish input\n