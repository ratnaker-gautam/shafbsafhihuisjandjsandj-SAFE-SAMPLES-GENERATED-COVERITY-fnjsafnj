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

int validate_timestamp(const char *timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int validate_level(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char level_buf[MAX_LEVEL_LENGTH];
    char timestamp_buf[20];
    char message_buf[256];
    
    int result = sscanf(line, "%19s %9s %255[^\n]", timestamp_buf, level_buf, message_buf);
    if (result != 3) return 0;
    
    if (!validate_timestamp(timestamp_buf)) return 0;
    if (!validate_level(level_buf)) return 0;
    
    strncpy(entry->timestamp, timestamp_buf, 19);
    entry->timestamp[19] = '\0';
    strncpy(entry->level, level_buf, MAX_LEVEL_LENGTH - 1);
    entry->level[MAX_LEVEL_LENGTH - 1] = '\0';
    strncpy(entry->message, message_buf, 255);
    entry->message[255] = '\0';
    
    if (strcmp(level_buf, "ERROR") == 0) entry->severity = 3;
    else if (strcmp(level_buf, "WARNING") == 0) entry->severity = 2;
    else entry->severity = 1;
    
    return 1;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer == NULL || line == NULL) return 0;
    if (analyzer->count >= MAX_ENTRIES) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->count];
    if (!parse_log_line(line, entry)) return 0;
    
    if (entry->severity == 3) analyzer->error_count++;
    else if (entry->severity == 2) analyzer->warning_count++;
    else analyzer->info_count++;
    
    analyzer->count++;
    return 1;
}

void generate_summary(const struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("ERROR entries: %d\n", analyzer->error_count);
    printf("WARNING entries: %d\n", analyzer->warning_count);
    printf("INFO entries: %d\n", analyzer->info_count);
    
    if (analyzer->count > 0) {
        printf("Most severe entries:\n");
        for (int i = 0; i < analyzer->count && i < 3; i++) {
            if (analyzer->entries[i].severity >= 2) {
                printf("- %s [%s]: %s\n", 
                       analyzer->entries[i].timestamp,
                       analyzer->entries[i].level,
                       analyzer->entries[i].message);
            }
        }
    }
}

int main(void) {
    struct LogAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char line[MAX_LINE_LENGTH];
    int line_count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Supported levels: ERROR, WARNING, INFO\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (fgets(line, sizeof(line), stdin) != NULL) {
        line_count++;
        if (line_count > MAX_ENTRIES) {
            printf("Maximum entry limit reached.\n");
            break;
        }
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strcmp(line, "