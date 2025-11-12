//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    int count;
    int error_count;
    int warning_count;
    int info_count;
};

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || strlen(line) < 20) {
        return 0;
    }
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) {
        return 0;
    }
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char *timestamp_end = strchr(temp_line, ' ');
    if (timestamp_end == NULL) {
        return 0;
    }
    
    size_t timestamp_len = timestamp_end - temp_line;
    if (timestamp_len >= sizeof(entry->timestamp)) {
        return 0;
    }
    strncpy(entry->timestamp, temp_line, timestamp_len);
    entry->timestamp[timestamp_len] = '\0';
    
    char *level_start = timestamp_end + 1;
    char *level_end = strchr(level_start, ' ');
    if (level_end == NULL) {
        return 0;
    }
    
    size_t level_len = level_end - level_start;
    if (level_len >= sizeof(entry->level)) {
        return 0;
    }
    strncpy(entry->level, level_start, level_len);
    entry->level[level_len] = '\0';
    
    char *message_start = level_end + 1;
    size_t message_len = strlen(message_start);
    if (message_len >= sizeof(entry->message)) {
        message_len = sizeof(entry->message) - 1;
    }
    strncpy(entry->message, message_start, message_len);
    entry->message[message_len] = '\0';
    
    if (strcmp(entry->level, "ERROR") == 0) {
        entry->severity = 3;
    } else if (strcmp(entry->level, "WARNING") == 0) {
        entry->severity = 2;
    } else if (strcmp(entry->level, "INFO") == 0) {
        entry->severity = 1;
    } else {
        entry->severity = 0;
    }
    
    return 1;
}

void add_log_entry(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    if (analyzer->count >= MAX_ENTRIES) {
        return;
    }
    
    analyzer->entries[analyzer->count] = *entry;
    
    if (entry->severity == 3) {
        analyzer->error_count++;
    } else if (entry->severity == 2) {
        analyzer->warning_count++;
    } else if (entry->severity == 1) {
        analyzer->info_count++;
    }
    
    analyzer->count++;
}

void analyze_logs(struct LogAnalyzer *analyzer) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("ERROR entries: %d\n", analyzer->error_count);
    printf("WARNING entries: %d\n", analyzer->warning_count);
    printf("INFO entries: %d\n", analyzer->info_count);
    
    if (analyzer->count > 0) {
        printf("\nFirst 5 entries:\n");
        int display_count = analyzer->count < 5 ? analyzer->count : 5;
        for (int i = 0; i < display_count; i++) {
            printf("[%s] %s: %s\n", 
                   analyzer->entries[i].timestamp,
                   analyzer->entries[i].level,
                   analyzer->entries[i].message);
        }
    }
}

int main(void) {
    struct LogAnalyzer analyzer;
    init_log_analyzer(&analyzer);
    
    char line[MAX_LINE_LENGTH];
    int line_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
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
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry