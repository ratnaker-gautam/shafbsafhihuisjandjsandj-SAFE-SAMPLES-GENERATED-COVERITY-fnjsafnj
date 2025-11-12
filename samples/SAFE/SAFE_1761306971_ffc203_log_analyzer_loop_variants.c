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
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        if (sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message) == 3) {
            int severity = parse_severity(level);
            if (severity >= 0) {
                strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
                strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
                strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
                entries[count].severity = severity;
                count++;
            }
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
    printf("Total entries: %d\n", total_entries);
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
    printf("High severity entries found: %d\n", error_count);
    
    char search_term[64];
    printf("Enter search term (or 'quit' to exit): ");
    if (fgets(search_term, sizeof(search_term), stdin)) {
        search_term[strcspn(search_term, "\n")] = 0;
        
        if (strcmp(search_term, "quit") != 0) {
            int found = 0;
            for (int j = 0; j < count; j++) {
                if (strstr(entries[j].message, search_term)) {
                    printf("Found: %s %s - %s\n", 
                           entries[j].timestamp, entries[j].level, entries[j].message);
                    found++;
                }
            }
            printf("Search results: %d entries found\n", found);
        }
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    
    printf("Log Analyzer\n");
    printf("Enter log filename: ");
    
    char filename[256];
    if (!fgets(filename, sizeof(filename), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    
    filename[strcspn(filename, "\n")] = 0;
    
    if (strlen(filename) == 0) {
        printf("No filename provided\n");
        return 1;
    }
    
    int entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    if (entry_count < 0) {
        printf("Error opening or reading file: %s\n", filename);
        return 1;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }
    
    printf("Successfully read %d log entries\n", entry_count);
    analyze_logs(entries, entry_count);
    
    return 0;
}