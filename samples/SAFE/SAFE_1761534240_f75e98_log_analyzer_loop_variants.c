//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
};

int parse_log_level(const char* level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARN") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "DEBUG") == 0) return 0;
    return -1;
}

int read_log_file(const char* filename, struct LogEntry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        if (strlen(line) < 10) continue;
        
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
                }
                entries[count].severity = parse_log_level(level);
                count++;
            }
        }
    }
    
    fclose(file);
    return count;
}

void analyze_logs(struct LogEntry* entries, int count) {
    int level_counts[4] = {0};
    int total_severity = 0;
    int valid_entries = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 0) {
            level_counts[entries[i].severity]++;
            total_severity += entries[i].severity;
            valid_entries++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries processed: %d\n", count);
    printf("Valid entries: %d\n", valid_entries);
    printf("\nBreakdown by level:\n");
    printf("DEBUG: %d\n", level_counts[0]);
    printf("INFO: %d\n", level_counts[1]);
    printf("WARN: %d\n", level_counts[2]);
    printf("ERROR: %d\n", level_counts[3]);
    
    if (valid_entries > 0) {
        printf("Average severity: %.2f\n", (double)total_severity / valid_entries);
    }
    
    int error_count = 0;
    int i = 0;
    while (i < count && error_count < 5) {
        if (entries[i].severity == 3) {
            printf("Recent ERROR: %s - %s\n", entries[i].timestamp, entries[i].message);
            error_count++;
        }
        i++;
    }
    
    int consecutive_errors = 0;
    int max_consecutive = 0;
    for (int j = 0; j < count; j++) {
        if (entries[j].severity == 3) {
            consecutive_errors++;
            if (consecutive_errors > max_consecutive) {
                max_consecutive = consecutive_errors;
            }
        } else {
            consecutive_errors = 0;
        }
    }
    printf("Max consecutive errors: %d\n", max_consecutive);
}

int main(void) {
    char filename[256];
    printf("Enter log file name: ");
    
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        fprintf(stderr, "Invalid filename\n");
        return 1;
    }
    
    struct LogEntry entries[MAX_ENTRIES];
    int count = read_log_file(filename, entries, MAX_ENTRIES);
    
    if (count < 0) {
        fprintf(stderr, "Error opening or reading file: %s\n", filename);
        return 1;
    }
    
    if (count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }
    
    analyze_logs(entries, count);
    return 0;
}