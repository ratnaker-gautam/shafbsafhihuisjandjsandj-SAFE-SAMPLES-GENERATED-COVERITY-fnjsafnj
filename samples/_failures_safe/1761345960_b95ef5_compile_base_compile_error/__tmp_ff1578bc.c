//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define IS_VALID_LEVEL(level) \
    (strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 || \
     strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0)

#define PARSE_TIMESTAMP(ts_str, ts_struct) \
    do { \
        if (strptime(ts_str, "%Y-%m-%d %H:%M:%S", &(ts_struct)) == NULL) { \
            return -1; \
        } \
    } while(0)

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[8];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 8];
} LogEntry;

typedef struct {
    LogEntry entries[MAX_ENTRIES];
    int count;
    int level_counts[LOG_LEVELS];
} LogAnalysis;

const char* level_strings[LOG_LEVELS] = {"INFO", "WARN", "ERROR", "DEBUG"};

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return -1;
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return -1;
    strncpy(temp_line, line, MAX_LINE_LEN - 1);
    temp_line[MAX_LINE_LEN - 1] = '\0';
    
    char* timestamp_end = strchr(temp_line, ' ');
    if (timestamp_end == NULL) return -1;
    *timestamp_end = '\0';
    
    if (strlen(temp_line) >= TIMESTAMP_LEN) return -1;
    strncpy(entry->timestamp, temp_line, TIMESTAMP_LEN - 1);
    entry->timestamp[TIMESTAMP_LEN - 1] = '\0';
    
    char* level_start = timestamp_end + 1;
    char* level_end = strchr(level_start, ' ');
    if (level_end == NULL) return -1;
    *level_end = '\0';
    
    if (!IS_VALID_LEVEL(level_start)) return -1;
    strncpy(entry->level, level_start, 7);
    entry->level[7] = '\0';
    
    char* message_start = level_end + 1;
    if (strlen(message_start) >= sizeof(entry->message)) return -1;
    strncpy(entry->message, message_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 0;
}

void init_analysis(LogAnalysis* analysis) {
    if (analysis == NULL) return;
    analysis->count = 0;
    for (int i = 0; i < LOG_LEVELS; i++) {
        analysis->level_counts[i] = 0;
    }
}

int add_log_entry(LogAnalysis* analysis, const LogEntry* entry) {
    if (analysis == NULL || entry == NULL) return -1;
    if (analysis->count >= MAX_ENTRIES) return -1;
    
    analysis->entries[analysis->count] = *entry;
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strcmp(entry->level, level_strings[i]) == 0) {
            analysis->level_counts[i]++;
            break;
        }
    }
    
    analysis->count++;
    return 0;
}

void print_analysis(const LogAnalysis* analysis) {
    if (analysis == NULL) return;
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", analysis->count);
    printf("Level distribution:\n");
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        double percentage = 0.0;
        if (analysis->count > 0) {
            percentage = (double)analysis->level_counts[i] / analysis->count * 100.0;
        }
        printf("  %s: %d (%.1f%%)\n", level_strings[i], analysis->level_counts[i], percentage);
    }
}

int main(void) {
    LogAnalysis analysis;
    init_analysis(&analysis);
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    char line[MAX_LINE_LEN];
    int line_count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL) {
        line_count++;
        
        if (line_count > MAX_ENTRIES) {
            printf("Maximum number of entries reached.\n");
            break;
        }
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(line