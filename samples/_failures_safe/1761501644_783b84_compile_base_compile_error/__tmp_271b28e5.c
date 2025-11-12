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

void init_analyzer(struct LogAnalyzer *analyzer) {
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
}

int parse_timestamp(const char *str, char *output) {
    if (str == NULL || output == NULL) return 0;
    
    int year, month, day, hour, min, sec;
    if (sscanf(str, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &min, &sec) != 6) {
        return 0;
    }
    
    if (year < 2000 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    if (hour < 0 || hour > 23) return 0;
    if (min < 0 || min > 59) return 0;
    if (sec < 0 || sec > 59) return 0;
    
    snprintf(output, 32, "%04d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, min, sec);
    return 1;
}

int validate_level(const char *level) {
    if (level == NULL) return 0;
    
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer == NULL || line == NULL) return 0;
    if (analyzer->count >= MAX_ENTRIES) return 0;
    
    char timestamp[32];
    char level[16];
    char message[256];
    
    if (sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message) != 3) {
        return 0;
    }
    
    struct LogEntry *entry = &analyzer->entries[analyzer->count];
    
    if (!parse_timestamp(timestamp, entry->timestamp)) {
        return 0;
    }
    
    int severity = validate_level(level);
    if (severity == 0) {
        return 0;
    }
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    entry->severity = severity;
    
    switch (severity) {
        case 3: analyzer->error_count++; break;
        case 2: analyzer->warning_count++; break;
        case 1: analyzer->info_count++; break;
    }
    
    analyzer->count++;
    return 1;
}

void generate_report(const struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Log Analysis Report\n");
    printf("==================\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("Errors: %d\n", analyzer->error_count);
    printf("Warnings: %d\n", analyzer->warning_count);
    printf("Info: %d\n", analyzer->info_count);
    printf("\n");
    
    if (analyzer->count > 0) {
        printf("Recent entries:\n");
        int start = analyzer->count > 5 ? analyzer->count - 5 : 0;
        for (int i = start; i < analyzer->count; i++) {
            const struct LogEntry *entry = &analyzer->entries[i];
            printf("%s [%s] %s\n", entry->timestamp, entry->level, entry->message);
        }
    }
}

int main(void) {
    struct LogAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Supported levels: ERROR, WARNING, INFO\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    char line[MAX_LINE_LENGTH];
    int line_count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL) {
        line_count++;
        if (line_count > MAX_ENTRIES) {
            printf("Maximum entries reached.\n");
            break;
        }
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1]