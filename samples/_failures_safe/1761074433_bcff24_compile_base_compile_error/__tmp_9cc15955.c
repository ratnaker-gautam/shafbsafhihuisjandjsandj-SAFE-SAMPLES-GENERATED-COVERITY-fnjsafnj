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

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 3];
} LogEntry;

int parse_timestamp(const char* str, char* timestamp) {
    if (!str || !timestamp) return 0;
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

int parse_log_entry(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
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
    if (message_len == 0 || message_len >= sizeof(entry->message)) return 0;
    
    strncpy(entry->message, message_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void generate_sample_logs(void) {
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
    
    FILE* file = fopen("sample.log", "w");
    if (!file) return;
    
    time_t now = time(NULL);
    struct tm tm_info;
    localtime_r(&now, &tm_info);
    
    for (int i = 0; i < 50; i++) {
        tm_info.tm_sec = (tm_info.tm_sec + 1) % 60;
        char timestamp[TIMESTAMP_LEN];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &tm_info);
        
        char level = levels[rand() % LOG_LEVELS];
        const char* message = messages[rand() % 10];
        
        fprintf(file, "%s [%c] %s\n", timestamp, level, message);
    }
    
    fclose(file);
}

int main(void) {
    printf("Log Analyzer\n");
    printf("Generating sample log file...\n");
    
    srand((unsigned int)time(NULL));
    generate_sample_logs();
    
    FILE* file = fopen("sample.log", "r");
    if (!file) {
        printf("Error: Cannot open log file\n");
        return 1;
    }
    
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int level_counts[LOG_LEVELS] = {0};
    char line[MAX_LINE_LEN];
    
    while (fgets(line, sizeof(line), file) && entry_count < MAX_ENTRIES) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) continue;
        
        LogEntry entry;
        if (parse_log_entry(line, &entry)) {
            if (entry_count < MAX_