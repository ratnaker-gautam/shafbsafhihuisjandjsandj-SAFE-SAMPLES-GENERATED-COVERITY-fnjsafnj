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
    if (analyzer == NULL) return;
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
}

int parse_timestamp(const char *str, char *timestamp) {
    if (str == NULL || timestamp == NULL) return 0;
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
    if (str == NULL || level == NULL || severity == NULL) return 0;
    
    if (strncmp(str, "ERROR", 5) == 0) {
        strncpy(level, "ERROR", 16);
        level[15] = '\0';
        *severity = 3;
        return 1;
    } else if (strncmp(str, "WARNING", 7) == 0) {
        strncpy(level, "WARNING", 16);
        level[15] = '\0';
        *severity = 2;
        return 1;
    } else if (strncmp(str, "INFO", 4) == 0) {
        strncpy(level, "INFO", 16);
        level[15] = '\0';
        *severity = 1;
        return 1;
    }
    
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer == NULL || line == NULL) return 0;
    if (analyzer->count >= MAX_ENTRIES) return 0;
    
    char timestamp[32] = {0};
    char level[16] = {0};
    char message[256] = {0};
    int severity = 0;
    
    const char *ptr = line;
    
    if (!parse_timestamp(ptr, timestamp)) return 0;
    ptr += 20;
    
    if (!parse_level(ptr, level, &severity)) return 0;
    
    while (*ptr && *ptr != ' ') ptr++;
    while (*ptr && *ptr == ' ') ptr++;
    
    size_t msg_len = strlen(ptr);
    if (msg_len == 0) return 0;
    if (msg_len >= 256) msg_len = 255;
    
    strncpy(message, ptr, msg_len);
    message[msg_len] = '\0';
    
    struct LogEntry *entry = &analyzer->entries[analyzer->count];
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    entry->severity = severity;
    
    analyzer->count++;
    
    if (severity == 3) analyzer->error_count++;
    else if (severity == 2) analyzer->warning_count++;
    else if (severity == 1) analyzer->info_count++;
    
    return 1;
}

void generate_sample_logs(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    const char *sample_logs[] = {
        "2024-01-15 10:30:25 INFO System started successfully",
        "2024-01-15 10:31:10 WARNING High memory usage detected",
        "2024-01-15 10:32:45 ERROR Database connection failed",
        "2024-01-15 10:33:20 INFO User login successful",
        "2024-01-15 10:34:05 WARNING Disk space running low",
        "2024-01-15 10:35:30 ERROR File not found: config.xml",
        "2024-01-15 10:36:15 INFO Backup completed",
        "2024-01-15 10:37:40 WARNING Unusual network activity",
        "2024-01-15 10:38:25 INFO Cache cleared",
        "2024-01-15 10:39:50 ERROR Permission denied: /var/log"
    };
    
    int num