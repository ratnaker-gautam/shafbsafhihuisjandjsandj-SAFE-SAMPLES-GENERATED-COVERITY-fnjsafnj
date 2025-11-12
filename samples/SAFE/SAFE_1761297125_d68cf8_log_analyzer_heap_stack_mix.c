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
    if (strcmp(level, "DEBUG") == 0) return 0;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "CRITICAL") == 0) return 4;
    return -1;
}

int read_log_file(const char* filename, struct LogEntry** entries) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
    char line[MAX_LINE_LEN];
    int count = 0;
    struct LogEntry* temp_entries = malloc(MAX_ENTRIES * sizeof(struct LogEntry));
    if (!temp_entries) {
        fclose(file);
        return -1;
    }
    
    while (fgets(line, sizeof(line), file) && count < MAX_ENTRIES) {
        char ts[32], lvl[16], msg[256];
        if (sscanf(line, "%31s %15s %255[^\n]", ts, lvl, msg) == 3) {
            strncpy(temp_entries[count].timestamp, ts, sizeof(temp_entries[count].timestamp) - 1);
            strncpy(temp_entries[count].level, lvl, sizeof(temp_entries[count].level) - 1);
            strncpy(temp_entries[count].message, msg, sizeof(temp_entries[count].message) - 1);
            temp_entries[count].severity = parse_severity(lvl);
            if (temp_entries[count].severity >= 0) {
                count++;
            }
        }
    }
    
    fclose(file);
    
    if (count > 0) {
        *entries = malloc(count * sizeof(struct LogEntry));
        if (*entries) {
            memcpy(*entries, temp_entries, count * sizeof(struct LogEntry));
        } else {
            count = -1;
        }
    }
    
    free(temp_entries);
    return count;
}

void analyze_logs(struct LogEntry* entries, int count) {
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

int main() {
    char filename[256];
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    struct LogEntry* entries = NULL;
    int count = read_log_file(filename, &entries);
    
    if (count <= 0) {
        printf("Error reading log file or no valid entries found\n");
        if (entries) free(entries);
        return 1;
    }
    
    analyze_logs(entries, count);
    
    free(entries);
    return 0;
}