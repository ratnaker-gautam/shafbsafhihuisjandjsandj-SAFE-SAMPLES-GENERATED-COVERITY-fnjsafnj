//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define LOG_INFO 0
#define LOG_WARN 1
#define LOG_ERROR 2
#define LOG_DEBUG 3

#define LEVEL_STR(level) \
    (level == LOG_INFO ? "INFO" : \
     level == LOG_WARN ? "WARN" : \
     level == LOG_ERROR ? "ERROR" : "DEBUG")

struct log_entry {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
};

int parse_log_level(const char* str) {
    if (strncmp(str, "INFO", 4) == 0) return LOG_INFO;
    if (strncmp(str, "WARN", 4) == 0) return LOG_WARN;
    if (strncmp(str, "ERROR", 5) == 0) return LOG_ERROR;
    if (strncmp(str, "DEBUG", 5) == 0) return LOG_DEBUG;
    return -1;
}

int parse_timestamp(const char* str, char* timestamp) {
    if (strlen(str) < TIMESTAMP_LEN - 1) return 0;
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (str[i] == ' ') break;
        if (!isdigit(str[i]) && str[i] != '-' && str[i] != ':' && str[i] != '.') return 0;
        timestamp[i] = str[i];
    }
    timestamp[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int read_log_entries(struct log_entry* entries, int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter empty line to finish.\n");
    
    while (count < max_entries && fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\n' || line[0] == '\0') break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) >= MAX_LINE_LEN - 1) {
            printf("Line too long, skipping.\n");
            continue;
        }
        
        struct log_entry entry;
        memset(&entry, 0, sizeof(entry));
        
        if (!parse_timestamp(line, entry.timestamp)) {
            printf("Invalid timestamp format, skipping.\n");
            continue;
        }
        
        const char* level_start = strchr(line, ' ');
        if (!level_start) {
            printf("Missing log level, skipping.\n");
            continue;
        }
        level_start++;
        
        const char* message_start = strchr(level_start, ' ');
        if (!message_start) {
            printf("Missing message, skipping.\n");
            continue;
        }
        message_start++;
        
        char level_str[10];
        int level_len = message_start - level_start - 1;
        if (level_len <= 0 || level_len >= (int)sizeof(level_str)) {
            printf("Invalid log level, skipping.\n");
            continue;
        }
        
        strncpy(level_str, level_start, level_len);
        level_str[level_len] = '\0';
        
        entry.level = parse_log_level(level_str);
        if (entry.level == -1) {
            printf("Unknown log level, skipping.\n");
            continue;
        }
        
        size_t message_len = strlen(message_start);
        if (message_len >= sizeof(entry.message)) {
            printf("Message too long, truncating.\n");
            message_len = sizeof(entry.message) - 1;
        }
        strncpy(entry.message, message_start, message_len);
        entry.message[message_len] = '\0';
        
        entries[count++] = entry;
    }
    
    return count;
}

void analyze_logs(struct log_entry* entries, int count) {
    int level_counts[LOG_LEVELS] = {0};
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level >= 0 && entries[i].level < LOG_LEVELS) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", count);
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d (%.1f%%)\n", 
               LEVEL_STR(i), 
               level_counts[i],
               count > 0 ? (level_counts[i] * 100.0 / count) : 0.0);
    }
    
    if (count > 0) {
        printf("\nRecent entries:\n");
        int show_count = count > 5 ? 5 : count;
        for (int i = count - show_count; i < count; i++) {
            printf("%s [%s] %s\n", 
                   entries[i].timestamp, 
                   LEVEL_STR(entries[i].level),
                   entries[i].message);
        }
    }
}

int main(void) {
    struct log_entry entries[MAX_ENTRI