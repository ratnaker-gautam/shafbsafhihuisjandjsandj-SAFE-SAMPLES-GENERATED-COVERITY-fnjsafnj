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
    char timestamp[TIMESTAMP_LEN + 1];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
};

static const char *level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};

int parse_log_level(const char *str) {
    if (str == NULL) return -1;
    if (strncmp(str, "ERROR", 5) == 0) return LOG_LEVEL_ERROR;
    if (strncmp(str, "WARN", 4) == 0) return LOG_LEVEL_WARN;
    if (strncmp(str, "INFO", 4) == 0) return LOG_LEVEL_INFO;
    if (strncmp(str, "DEBUG", 5) == 0) return LOG_LEVEL_DEBUG;
    return -1;
}

int parse_timestamp(const char *str, char *timestamp) {
    if (str == NULL || timestamp == NULL) return 0;
    if (strlen(str) < TIMESTAMP_LEN) return 0;
    
    for (int i = 0; i < TIMESTAMP_LEN; i++) {
        char c = str[i];
        if (!(isdigit(c) || c == '-' || c == ':' || c == ' ' || c == '.')) {
            return 0;
        }
    }
    
    strncpy(timestamp, str, TIMESTAMP_LEN);
    timestamp[TIMESTAMP_LEN] = '\0';
    return 1;
}

int read_log_entries(struct log_entry *entries, int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        char timestamp[TIMESTAMP_LEN + 1];
        char level_str[10];
        char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
        
        if (!parse_timestamp(line, timestamp)) continue;
        
        const char *level_start = line + TIMESTAMP_LEN + 1;
        if (level_start >= line + strlen(line)) continue;
        
        int level = parse_log_level(level_start);
        if (!IS_VALID_LEVEL(level)) continue;
        
        const char *message_start = level_start;
        while (*message_start && !isspace(*message_start)) message_start++;
        while (*message_start && isspace(*message_start)) message_start++;
        
        if (message_start >= line + strlen(line)) continue;
        
        size_t msg_len = strlen(message_start);
        if (msg_len >= sizeof(entries[count].message)) {
            msg_len = sizeof(entries[count].message) - 1;
        }
        
        strncpy(entries[count].timestamp, timestamp, TIMESTAMP_LEN);
        entries[count].timestamp[TIMESTAMP_LEN] = '\0';
        entries[count].level = level;
        strncpy(entries[count].message, message_start, msg_len);
        entries[count].message[msg_len] = '\0';
        
        count++;
    }
    
    return count;
}

void analyze_logs(struct log_entry *entries, int count) {
    int level_counts[LOG_LEVELS] = {0};
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("=====================\n");
    printf("Total entries: %d\n", count);
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d (%.1f%%)\n", level_names[i], level_counts[i],
               count > 0 ? (level_counts[i] * 100.0 / count) : 0.0);
    }
    
    if (count > 0) {
        printf("\nRecent entries:\n");
        int show_count = count > 5 ? 5 : count;
        for (int i = count - show_count; i < count; i++) {
            printf("%s [%s] %s", entries[i].timestamp, 
                   level_names[entries[i].level], entries[i].message);
        }
    }
}

int main(void) {
    struct log_entry entries[MAX_ENTRIES];
    
    printf("Enter log entries (timestamp level message):\n");
    printf("Format: YYYY-MM-DD HH:MM:SS LEVEL message\\n\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to finish input.\n\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");