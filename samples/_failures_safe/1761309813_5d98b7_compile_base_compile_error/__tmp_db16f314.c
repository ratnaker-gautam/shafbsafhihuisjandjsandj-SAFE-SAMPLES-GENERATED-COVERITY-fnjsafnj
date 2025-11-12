//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 10000

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

int is_valid_timestamp(const char* timestamp) {
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

int read_log_file(const char* filename, LogEntry* entries, int* count) {
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[MAX_LINE_LENGTH];
    *count = 0;
    
    while (fgets(line, sizeof(line), file) && *count < MAX_ENTRIES) {
        if (strlen(line) < 10) continue;
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed != 3) continue;
        
        if (!is_valid_timestamp(timestamp)) continue;
        
        int severity = parse_log_level(level);
        if (severity == 0) continue;
        
        strncpy(entries[*count].timestamp, timestamp, sizeof(entries[*count].timestamp) - 1);
        entries[*count].timestamp[sizeof(entries[*count].timestamp) - 1] = '\0';
        strncpy(entries[*count].level, level, sizeof(entries[*count].level) - 1);
        entries[*count].level[sizeof(entries[*count].level) - 1] = '\0';
        strncpy(entries[*count].message, message, sizeof(entries[*count].message) - 1);
        entries[*count].message[sizeof(entries[*count].message) - 1] = '\0';
        entries[*count].severity = severity;
        (*count)++;
    }
    
    fclose(file);
    return 1;
}

void analyze_logs(LogEntry* entries, int count) {
    int severity_count[6] = {0};
    int total_messages = 0;
    int error_messages = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 1 && entries[i].severity <= 5) {
            severity_count[entries[i].severity]++;
            total_messages++;
            if (entries[i].severity >= 4) {
                error_messages++;
            }
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries processed: %d\n", total_messages);
    printf("DEBUG: %d\n", severity_count[1]);
    printf("INFO: %d\n", severity_count[2]);
    printf("WARNING: %d\n", severity_count[3]);
    printf("ERROR: %d\n", severity_count[4]);
    printf("CRITICAL: %d\n", severity_count[5]);
    printf("Error rate: %.2f%%\n", total_messages > 0 ? (error_messages * 100.0) / total_messages : 0.0);
    
    if (count > 0) {
        printf("\nRecent critical errors:\n");
        int printed = 0;
        for (int i = count - 1; i >= 0 && printed < 5; i--) {
            if (entries[i].severity == 5) {
                printf("- %s: %s\n", entries[i].timestamp, entries[i].message);
                printed++;
            }
        }
        if (printed == 0) {
            printf("No critical errors found.\n");
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    LogEntry entries[MAX_ENTRIES];
    int count = 0;
    
    if (!read_log_file(argv[1], entries, &count)) {
        printf("Error: Could not read log file '%s'\n", argv[1]);
        return 1;
    }
    
    if (count == 0) {