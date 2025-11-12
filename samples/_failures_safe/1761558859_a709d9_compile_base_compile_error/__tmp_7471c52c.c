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
    size_t len = strlen(str);
    if (len < TIMESTAMP_LEN - 1) return 0;
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

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    size_t line_len = strlen(line);
    if (line_len >= MAX_LINE_LEN - 1) return 0;
    
    if (!parse_timestamp(line, entry->timestamp)) return 0;
    
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
        "User login successful",
        "Database connection established",
        "Cache cleared",
        "Backup initiated",
        "Security scan running",
        "Memory usage high",
        "Disk space low",
        "Network timeout",
        "Authentication failed"
    };
    const int num_messages = sizeof(messages) / sizeof(messages[0]);
    
    time_t now = time(NULL);
    struct tm tm_info_buf;
    struct tm* tm_info = localtime(&now);
    if (tm_info == NULL) return;
    
    tm_info_buf = *tm_info;
    
    *count = 0;
    for (int i = 0; i < 20 && *count < MAX_ENTRIES; i++) {
        LogEntry* entry = &logs[*count];
        
        struct tm temp_tm = tm_info_buf;
        temp_tm.tm_sec = (i * 3) % 60;
        temp_tm.tm_min = (i * 7) % 60;
        temp_tm.tm_hour = (i * 2) % 24;
        temp_tm.tm_mday = 10 + (i % 5);
        
        if (strftime(entry->timestamp, TIMESTAMP_LEN, "%Y-%m-%d %H:%M:%S", &temp_tm) == 0) {
            continue;
        }
        entry->level = levels[i % LOG_LEVELS];
        strncpy(entry->message, messages[i % num_messages], sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
        
        (*count)++;
    }
}

void analyze_logs(LogEntry* logs, int count) {
    if (logs == NULL || count <= 0) return;
    
    int level_counts[LOG_LEVELS] = {0};
    char level_names[LOG_LEVELS] = {'E', 'W', 'I', 'D'};
    const char* level_descriptions[LOG_LEVELS