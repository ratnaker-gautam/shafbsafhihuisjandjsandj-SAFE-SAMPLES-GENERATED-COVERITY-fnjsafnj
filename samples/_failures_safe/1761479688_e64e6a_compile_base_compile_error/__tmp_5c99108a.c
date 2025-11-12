//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
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

int read_log_file(const char* filename, LogEntry** entries, int* count) {
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[MAX_LINE_LEN];
    *count = 0;
    *entries = malloc(MAX_ENTRIES * sizeof(LogEntry));
    if (!*entries) {
        fclose(file);
        return 0;
    }
    
    while (fgets(line, sizeof(line), file) && *count < MAX_ENTRIES) {
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        if (sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message) == 3) {
            if (validate_timestamp(timestamp) && parse_log_level(level) > 0) {
                LogEntry* entry = &(*entries)[*count];
                strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
                strncpy(entry->level, level, sizeof(entry->level) - 1);
                strncpy(entry->message, message, sizeof(entry->message) - 1);
                entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
                entry->level[sizeof(entry->level) - 1] = '\0';
                entry->message[sizeof(entry->message) - 1] = '\0';
                entry->severity = parse_log_level(level);
                (*count)++;
            }
        }
    }
    
    fclose(file);
    return 1;
}

void analyze_logs(LogEntry* entries, int count) {
    int level_counts[6] = {0};
    int total_severity = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 1 && entries[i].severity <= 5) {
            level_counts[entries[i].severity]++;
            total_severity += entries[i].severity;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("DEBUG: %d\n", level_counts[1]);
    printf("INFO: %d\n", level_counts[2]);
    printf("WARNING: %d\n", level_counts[3]);
    printf("ERROR: %d\n", level_counts[4]);
    printf("CRITICAL: %d\n", level_counts[5]);
    
    if (count > 0) {
        double avg_severity = (double)total_severity / count;
        printf("Average severity: %.2f\n", avg_severity);
    }
}

void find_most_severe(LogEntry* entries, int count) {
    if (count == 0) {
        printf("No logs to analyze.\n");
        return;
    }
    
    int max_severity = 0;
    int max_index = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity > max_severity) {
            max_severity = entries[i].severity;
            max_index = i;
        }
    }
    
    printf("Most severe log entry:\n");
    printf("Timestamp: %s\n", entries[max_index].timestamp);
    printf("Level: %s\n", entries[max_index].level);
    printf("Message: %s\n", entries[max_index].message);
}

int main() {
    char filename[256];
    printf("Enter log file name: ");
    if (scanf("%255s", filename) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    LogEntry* entries = NULL;
    int count = 0;
    
    if (!read_log_file(filename, &entries, &count)) {
        printf("