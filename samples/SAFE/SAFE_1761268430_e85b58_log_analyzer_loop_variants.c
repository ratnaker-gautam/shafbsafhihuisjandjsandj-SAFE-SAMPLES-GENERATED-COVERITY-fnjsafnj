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

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    int count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        char timestamp[32], level[16], message[256];
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        
        if (parsed != 3) {
            printf("Invalid format. Skipping: %s\n", line);
            continue;
        }
        
        if (!is_valid_timestamp(timestamp)) {
            printf("Invalid timestamp. Skipping: %s\n", timestamp);
            continue;
        }
        
        int severity = parse_severity(level);
        if (severity == 0) {
            printf("Unknown log level. Skipping: %s\n", level);
            continue;
        }
        
        strncpy(logs[count].timestamp, timestamp, sizeof(logs[count].timestamp) - 1);
        strncpy(logs[count].level, level, sizeof(logs[count].level) - 1);
        strncpy(logs[count].message, message, sizeof(logs[count].message) - 1);
        logs[count].severity = severity;
        logs[count].timestamp[31] = '\0';
        logs[count].level[15] = '\0';
        logs[count].message[255] = '\0';
        
        count++;
    }
    
    if (count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    int severity_counts[6] = {0};
    int i = 0;
    do {
        severity_counts[logs[i].severity]++;
        i++;
    } while (i < count);
    
    printf("Total entries: %d\n", count);
    printf("By severity level:\n");
    const char* levels[] = {"", "DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    for (int j = 1; j <= 5; j++) {
        printf("  %s: %d\n", levels[j], severity_counts[j]);
    }
    
    printf("\nEntries by severity (highest first):\n");
    for (int sev = 5; sev >= 1; sev--) {
        int found = 0;
        int k = 0;
        while (k < count) {
            if (logs[k].severity == sev) {
                if (!found) {
                    printf("\n%s entries:\n", levels[sev]);
                    found = 1;
                }
                printf("  [%s] %s\n", logs[k].timestamp, logs[k].message);
            }
            k++;
        }
    }
    
    int error_count = severity_counts[4] + severity_counts[5];
    if (error_count > 0) {
        printf("\nALERT: Found %d error/critical entries requiring attention!\n", error_count);
    } else {
        printf("\nNo critical errors found.\n");
    }
    
    return 0;
}