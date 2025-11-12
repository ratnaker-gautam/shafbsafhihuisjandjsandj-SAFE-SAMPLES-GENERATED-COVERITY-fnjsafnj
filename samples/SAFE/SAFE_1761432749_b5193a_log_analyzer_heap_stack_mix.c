//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
};

int parse_severity(const char* level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARN") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "DEBUG") == 0) return 0;
    return -1;
}

int read_log_entries(struct LogEntry* entries, int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a new line to finish.\n");
    
    while (count < max_entries && fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed >= 2) {
            if (strlen(timestamp) > 0 && strlen(level) > 0) {
                strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
                strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
                if (parsed == 3) {
                    strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
                } else {
                    entries[count].message[0] = '\0';
                }
                entries[count].severity = parse_severity(level);
                count++;
            }
        }
    }
    
    return count;
}

void analyze_logs(struct LogEntry* entries, int count) {
    if (count <= 0) {
        printf("No log entries to analyze.\n");
        return;
    }
    
    int severity_count[4] = {0};
    int total_severity = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 0 && entries[i].severity <= 3) {
            severity_count[entries[i].severity]++;
            total_severity += entries[i].severity;
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("DEBUG: %d\n", severity_count[0]);
    printf("INFO: %d\n", severity_count[1]);
    printf("WARN: %d\n", severity_count[2]);
    printf("ERROR: %d\n", severity_count[3]);
    
    if (count > 0) {
        double avg_severity = (double)total_severity / count;
        printf("Average severity: %.2f\n", avg_severity);
    }
    
    printf("\nMost recent entries:\n");
    int display_count = count < 5 ? count : 5;
    for (int i = count - display_count; i < count; i++) {
        printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
}

int main(void) {
    struct LogEntry* heap_entries = malloc(MAX_ENTRIES * sizeof(struct LogEntry));
    if (heap_entries == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    int entry_count = read_log_entries(heap_entries, MAX_ENTRIES);
    
    struct LogEntry stack_entries[100];
    int stack_count = entry_count < 100 ? entry_count : 100;
    
    for (int i = 0; i < stack_count; i++) {
        strncpy(stack_entries[i].timestamp, heap_entries[i].timestamp, sizeof(stack_entries[i].timestamp) - 1);
        strncpy(stack_entries[i].level, heap_entries[i].level, sizeof(stack_entries[i].level) - 1);
        strncpy(stack_entries[i].message, heap_entries[i].message, sizeof(stack_entries[i].message) - 1);
        stack_entries[i].severity = heap_entries[i].severity;
    }
    
    analyze_logs(heap_entries, entry_count);
    
    printf("\nProcessing complete. Analyzed %d log entries.\n", entry_count);
    
    free(heap_entries);
    return 0;
}