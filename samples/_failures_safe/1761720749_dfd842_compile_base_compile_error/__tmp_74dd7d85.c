//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define MAX_LEVEL_LEN 10
#define TIMESTAMP_LEN 20

#define LEVEL_ERROR "ERROR"
#define LEVEL_WARN  "WARN"
#define LEVEL_INFO  "INFO"
#define LEVEL_DEBUG "DEBUG"

#define IS_VALID_LEVEL(level) \
    (strcmp(level, LEVEL_ERROR) == 0 || strcmp(level, LEVEL_WARN) == 0 || \
     strcmp(level, LEVEL_INFO) == 0 || strcmp(level, LEVEL_DEBUG) == 0)

typedef struct {
    char timestamp[TIMESTAMP_LEN + 1];
    char level[MAX_LEVEL_LEN + 1];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - MAX_LEVEL_LEN - 3];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    size_t len = strlen(line);
    if (len < TIMESTAMP_LEN + MAX_LEVEL_LEN + 3 || len >= MAX_LINE_LEN) return 0;
    
    if (line[TIMESTAMP_LEN] != ' ' || line[TIMESTAMP_LEN + 1] != '[') return 0;
    
    strncpy(entry->timestamp, line, TIMESTAMP_LEN);
    entry->timestamp[TIMESTAMP_LEN] = '\0';
    
    const char* level_start = line + TIMESTAMP_LEN + 2;
    const char* level_end = strchr(level_start, ']');
    if (!level_end) return 0;
    
    size_t level_len = level_end - level_start;
    if (level_len == 0 || level_len > MAX_LEVEL_LEN) return 0;
    
    strncpy(entry->level, level_start, level_len);
    entry->level[level_len] = '\0';
    
    if (!IS_VALID_LEVEL(entry->level)) return 0;
    
    const char* message_start = level_end + 1;
    if (*message_start != ' ') return 0;
    message_start++;
    
    size_t message_len = strlen(message_start);
    if (message_len == 0 || message_len >= sizeof(entry->message)) return 0;
    
    strncpy(entry->message, message_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void generate_sample_logs(void) {
    const char* levels[] = {LEVEL_ERROR, LEVEL_WARN, LEVEL_INFO, LEVEL_DEBUG};
    const char* messages[] = {
        "Database connection failed",
        "High memory usage detected",
        "User login successful",
        "Starting backup process",
        "Invalid input received",
        "Cache cleared successfully"
    };
    
    time_t now = time(NULL);
    struct tm tm_info;
    if (localtime_r(&now, &tm_info) == NULL) return;
    
    for (int i = 0; i < 10; i++) {
        char timestamp[TIMESTAMP_LEN + 1];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &tm_info);
        
        int level_idx = rand() % 4;
        int msg_idx = rand() % 6;
        
        printf("%s [%s] %s\n", timestamp, levels[level_idx], messages[msg_idx]);
        
        tm_info.tm_sec += rand() % 60 + 1;
        mktime(&tm_info);
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int level_counts[4] = {0};
    
    printf("Log Analyzer - Processing log entries...\n\n");
    
    char line[MAX_LINE_LEN];
    int line_num = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && entry_count < MAX_ENTRIES) {
        line_num++;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (len == 0 || len == 1) continue;
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
            
            if (strcmp(entry.level, LEVEL_ERROR) == 0) level_counts[0]++;
            else if (strcmp(entry.level, LEVEL_WARN) == 0) level_counts[1]++;
            else if (strcmp(entry.level, LEVEL_INFO) == 0) level_counts[2]++;
            else if (strcmp(entry.level, LEVEL_DEBUG) == 0) level_counts[3]++;
        } else {
            fprintf(stderr, "Invalid log format at line %d\n", line_num);
        }
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("ERROR entries: %d\n", level_counts[0