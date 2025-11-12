//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000
#define MAX_LEVEL_LENGTH 16
#define MAX_MESSAGE_LENGTH 256

struct LogEntry {
    time_t timestamp;
    char level[MAX_LEVEL_LENGTH];
    char message[MAX_MESSAGE_LENGTH];
    int severity;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count;
    int error_count;
    int warning_count;
    int info_count;
    time_t start_time;
    time_t end_time;
};

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    analyzer->entry_count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
    analyzer->start_time = 0;
    analyzer->end_time = 0;
}

int parse_timestamp(const char *str, time_t *timestamp) {
    struct tm tm = {0};
    if (strptime(str, "%Y-%m-%d %H:%M:%S", &tm) == NULL) {
        return 0;
    }
    *timestamp = mktime(&tm);
    return *timestamp != -1;
}

int parse_log_level(const char *level, int *severity) {
    if (strcmp(level, "ERROR") == 0) {
        *severity = 3;
        return 1;
    } else if (strcmp(level, "WARNING") == 0) {
        *severity = 2;
        return 1;
    } else if (strcmp(level, "INFO") == 0) {
        *severity = 1;
        return 1;
    }
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp_str, 
                  const char *level, const char *message) {
    if (analyzer->entry_count >= MAX_ENTRIES) {
        return 0;
    }
    
    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    
    if (!parse_timestamp(timestamp_str, &entry->timestamp)) {
        return 0;
    }
    
    if (strlen(level) >= MAX_LEVEL_LENGTH) {
        return 0;
    }
    strncpy(entry->level, level, MAX_LEVEL_LENGTH - 1);
    entry->level[MAX_LEVEL_LENGTH - 1] = '\0';
    
    if (strlen(message) >= MAX_MESSAGE_LENGTH) {
        return 0;
    }
    strncpy(entry->message, message, MAX_MESSAGE_LENGTH - 1);
    entry->message[MAX_MESSAGE_LENGTH - 1] = '\0';
    
    if (!parse_log_level(level, &entry->severity)) {
        return 0;
    }
    
    if (analyzer->entry_count == 0) {
        analyzer->start_time = entry->timestamp;
        analyzer->end_time = entry->timestamp;
    } else {
        if (entry->timestamp < analyzer->start_time) {
            analyzer->start_time = entry->timestamp;
        }
        if (entry->timestamp > analyzer->end_time) {
            analyzer->end_time = entry->timestamp;
        }
    }
    
    switch (entry->severity) {
        case 3: analyzer->error_count++; break;
        case 2: analyzer->warning_count++; break;
        case 1: analyzer->info_count++; break;
    }
    
    analyzer->entry_count++;
    return 1;
}

void generate_report(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Report\n");
    printf("==================\n");
    printf("Total entries: %d\n", analyzer->entry_count);
    printf("Time range: %ld seconds\n", analyzer->end_time - analyzer->start_time);
    printf("Error count: %d\n", analyzer->error_count);
    printf("Warning count: %d\n", analyzer->warning_count);
    printf("Info count: %d\n", analyzer->info_count);
    
    if (analyzer->entry_count > 0) {
        double error_rate = (double)analyzer->error_count / analyzer->entry_count * 100.0;
        printf("Error rate: %.2f%%\n", error_rate);
    }
}

int main(void) {
    struct LogAnalyzer analyzer;
    init_log_analyzer(&analyzer);
    
    printf("Log Analyzer - Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE)\n");
    printf("Enter 'quit' to finish input\n");
    
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "quit", 4) == 0) {
            break;
        }
        
        char timestamp[20], level[MAX_LEVEL_LENGTH], message[MAX_MESSAGE_LENGTH];
        
        if (sscanf(line, "%19s %15s %255[^\n]", timestamp, level, message) == 3) {
            if (add_log_entry(&analyzer, timestamp, level, message)) {
                printf("Entry added successfully\n");
            } else {
                printf("Failed to add entry - invalid format or analyzer full\n");
            }
        } else {
            printf("Invalid input format\n");
        }
    }
    
    generate