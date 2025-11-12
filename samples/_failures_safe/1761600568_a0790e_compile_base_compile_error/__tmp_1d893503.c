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

#define LOG_LEVEL_ERROR "ERROR"
#define LOG_LEVEL_WARN "WARN"
#define LOG_LEVEL_INFO "INFO"
#define LOG_LEVEL_DEBUG "DEBUG"

#define IS_VALID_LEVEL(level) \
    (strcmp(level, LOG_LEVEL_ERROR) == 0 || \
     strcmp(level, LOG_LEVEL_WARN) == 0 || \
     strcmp(level, LOG_LEVEL_INFO) == 0 || \
     strcmp(level, LOG_LEVEL_DEBUG) == 0)

typedef struct {
    char timestamp[TIMESTAMP_LEN + 1];
    char level[MAX_LEVEL_LEN + 1];
    char message[MAX_LINE_LEN + 1];
} LogEntry;

int parse_timestamp(const char* str, char* timestamp) {
    if (str == NULL || timestamp == NULL) return 0;
    if (strlen(str) < TIMESTAMP_LEN) return 0;
    
    for (int i = 0; i < TIMESTAMP_LEN; i++) {
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
    }
    
    strncpy(timestamp, str, TIMESTAMP_LEN);
    timestamp[TIMESTAMP_LEN] = '\0';
    return 1;
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    
    char temp[MAX_LINE_LEN + 1];
    strncpy(temp, line, MAX_LINE_LEN);
    temp[MAX_LINE_LEN] = '\0';
    
    char* timestamp_part = strtok(temp, " ");
    if (timestamp_part == NULL) return 0;
    
    if (!parse_timestamp(timestamp_part, entry->timestamp)) return 0;
    
    char* level_part = strtok(NULL, " ");
    if (level_part == NULL) return 0;
    
    if (!IS_VALID_LEVEL(level_part)) return 0;
    strncpy(entry->level, level_part, MAX_LEVEL_LEN);
    entry->level[MAX_LEVEL_LEN] = '\0';
    
    char* message_part = strtok(NULL, "\n");
    if (message_part == NULL) return 0;
    
    strncpy(entry->message, message_part, MAX_LINE_LEN);
    entry->message[MAX_LINE_LEN] = '\0';
    
    return 1;
}

void analyze_logs(LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int error_count = 0;
    int warn_count = 0;
    int info_count = 0;
    int debug_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, LOG_LEVEL_ERROR) == 0) error_count++;
        else if (strcmp(entries[i].level, LOG_LEVEL_WARN) == 0) warn_count++;
        else if (strcmp(entries[i].level, LOG_LEVEL_INFO) == 0) info_count++;
        else if (strcmp(entries[i].level, LOG_LEVEL_DEBUG) == 0) debug_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("ERROR: %d\n", error_count);
    printf("WARN:  %d\n", warn_count);
    printf("INFO:  %d\n", info_count);
    printf("DEBUG: %d\n", debug_count);
    
    if (count > 0) {
        printf("\nRecent entries:\n");
        int show_count = (count < 3) ? count : 3;
        for (int i = count - show_count; i < count; i++) {
            printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer - Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Supported levels: ERROR, WARN, INFO, DEBUG\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    char line[MAX_LINE_LEN + 1];
    
    while (entry_count < MAX_ENTRIES) {
        printf("> ");
        
        if (fgets(line, MAX_LINE_LEN, stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) {
            continue;