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

#define IS_VALID_LEVEL(c) ((c) == 'E' || (c) == 'W' || (c) == 'I' || (c) == 'D')
#define LEVEL_TO_IDX(c) ((c) == 'E' ? 0 : (c) == 'W' ? 1 : (c) == 'I' ? 2 : 3)

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 4];
} LogEntry;

int parse_timestamp(const char* str, char* timestamp) {
    if (str == NULL || timestamp == NULL) return 0;
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

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    size_t line_len = strlen(line);
    if (line_len >= MAX_LINE_LEN - 1) return 0;
    
    if (!parse_timestamp(line, entry->timestamp)) return 0;
    
    if (line_len < TIMESTAMP_LEN + 5) return 0;
    if (line[TIMESTAMP_LEN] != ' ' || line[TIMESTAMP_LEN + 1] != '[') return 0;
    
    char level_char = line[TIMESTAMP_LEN + 2];
    if (!IS_VALID_LEVEL(level_char)) return 0;
    entry->level = level_char;
    
    if (line[TIMESTAMP_LEN + 3] != ']' || line[TIMESTAMP_LEN + 4] != ' ') return 0;
    
    const char* message_start = line + TIMESTAMP_LEN + 5;
    size_t message_len = strlen(message_start);
    if (message_len >= sizeof(entry->message)) return 0;
    
    strncpy(entry->message, message_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void generate_sample_logs(LogEntry* logs, int* count) {
    if (logs == NULL || count == NULL) return;
    
    const char* levels = "EWID";
    const char* messages[] = {
        "System startup completed",
        "Database connection established",
        "User authentication failed",
        "Disk space running low",
        "Network latency detected",
        "Backup process started",
        "Cache cleared successfully",
        "Invalid input received",
        "Service restart required",
        "Configuration updated"
    };
    
    time_t now = time(NULL);
    struct tm tm_info;
    localtime_r(&now, &tm_info);
    
    *count = 0;
    for (int i = 0; i < 20 && *count < MAX_ENTRIES; i++) {
        LogEntry* entry = &logs[*count];
        
        tm_info.tm_sec = (i * 3) % 60;
        tm_info.tm_min = (i * 7) % 60;
        tm_info.tm_hour = (i * 2) % 24;
        
        strftime(entry->timestamp, sizeof(entry->timestamp), 
                "%Y-%m-%d %H:%M:%S", &tm_info);
        
        entry->level = levels[i % LOG_LEVELS];
        strncpy(entry->message, messages[i % 10], sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
        
        (*count)++;
    }
}

void analyze_logs(LogEntry* logs, int count) {
    if (logs == NULL || count <= 0 || count > MAX_ENTRIES) return;
    
    int level_counts[LOG_LEVELS] = {0};
    char level_names[LOG_LEVELS] = {'E', 'W', 'I', 'D'};
    char* level_descriptions[LOG_LEVELS] = {
        "Error", "Warning", "Info", "Debug"
    };
    
    for (int i = 0; i < count; i++) {
        int idx = LEVEL_TO_IDX(logs[i].level);
        if (idx >= 0 && idx < LOG_LEVELS) {