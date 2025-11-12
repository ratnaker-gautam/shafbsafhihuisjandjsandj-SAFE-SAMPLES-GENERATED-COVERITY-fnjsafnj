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

#define IS_VALID_LEVEL(l) ((l) >= 0 && (l) < LOG_LEVELS)
#define LEVEL_NAME(l) (level_names[(l)])
#define IS_EMPTY_LINE(s) ((s)[0] == '\0' || (s)[0] == '\n')

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

static const char *level_names[] = {"DEBUG", "INFO", "WARN", "ERROR"};

int parse_timestamp(const char *str, char *timestamp) {
    if (str == NULL || timestamp == NULL) return 0;
    if (strlen(str) < TIMESTAMP_LEN - 1) return 0;
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (str[i] == ' ') break;
        timestamp[i] = str[i];
    }
    timestamp[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int parse_level(const char *str) {
    if (str == NULL) return -1;
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strstr(str, level_names[i]) != NULL) return i;
    }
    return -1;
}

int parse_log_line(const char *line, LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    if (IS_EMPTY_LINE(line)) return 0;
    
    char temp[MAX_LINE_LEN];
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    if (!parse_timestamp(temp, entry->timestamp)) return 0;
    
    char *level_start = strchr(temp, ']');
    if (level_start == NULL) return 0;
    level_start++;
    
    int level = parse_level(level_start);
    if (!IS_VALID_LEVEL(level)) return 0;
    entry->level = level;
    
    char *msg_start = strchr(level_start, ':');
    if (msg_start == NULL) return 0;
    msg_start += 2;
    
    strncpy(entry->message, msg_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    char *newline = strchr(entry->message, '\n');
    if (newline) *newline = '\0';
    
    return 1;
}

void generate_sample_logs(LogEntry logs[], int count) {
    if (logs == NULL || count <= 0 || count > MAX_ENTRIES) return;
    
    const char *messages[] = {
        "System startup completed",
        "User login successful",
        "Database connection established",
        "Cache cleared",
        "Backup initiated",
        "Security scan started",
        "Configuration updated",
        "Memory usage high",
        "Disk space low",
        "Network latency detected"
    };
    int msg_count = sizeof(messages) / sizeof(messages[0]);
    
    time_t now = time(NULL);
    for (int i = 0; i < count && i < MAX_ENTRIES; i++) {
        struct tm *tm_info = localtime(&now);
        if (tm_info != NULL) {
            strftime(logs[i].timestamp, TIMESTAMP_LEN, "%Y-%m-%d %H:%M:%S", tm_info);
        }
        logs[i].level = rand() % LOG_LEVELS;
        strncpy(logs[i].message, messages[rand() % msg_count], 
                sizeof(logs[i].message) - 1);
        logs[i].message[sizeof(logs[i].message) - 1] = '\0';
        now -= rand() % 3600;
    }
}

void analyze_logs(LogEntry logs[], int count) {
    if (logs == NULL || count <= 0) return;
    
    int level_counts[LOG_LEVELS] = {0};
    int total_messages = 0;
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(logs[i].level)) {
            level_counts[logs[i].level]++;
            total_messages++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", total_messages);
    for (int i = 0; i < LOG_LEVELS; i++) {
        float percentage = total_messages > 0 ? 
            (level_counts[i] * 100.0f) / total_messages : 0.0f;
        printf("%s: %d (%.1f%%)\n", LEVEL_NAME(i), level_counts[i], percentage);
    }
}

void display_recent_logs(LogEntry logs[], int count, int max_display) {
    if (logs == NULL || count <= 0 || max_display <= 0) return;
    
    printf("\nRecent log entries (showing %d):\n", max_display