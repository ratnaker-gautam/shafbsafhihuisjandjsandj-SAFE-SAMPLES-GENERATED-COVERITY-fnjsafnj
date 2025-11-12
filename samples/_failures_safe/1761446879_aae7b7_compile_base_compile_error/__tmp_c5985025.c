//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char buffer[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strncpy(buffer, line, MAX_LINE_LENGTH - 1);
    buffer[MAX_LINE_LENGTH - 1] = '\0';
    
    char* parts[3];
    parts[0] = buffer;
    int part_count = 1;
    
    for (char* p = buffer; *p && part_count < 3; p++) {
        if (*p == '|') {
            *p = '\0';
            parts[part_count++] = p + 1;
        }
    }
    
    if (part_count != 3) return 0;
    
    if (strlen(parts[0]) >= sizeof(entry->timestamp) - 1) return 0;
    if (strlen(parts[1]) >= sizeof(entry->level) - 1) return 0;
    if (strlen(parts[2]) >= sizeof(entry->message) - 1) return 0;
    
    strncpy(entry->timestamp, parts[0], sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, parts[1], sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, parts[2], sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int read_log_file(struct LogEntry entries[], int max_entries) {
    if (entries == NULL || max_entries <= 0) return 0;
    
    printf("Enter log entries (format: timestamp|level|message), one per line.\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (count < max_entries) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        if (parse_log_line(line, &entries[count])) {
            count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    return count;
}

void analyze_log_levels(const struct LogEntry entries[], int count) {
    if (entries == NULL || count <= 0) return;
    
    int info_count = 0, warning_count = 0, error_count = 0, other_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcasecmp(entries[i].level, "INFO") == 0) {
            info_count++;
        } else if (strcasecmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcasecmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else {
            other_count++;
        }
    }
    
    printf("\nLog Level Analysis:\n");
    printf("INFO: %d entries\n", info_count);
    printf("WARNING: %d entries\n", warning_count);
    printf("ERROR: %d entries\n", error_count);
    printf("OTHER: %d entries\n", other_count);
}

void search_logs(const struct LogEntry entries[], int count, const char* keyword) {
    if (entries == NULL || count <= 0 || keyword == NULL) return;
    
    printf("\nSearch results for '%s':\n", keyword);
    int found = 0;
    
    for (int i = 0; i < count; i++) {
        if (strstr(entries[i].message, keyword) != NULL) {
            printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No entries found containing '%s'\n", keyword);
    }
}

void display_recent_logs(const struct LogEntry entries[], int count, int num_recent) {
    if (entries == NULL || count <= 0 || num_recent <= 0) return;
    
    if (num_recent > count) num_recent = count;
    
    printf("\n%d most recent log entries:\n", num_recent);
    
    int start = count - num_recent;
    if (start < 0) start = 0;
    
    for (int i = start; i < count; i++) {
        printf("[