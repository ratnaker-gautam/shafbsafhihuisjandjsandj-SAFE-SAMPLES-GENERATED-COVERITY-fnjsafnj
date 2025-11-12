//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
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

int read_log_file(const char* filename, struct LogEntry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        if (strlen(line) < 10) continue;
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed >= 2) {
            strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp)-1);
            strncpy(entries[count].level, level, sizeof(entries[count].level)-1);
            if (parsed == 3) {
                strncpy(entries[count].message, message, sizeof(entries[count].message)-1);
            }
            entries[count].severity = parse_severity(level);
            count++;
        }
    }
    
    fclose(file);
    return count;
}

void analyze_logs(struct LogEntry* entries, int count) {
    int severity_counts[5] = {0};
    int total_entries = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 0 && entries[i].severity < 5) {
            severity_counts[entries[i].severity]++;
            total_entries++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries processed: %d\n", total_entries);
    printf("DEBUG: %d\n", severity_counts[0]);
    printf("INFO: %d\n", severity_counts[1]);
    printf("WARNING: %d\n", severity_counts[2]);
    printf("ERROR: %d\n", severity_counts[3]);
    printf("CRITICAL: %d\n", severity_counts[4]);
    
    int error_count = 0;
    int i = 0;
    while (i < count) {
        if (entries[i].severity >= 3) {
            printf("High severity: %s %s - %s\n", 
                   entries[i].timestamp, entries[i].level, entries[i].message);
            error_count++;
        }
        i++;
    }
    printf("Total high severity entries: %d\n", error_count);
    
    char last_timestamp[32] = {0};
    int duplicate_count = 0;
    for (int j = 0; j < count; j++) {
        if (strlen(entries[j].timestamp) > 0 && 
            strcmp(entries[j].timestamp, last_timestamp) == 0) {
            duplicate_count++;
        }
        strncpy(last_timestamp, entries[j].timestamp, sizeof(last_timestamp)-1);
    }
    printf("Duplicate timestamps: %d\n", duplicate_count);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = read_log_file(argv[1], entries, MAX_ENTRIES);
    
    if (entry_count < 0) {
        printf("Error: Could not open or read file '%s'\n", argv[1]);
        return 1;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found in '%s'\n", argv[1]);
        return 1;
    }
    
    analyze_logs(entries, entry_count);
    return 0;
}