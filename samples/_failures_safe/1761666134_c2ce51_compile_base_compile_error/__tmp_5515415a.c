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

#define IS_VALID_LEVEL(l) ((l) >= 0 && (l) < LOG_LEVELS)
#define LEVEL_NAME(l) (level_names[(l)])

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

static const char *level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};
static int level_counts[LOG_LEVELS] = {0};

int parse_timestamp(const char *str, char *timestamp) {
    if (str == NULL || timestamp == NULL) return 0;
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

int parse_log_level(const char *str) {
    if (str == NULL) return -1;
    
    if (strncmp(str, "[ERROR]", 7) == 0) return LOG_LEVEL_ERROR;
    if (strncmp(str, "[WARN]", 6) == 0) return LOG_LEVEL_WARN;
    if (strncmp(str, "[INFO]", 6) == 0) return LOG_LEVEL_INFO;
    if (strncmp(str, "[DEBUG]", 7) == 0) return LOG_LEVEL_DEBUG;
    
    return -1;
}

int parse_log_line(const char *line, LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    
    char temp[MAX_LINE_LEN];
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    char *timestamp_part = strtok(temp, " ");
    if (timestamp_part == NULL) return 0;
    
    if (!parse_timestamp(timestamp_part, entry->timestamp)) return 0;
    
    char *level_part = strtok(NULL, " ");
    if (level_part == NULL) return 0;
    
    int level = parse_log_level(level_part);
    if (!IS_VALID_LEVEL(level)) return 0;
    entry->level = level;
    
    char *message_part = strtok(NULL, "\n");
    if (message_part == NULL) return 0;
    
    strncpy(entry->message, message_part, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void generate_sample_logs(void) {
    printf("2024-01-15 10:30:25 [INFO] Application started successfully\n");
    printf("2024-01-15 10:31:10 [DEBUG] Loading configuration file\n");
    printf("2024-01-15 10:31:15 [WARN] Configuration value missing, using default\n");
    printf("2024-01-15 10:32:00 [INFO] Database connection established\n");
    printf("2024-01-15 10:33:45 [ERROR] Failed to process user request\n");
    printf("2024-01-15 10:34:20 [DEBUG] Memory usage: 45MB\n");
    printf("2024-01-15 10:35:10 [INFO] User login successful\n");
    printf("2024-01-15 10:36:05 [WARN] High CPU usage detected\n");
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Log Analyzer - Enter log lines (empty line to finish):\n");
    printf("Example format: YYYY-MM-DD HH:MM:SS [LEVEL] message\n");
    
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
        } else