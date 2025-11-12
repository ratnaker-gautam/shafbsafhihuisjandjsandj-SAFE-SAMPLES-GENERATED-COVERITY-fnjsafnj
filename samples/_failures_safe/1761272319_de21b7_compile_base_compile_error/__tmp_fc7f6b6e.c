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
    
    if (strncmp(str, "ERROR", 5) == 0) return LOG_LEVEL_ERROR;
    if (strncmp(str, "WARN", 4) == 0) return LOG_LEVEL_WARN;
    if (strncmp(str, "INFO", 4) == 0) return LOG_LEVEL_INFO;
    if (strncmp(str, "DEBUG", 5) == 0) return LOG_LEVEL_DEBUG;
    
    return -1;
}

int parse_log_line(const char *line, LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    
    char temp[MAX_LINE_LEN];
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    if (!parse_timestamp(temp, entry->timestamp)) return 0;
    
    char *level_start = temp + TIMESTAMP_LEN;
    while (*level_start == ' ' && level_start < temp + strlen(temp)) level_start++;
    
    int level = parse_log_level(level_start);
    if (!IS_VALID_LEVEL(level)) return 0;
    entry->level = level;
    
    char *message_start = level_start;
    while (*message_start && *message_start != ' ' && *message_start != ']') message_start++;
    while (*message_start && (*message_start == ' ' || *message_start == ']' || *message_start == '[')) message_start++;
    
    if (strlen(message_start) == 0) return 0;
    
    strncpy(entry->message, message_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    char *newline = strchr(entry->message, '\n');
    if (newline) *newline = '\0';
    
    return 1;
}

void print_summary(void) {
    printf("Log Level Summary:\n");
    printf("==================\n");
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%-6s: %d\n", LEVEL_NAME(i), level_counts[i]);
    }
    printf("Total  : %d\n", level_counts[0] + level_counts[1] + level_counts[2] + level_counts[3]);
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (empty line to finish):\n");
    printf("Format: YYYY-MM-DD HH:MM:SS [LEVEL] message\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        if (line[0] == '\n' || line[0] == '\0') break;
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            level_counts[entry.level]++;
            entry_count++;
        } else {
            printf("Invalid log format. Skipping: %s", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("\nParsed %d log entries:\n", entry_count);
    for (