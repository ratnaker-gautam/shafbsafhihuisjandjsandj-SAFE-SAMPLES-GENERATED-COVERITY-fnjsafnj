//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000
#define MAX_LEVEL_LENGTH 10

struct LogEntry {
    char timestamp[20];
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

int validate_level(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int parse_timestamp(const char *timestamp) {
    struct tm tm = {0};
    if (strptime(timestamp, "%Y-%m-%d %H:%M:%S", &tm) == NULL) {
        return 0;
    }
    return 1;
}

int add_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (analyzer->count >= MAX_ENTRIES) {
        return 0;
    }
    if (strlen(timestamp) >= 20 || strlen(level) >= MAX_LEVEL_LENGTH || strlen(message) >= 256) {
        return 0;
    }
    
    int severity = validate_level(level);
    if (severity == 0) {
        return 0;
    }
    if (!parse_timestamp(timestamp)) {
        return 0;
    }
    
    struct LogEntry *entry = &analyzer->entries[analyzer->count];
    strncpy(entry->timestamp, timestamp, 19);
    entry->timestamp[19] = '\0';
    strncpy(entry->level, level, MAX_LEVEL_LENGTH - 1);
    entry->level[MAX_LEVEL_LENGTH - 1] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    entry->severity = severity;
    
    analyzer->count++;
    
    if (severity == 3) analyzer->error_count++;
    else if (severity == 2) analyzer->warning_count++;
    else if (severity == 1) analyzer->info_count++;
    
    return 1;
}

void generate_summary(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("ERROR entries: %d\n", analyzer->error_count);
    printf("WARNING entries: %d\n", analyzer->warning_count);
    printf("INFO entries: %d\n", analyzer->info_count);
    
    if (analyzer->count > 0) {
        printf("Error rate: %.2f%%\n", (float)analyzer->error_count / analyzer->count * 100);
    }
}

void display_entries_by_level(const struct LogAnalyzer *analyzer, const char *level) {
    int severity = validate_level(level);
    if (severity == 0) {
        printf("Invalid log level: %s\n", level);
        return;
    }
    
    printf("Entries with level %s:\n", level);
    for (int i = 0; i < analyzer->count; i++) {
        if (analyzer->entries[i].severity == severity) {
            printf("[%s] %s: %s\n", analyzer->entries[i].timestamp, 
                   analyzer->entries[i].level, analyzer->entries[i].message);
        }
    }
}

int read_log_file(struct LogAnalyzer *analyzer) {
    char filename[256];
    printf("Enter log filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        return 0;
    }
    filename[strcspn(filename, "\n")] = '\0';
    
    if (strlen(filename) == 0) {
        printf("No filename provided.\n");
        return 0;
    }
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Cannot open file: %s\n", filename);
        return 0;
    }
    
    char line[MAX_LINE_LENGTH];
    int lines_read = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && lines_read < MAX_ENTRIES) {
        lines_read++;
        line[strcspn(line, "\n")] = '\0';
        
        char timestamp[20] = {0};
        char level[MAX_LEVEL_LENGTH] = {0};
        char message[256] = {0};
        
        if (sscanf(line, "%19[^,],%9[^,],%255[^\n]", timestamp, level, message) == 3) {
            if (!add_entry(analyzer, timestamp, level, message)) {
                printf("Skipping invalid entry: %s\n", line);
            }
        } else {
            printf("Skipping malformed line: %s