//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000
#define MAX_LEVEL_LENGTH 16

struct LogEntry {
    char timestamp[32];
    char level[MAX_LEVEL_LENGTH];
    char message[256];
    int severity;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    int count;
    int error_count;
    int warning_count;
    int info_count;
    time_t start_time;
    time_t end_time;
};

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
    analyzer->start_time = 0;
    analyzer->end_time = 0;
}

int parse_timestamp(const char *str, struct tm *tm) {
    if (strlen(str) != 19) return 0;
    if (sscanf(str, "%d-%d-%d %d:%d:%d", 
               &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
               &tm->tm_hour, &tm->tm_min, &tm->tm_sec) != 6) return 0;
    tm->tm_year -= 1900;
    tm->tm_mon -= 1;
    return 1;
}

int validate_level(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer->count >= MAX_ENTRIES) return 0;
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->count];
    struct tm tm = {0};
    
    char level_buf[MAX_LEVEL_LENGTH];
    char message_buf[256];
    
    if (sscanf(line, "%31s %15s %255[^\n]", 
               entry->timestamp, level_buf, message_buf) != 3) {
        return 0;
    }
    
    if (!parse_timestamp(entry->timestamp, &tm)) return 0;
    
    entry->severity = validate_level(level_buf);
    if (entry->severity == 0) return 0;
    
    strncpy(entry->level, level_buf, MAX_LEVEL_LENGTH - 1);
    entry->level[MAX_LEVEL_LENGTH - 1] = '\0';
    strncpy(entry->message, message_buf, 255);
    entry->message[255] = '\0';
    
    time_t entry_time = mktime(&tm);
    if (entry_time == -1) return 0;
    
    if (analyzer->count == 0) {
        analyzer->start_time = entry_time;
        analyzer->end_time = entry_time;
    } else {
        if (entry_time < analyzer->start_time) analyzer->start_time = entry_time;
        if (entry_time > analyzer->end_time) analyzer->end_time = entry_time;
    }
    
    switch (entry->severity) {
        case 3: analyzer->error_count++; break;
        case 2: analyzer->warning_count++; break;
        case 1: analyzer->info_count++; break;
    }
    
    analyzer->count++;
    return 1;
}

void generate_report(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Report\n");
    printf("==================\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("Time range: %ld seconds\n", analyzer->end_time - analyzer->start_time);
    printf("Error count: %d\n", analyzer->error_count);
    printf("Warning count: %d\n", analyzer->warning_count);
    printf("Info count: %d\n", analyzer->info_count);
    
    if (analyzer->count > 0) {
        printf("\nMost recent entries:\n");
        int start = analyzer->count > 5 ? analyzer->count - 5 : 0;
        for (int i = start; i < analyzer->count; i++) {
            const struct LogEntry *entry = &analyzer->entries[i];
            printf("%s [%s] %s\n", entry->timestamp, entry->level, entry->message);
        }
    }
}

int main(void) {
    struct LogAnalyzer analyzer;
    init_log_analyzer(&analyzer);
    
    char line[MAX_LINE_LENGTH];
    int line_count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Enter empty line to finish.\n");
    
    while (fgets(line, sizeof(line), stdin) != NULL) {
        line_count++;
        if (line_count > 100) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') line[len-1] = '\0';
        
        if (strlen(line) == 0) break;
        
        if