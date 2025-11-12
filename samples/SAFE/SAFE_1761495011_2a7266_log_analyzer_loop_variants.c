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

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES) {
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
        
        struct LogEntry entry;
        char level_buf[16];
        char message_buf[256];
        int parsed = sscanf(line, "%31s %15s %255[^\n]", 
                           entry.timestamp, level_buf, message_buf);
        
        if (parsed != 3) {
            printf("Invalid format. Skipping: %s\n", line);
            continue;
        }
        
        if (!is_valid_timestamp(entry.timestamp)) {
            printf("Invalid timestamp. Skipping: %s\n", line);
            continue;
        }
        
        strncpy(entry.level, level_buf, sizeof(entry.level) - 1);
        entry.level[sizeof(entry.level) - 1] = '\0';
        strncpy(entry.message, message_buf, sizeof(entry.message) - 1);
        entry.message[sizeof(entry.message) - 1] = '\0';
        
        entry.severity = parse_log_level(entry.level);
        if (entry.severity == -1) {
            printf("Unknown log level. Skipping: %s\n", line);
            continue;
        }
        
        logs[entry_count] = entry;
        entry_count++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    int level_counts[5] = {0};
    char* level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    int i = 0;
    while (i < entry_count) {
        if (logs[i].severity >= 0 && logs[i].severity < 5) {
            level_counts[logs[i].severity]++;
        }
        i++;
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("====================\n");
    printf("Total entries: %d\n", entry_count);
    
    for (int j = 0; j < 5; j++) {
        printf("%-8s: %d entries\n", level_names[j], level_counts[j]);
    }
    
    int error_count = 0;
    int critical_count = 0;
    
    int k = 0;
    do {
        if (logs[k].severity == 3) error_count++;
        if (logs[k].severity == 4) critical_count++;
        k++;
    } while (k < entry_count);
    
    printf("\nHigh Severity Issues:\n");
    printf("Errors: %d, Critical: %d\n", error_count, critical_count);
    
    printf("\nRecent entries (last 5):\n");
    int start = (entry_count > 5) ? entry_count - 5 : 0;
    for (int idx = start; idx < entry_count; idx++) {
        printf("%s %s %s\n", logs[idx].timestamp, logs[idx].level, logs[idx].message);
    }
    
    return 0;
}