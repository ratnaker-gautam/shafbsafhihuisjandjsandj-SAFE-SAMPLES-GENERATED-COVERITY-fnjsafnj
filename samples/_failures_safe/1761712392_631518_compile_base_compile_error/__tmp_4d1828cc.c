//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
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
    int debug_count;
};

void init_analyzer(struct LogAnalyzer *analyzer) {
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
    analyzer->debug_count = 0;
}

int parse_timestamp(const char *timestamp, struct tm *tm) {
    return strptime(timestamp, "%Y-%m-%d %H:%M:%S", tm) != NULL;
}

int validate_level(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "WARNING") == 0) return 3;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "DEBUG") == 0) return 1;
    return 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || strlen(line) == 0) return 0;
    
    char timestamp[32] = {0};
    char level[MAX_LEVEL_LENGTH] = {0};
    char message[256] = {0};
    
    int result = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (result != 3) return 0;
    
    struct tm tm = {0};
    if (!parse_timestamp(timestamp, &tm)) return 0;
    
    int severity = validate_level(level);
    if (severity == 0) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    entry->severity = severity;
    
    return 1;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer->count >= MAX_ENTRIES) return 0;
    
    struct LogEntry entry;
    if (!parse_log_line(line, &entry)) return 0;
    
    analyzer->entries[analyzer->count] = entry;
    analyzer->count++;
    
    switch (entry.severity) {
        case 4: analyzer->error_count++; break;
        case 3: analyzer->warning_count++; break;
        case 2: analyzer->info_count++; break;
        case 1: analyzer->debug_count++; break;
    }
    
    return 1;
}

void generate_summary(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("ERROR: %d\n", analyzer->error_count);
    printf("WARNING: %d\n", analyzer->warning_count);
    printf("INFO: %d\n", analyzer->info_count);
    printf("DEBUG: %d\n", analyzer->debug_count);
    
    if (analyzer->count > 0) {
        printf("Error rate: %.2f%%\n", 
               (float)analyzer->error_count / analyzer->count * 100);
    }
}

void find_most_severe_entries(const struct LogAnalyzer *analyzer, int min_severity) {
    printf("Entries with severity >= %d:\n", min_severity);
    int found = 0;
    
    for (int i = 0; i < analyzer->count; i++) {
        if (analyzer->entries[i].severity >= min_severity) {
            printf("%s %s %s\n", 
                   analyzer->entries[i].timestamp,
                   analyzer->entries[i].level,
                   analyzer->entries[i].message);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No entries found with specified severity.\n");
    }
}

int main(void) {
    struct LogAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char line[MAX_LINE_LENGTH];
    int line_count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (fgets(line, sizeof(line), stdin) != NULL) {
        line_count++;
        
        if (line_count > MAX_ENTRIES) {
            printf("Maximum number of entries reached.\n");
            break;
        }
        
        size_t len = strlen(line);
        if (len > 0 && line[len -