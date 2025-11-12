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
    if (strcmp(level, "DEBUG") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "WARNING") == 0) return 3;
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "CRITICAL") == 0) return 5;
    return 0;
}

int read_log_file(const char* filename, struct LogEntry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && count < max_entries) {
        if (strlen(line) >= sizeof(line) - 1) continue;
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed == 3) {
            strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
            strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
            strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
            entries[count].severity = parse_severity(level);
            count++;
        }
    }
    
    fclose(file);
    return count;
}

void analyze_logs(struct LogEntry* entries, int count) {
    int severity_counts[6] = {0};
    int total_severity = 0;
    int valid_entries = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity > 0) {
            severity_counts[entries[i].severity]++;
            total_severity += entries[i].severity;
            valid_entries++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries processed: %d\n", count);
    printf("Valid entries: %d\n", valid_entries);
    
    const char* levels[] = {"", "DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    int i = 1;
    while (i <= 5) {
        printf("%s: %d\n", levels[i], severity_counts[i]);
        i++;
    }
    
    if (valid_entries > 0) {
        double avg_severity = (double)total_severity / valid_entries;
        printf("Average severity: %.2f\n", avg_severity);
    }
    
    int error_count = 0;
    int j = 0;
    do {
        if (entries[j].severity >= 4) {
            error_count++;
        }
        j++;
    } while (j < count);
    
    printf("High severity entries (ERROR+): %d\n", error_count);
    
    struct LogEntry* current = entries;
    int warning_count = 0;
    for (int k = 0; k < count; k++) {
        if (current->severity == 3) {
            warning_count++;
        }
        current++;
    }
    printf("Warnings: %d\n", warning_count);
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log filename: ");
    char filename[256];
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
    
    int entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    if (entry_count < 0) {
        printf("Error opening or reading file: %s\n", filename);
        return 1;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }
    
    analyze_logs(entries, entry_count);
    
    return 0;
}