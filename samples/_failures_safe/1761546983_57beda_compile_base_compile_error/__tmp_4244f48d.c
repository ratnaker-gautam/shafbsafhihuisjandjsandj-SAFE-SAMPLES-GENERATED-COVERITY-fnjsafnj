//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

const char* level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};

int parse_log_level(const char* str) {
    if (str == NULL) return -1;
    if (strncasecmp(str, "ERROR", 5) == 0) return LOG_LEVEL_ERROR;
    if (strncasecmp(str, "WARN", 4) == 0) return LOG_LEVEL_WARN;
    if (strncasecmp(str, "INFO", 4) == 0) return LOG_LEVEL_INFO;
    if (strncasecmp(str, "DEBUG", 5) == 0) return LOG_LEVEL_DEBUG;
    return -1;
}

int parse_timestamp(const char* str, char* timestamp) {
    if (str == NULL || timestamp == NULL) return 0;
    if (strlen(str) < TIMESTAMP_LEN - 1) return 0;
    
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (str[i] == '\0') return 0;
        timestamp[i] = str[i];
    }
    timestamp[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int read_log_entry(FILE* file, LogEntry* entry) {
    if (file == NULL || entry == NULL) return 0;
    
    char line[MAX_LINE_LEN];
    if (fgets(line, sizeof(line), file) == NULL) return 0;
    
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';
    
    if (strcmp(line, "END") == 0) return -1;
    
    char timestamp[TIMESTAMP_LEN];
    char level_str[16];
    char message[MAX_LINE_LEN];
    
    if (sscanf(line, "%19s %15s %1023[^\n]", timestamp, level_str, message) != 3) {
        return 0;
    }
    
    if (!parse_timestamp(timestamp, entry->timestamp)) return 0;
    
    int level = parse_log_level(level_str);
    if (!IS_VALID_LEVEL(level)) return 0;
    entry->level = level;
    
    if (strlen(message) >= sizeof(entry->message)) return 0;
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void analyze_logs(LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int level_counts[LOG_LEVELS] = {0};
    char earliest[TIMESTAMP_LEN] = "";
    char latest[TIMESTAMP_LEN] = "";
    
    for (int i = 0; i < count; i++) {
        if (!IS_VALID_LEVEL(entries[i].level)) continue;
        
        level_counts[entries[i].level]++;
        
        if (i == 0) {
            strncpy(earliest, entries[i].timestamp, sizeof(earliest) - 1);
            earliest[sizeof(earliest) - 1] = '\0';
            strncpy(latest, entries[i].timestamp, sizeof(latest) - 1);
            latest[sizeof(latest) - 1] = '\0';
        } else {
            if (strcmp(entries[i].timestamp, earliest) < 0) {
                strncpy(earliest, entries[i].timestamp, sizeof(earliest) - 1);
                earliest[sizeof(earliest) - 1] = '\0';
            }
            if (strcmp(entries[i].timestamp, latest) > 0) {
                strncpy(latest, entries[i].timestamp, sizeof(latest) - 1);
                latest[sizeof(latest) - 1] = '\0';
            }
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("Time range: %s to %s\n", earliest, latest);
    printf("Total entries: %d\n", count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", level_names[i], level_counts[i]);
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < MAX_ENTRIES) {
        printf("Log entry %d: ",