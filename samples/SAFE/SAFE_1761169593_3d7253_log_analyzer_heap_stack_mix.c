//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
} LogEntry;

int parse_log_level(const char* level) {
    if (strcmp(level, "DEBUG") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "WARNING") == 0) return 3;
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "CRITICAL") == 0) return 5;
    return 0;
}

int validate_timestamp(const char* timestamp) {
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

int read_log_file(const char* filename, LogEntry** entries, int* count) {
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[MAX_LINE_LENGTH];
    *count = 0;
    *entries = malloc(MAX_ENTRIES * sizeof(LogEntry));
    if (!*entries) {
        fclose(file);
        return 0;
    }
    
    while (fgets(line, sizeof(line), file) && *count < MAX_ENTRIES) {
        if (strlen(line) < 10) continue;
        
        char timestamp[32], level[16], message[256];
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        
        if (parsed == 3 && validate_timestamp(timestamp)) {
            int severity = parse_log_level(level);
            if (severity > 0) {
                LogEntry* entry = &(*entries)[(*count)++];
                strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
                strncpy(entry->level, level, sizeof(entry->level) - 1);
                strncpy(entry->message, message, sizeof(entry->message) - 1);
                entry->severity = severity;
                entry->timestamp[31] = '\0';
                entry->level[15] = '\0';
                entry->message[255] = '\0';
            }
        }
    }
    
    fclose(file);
    return 1;
}

void analyze_logs(LogEntry* entries, int count) {
    if (count == 0) {
        printf("No valid log entries found.\n");
        return;
    }
    
    int severity_count[6] = {0};
    char latest_timestamp[32] = "";
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 1 && entries[i].severity <= 5) {
            severity_count[entries[i].severity]++;
        }
        if (strcmp(entries[i].timestamp, latest_timestamp) > 0) {
            strncpy(latest_timestamp, entries[i].timestamp, sizeof(latest_timestamp) - 1);
            latest_timestamp[31] = '\0';
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("Latest timestamp: %s\n", latest_timestamp);
    printf("Severity breakdown:\n");
    printf("  DEBUG: %d\n", severity_count[1]);
    printf("  INFO: %d\n", severity_count[2]);
    printf("  WARNING: %d\n", severity_count[3]);
    printf("  ERROR: %d\n", severity_count[4]);
    printf("  CRITICAL: %d\n", severity_count[5]);
}

int main(void) {
    char filename[256];
    printf("Enter log file name: ");
    
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        printf("Invalid filename.\n");
        return 1;
    }
    
    LogEntry* entries = NULL;
    int count = 0;
    
    if (!read_log_file(filename, &entries, &count)) {
        printf("Error reading log file or no valid entries found.\n");
        if (entries) free(entries);
        return 1;
    }
    
    analyze_logs(entries, count);
    
    free(entries);
    return 0;
}