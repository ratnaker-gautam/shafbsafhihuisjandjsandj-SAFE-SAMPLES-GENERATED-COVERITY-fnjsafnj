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
};

void init_analyzer(struct LogAnalyzer *analyzer) {
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
}

int parse_severity(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int validate_timestamp(const char *timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int add_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (analyzer->count >= MAX_ENTRIES) return 0;
    if (!validate_timestamp(timestamp)) return 0;
    if (strlen(level) >= MAX_LEVEL_LENGTH) return 0;
    if (strlen(message) >= 256) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->count];
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, MAX_LEVEL_LENGTH - 1);
    entry->level[MAX_LEVEL_LENGTH - 1] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    entry->severity = parse_severity(level);
    
    if (entry->severity == 3) analyzer->error_count++;
    else if (entry->severity == 2) analyzer->warning_count++;
    else if (entry->severity == 1) analyzer->info_count++;
    
    analyzer->count++;
    return 1;
}

void generate_report(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Report\n");
    printf("==================\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("ERROR entries: %d\n", analyzer->error_count);
    printf("WARNING entries: %d\n", analyzer->warning_count);
    printf("INFO entries: %d\n", analyzer->info_count);
    printf("\nRecent entries:\n");
    
    int start = analyzer->count > 5 ? analyzer->count - 5 : 0;
    for (int i = start; i < analyzer->count; i++) {
        const struct LogEntry *entry = &analyzer->entries[i];
        printf("[%s] %s: %s\n", entry->timestamp, entry->level, entry->message);
    }
}

int main(void) {
    struct LogAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    printf("Log Analyzer - Enter log entries (timestamp level message)\n");
    printf("Timestamp format: YYYY-MM-DD HH:MM:SS\n");
    printf("Levels: ERROR, WARNING, INFO\n");
    printf("Enter 'quit' to finish\n\n");
    
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "quit", 4) == 0) break;
        
        char timestamp[32] = {0};
        char level[MAX_LEVEL_LENGTH] = {0};
        char message[256] = {0};
        
        int result = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (result < 3) {
            printf("Invalid format. Use: YYYY-MM-DD HH:MM:SS LEVEL message\n");
            continue;
        }
        
        if (!add_entry(&analyzer, timestamp, level, message)) {
            printf("Failed to add entry. Check format and limits.\n");
        } else {
            printf("Entry added successfully.\n");
        }
        
        if (analyzer.count >= MAX_ENTRIES) {
            printf("Maximum entries reached.\n");
            break;
        }
    }
    
    generate_report(&analyzer);
    return 0;
}