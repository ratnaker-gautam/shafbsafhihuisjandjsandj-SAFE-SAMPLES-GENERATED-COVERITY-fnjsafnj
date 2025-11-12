//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[20];
    char level[10];
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

int parse_timestamp(const char *str, char *timestamp) {
    if (strlen(str) < 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (str[i] != '-') return 0;
        } else if (i == 10) {
            if (str[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (str[i] != ':') return 0;
        } else {
            if (!isdigit(str[i])) return 0;
        }
    }
    strncpy(timestamp, str, 19);
    timestamp[19] = '\0';
    return 1;
}

int parse_level(const char *str, char *level, int *severity) {
    if (strncmp(str, "ERROR", 5) == 0) {
        strcpy(level, "ERROR");
        *severity = 3;
        return 1;
    } else if (strncmp(str, "WARNING", 7) == 0) {
        strcpy(level, "WARNING");
        *severity = 2;
        return 1;
    } else if (strncmp(str, "INFO", 4) == 0) {
        strcpy(level, "INFO");
        *severity = 1;
        return 1;
    }
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer->count >= MAX_ENTRIES) return 0;
    
    char timestamp[20];
    char level[10];
    int severity;
    char message[256];
    
    if (!parse_timestamp(line, timestamp)) return 0;
    
    const char *level_start = line + 20;
    if (*level_start != '[') return 0;
    level_start++;
    
    if (!parse_level(level_start, level, &severity)) return 0;
    
    const char *message_start = level_start + strlen(level) + 2;
    if (message_start >= line + strlen(line)) return 0;
    
    size_t msg_len = strlen(message_start);
    if (msg_len >= 256) msg_len = 255;
    strncpy(message, message_start, msg_len);
    message[msg_len] = '\0';
    
    struct LogEntry *entry = &analyzer->entries[analyzer->count];
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
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
    printf("Errors: %d\n", analyzer->error_count);
    printf("Warnings: %d\n", analyzer->warning_count);
    printf("Info: %d\n", analyzer->info_count);
    
    if (analyzer->count > 0) {
        printf("\nMost recent entries:\n");
        int start = analyzer->count - 3;
        if (start < 0) start = 0;
        for (int i = start; i < analyzer->count; i++) {
            printf("%s [%s] %s\n", 
                   analyzer->entries[i].timestamp,
                   analyzer->entries[i].level,
                   analyzer->entries[i].message);
        }
    }
}

int main(void) {
    struct LogAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char line[MAX_LINE_LENGTH];
    int line_count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS [LEVEL] message):\n");
    printf("Enter empty line to finish.\n");
    
    while (fgets(line, sizeof(line), stdin) != NULL) {
        line_count++;
        if (line_count > 100) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }
        
        if (strlen(line) == 0) break;
        
        if (!add_log_entry(&analyzer, line)) {
            printf("Invalid log format on line %d\n", line_count);
        }
    }
    
    if (analyzer.count == 0