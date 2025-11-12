//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define MAX_FILENAME 256

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
};

int parse_log_level(const char* level) {
    if (strcmp(level, "DEBUG") == 0) return 0;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "CRITICAL") == 0) return 4;
    return -1;
}

int is_valid_timestamp(const char* ts) {
    if (strlen(ts) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (ts[i] != '-') return 0;
        } else if (i == 10) {
            if (ts[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (ts[i] != ':') return 0;
        } else {
            if (!isdigit(ts[i])) return 0;
        }
    }
    return 1;
}

int read_log_file(const char* filename, struct LogEntry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        if (strlen(line) >= sizeof(line) - 1) continue;
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed != 3) continue;
        
        if (!is_valid_timestamp(timestamp)) continue;
        
        int severity = parse_log_level(level);
        if (severity == -1) continue;
        
        strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
        strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        entries[count].severity = severity;
        entries[count].timestamp[31] = '\0';
        entries[count].level[15] = '\0';
        entries[count].message[255] = '\0';
        
        count++;
    }
    
    fclose(file);
    return count;
}

void analyze_logs(struct LogEntry* entries, int count) {
    if (count <= 0) return;
    
    int severity_count[5] = {0};
    int total_severity = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 0 && entries[i].severity < 5) {
            severity_count[entries[i].severity]++;
            total_severity += entries[i].severity;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("DEBUG: %d\n", severity_count[0]);
    printf("INFO: %d\n", severity_count[1]);
    printf("WARNING: %d\n", severity_count[2]);
    printf("ERROR: %d\n", severity_count[3]);
    printf("CRITICAL: %d\n", severity_count[4]);
    
    if (count > 0) {
        double avg_severity = (double)total_severity / count;
        printf("Average severity: %.2f\n", avg_severity);
    }
}

void process_heap_entries(struct LogEntry* heap_entries, int count) {
    if (!heap_entries || count <= 0) return;
    
    printf("Processing heap entries...\n");
    int error_count = 0;
    for (int i = 0; i < count; i++) {
        if (heap_entries[i].severity >= 3) {
            printf("High severity: %s - %s\n", heap_entries[i].level, heap_entries[i].message);
            error_count++;
        }
    }
    printf("Total high severity entries: %d\n", error_count);
}

int main() {
    char filename[MAX_FILENAME];
    struct LogEntry* heap_entries = NULL;
    struct LogEntry stack_entries[MAX_ENTRIES];
    
    printf("Enter log filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading filename\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen