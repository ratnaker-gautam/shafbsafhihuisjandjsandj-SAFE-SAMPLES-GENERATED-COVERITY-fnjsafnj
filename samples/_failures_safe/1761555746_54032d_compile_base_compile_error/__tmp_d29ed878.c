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

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARN 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3

#define IS_VALID_LEVEL(c) ((c) >= 'A' && (c) <= 'Z')
#define LEVEL_TO_IDX(c) ((c) == 'E' ? LOG_LEVEL_ERROR : \
                        (c) == 'W' ? LOG_LEVEL_WARN : \
                        (c) == 'I' ? LOG_LEVEL_INFO : \
                        (c) == 'D' ? LOG_LEVEL_DEBUG : -1)

struct log_entry {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
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

int parse_log_line(const char* line, struct log_entry* entry) {
    if (strlen(line) >= MAX_LINE_LEN - 1) return 0;
    
    char temp[MAX_LINE_LEN];
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    if (!parse_timestamp(temp, entry->timestamp)) return 0;
    
    if (strlen(temp) < TIMESTAMP_LEN + 3) return 0;
    const char* level_start = temp + TIMESTAMP_LEN + 1;
    
    if (!IS_VALID_LEVEL(level_start[0]) || level_start[1] != ' ') return 0;
    
    entry->level = LEVEL_TO_IDX(level_start[0]);
    if (entry->level == -1) return 0;
    
    const char* message_start = level_start + 2;
    if (strlen(message_start) == 0) return 0;
    
    strncpy(entry->message, message_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void print_summary(int counts[LOG_LEVELS]) {
    const char* level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};
    printf("\nLog Level Summary:\n");
    printf("==================\n");
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%-6s: %d\n", level_names[i], counts[i]);
    }
}

int main(void) {
    struct log_entry entries[MAX_ENTRIES];
    int entry_count = 0;
    int level_counts[LOG_LEVELS] = {0};
    
    printf("Log Analyzer - Enter log entries (empty line to finish):\n");
    printf("Format: YYYY-MM-DD HH:MM:SS LEVEL message\n");
    
    char line[MAX_LINE_LEN];
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            level_counts[entries[entry_count].level]++;
            entry_count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    print_summary(level_counts);
    
    printf("\nRecent log entries:\n");
    printf("===================\n");
    int display_count = (entry_count > 10) ? 10 : entry_count;
    for (int i = entry_count - display_count; i < entry_count; i++) {
        const char* level_str = "UNKNOWN";
        switch (entries[i].level) {
            case LOG_LEVEL_ERROR: level_str = "ERROR"; break;
            case LOG_LEVEL_WARN: level_str = "WARN"; break;
            case LOG_LEVEL_INFO: level_str = "INFO"; break;
            case LOG_LEVEL_DEBUG: level_str = "DEBUG"; break;
        }
        printf("%s %s %s\n", entries[i].timestamp, level_str,