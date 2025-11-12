//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
} LogEntry;

int parse_log_level(const char* level) {
    if (strcmp(level, "DEBUG") == 0) return 0;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "CRITICAL") == 0) return 4;
    return -1;
}

int validate_timestamp(const char* ts) {
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

int process_log_entry(const char* line, LogEntry* entry) {
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN - 1) return 0;
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';

    char* timestamp = strtok(temp, " ");
    if (!timestamp || !validate_timestamp(timestamp)) return 0;
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';

    char* level = strtok(NULL, " ");
    if (!level) return 0;
    int sev = parse_log_level(level);
    if (sev == -1) return 0;
    entry->severity = sev;
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';

    char* message = strtok(NULL, "\n");
    if (!message) return 0;
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';

    return 1;
}

void analyze_logs(LogEntry* logs, int count) {
    int severity_count[5] = {0};
    int total_chars = 0;
    int max_severity = 0;

    for (int i = 0; i < count; i++) {
        if (logs[i].severity >= 0 && logs[i].severity < 5) {
            severity_count[logs[i].severity]++;
            if (logs[i].severity > max_severity) {
                max_severity = logs[i].severity;
            }
        }
        total_chars += strlen(logs[i].message);
    }

    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("DEBUG: %d\n", severity_count[0]);
    printf("INFO: %d\n", severity_count[1]);
    printf("WARNING: %d\n", severity_count[2]);
    printf("ERROR: %d\n", severity_count[3]);
    printf("CRITICAL: %d\n", severity_count[4]);
    printf("Highest severity level: %s\n", 
           max_severity == 0 ? "DEBUG" :
           max_severity == 1 ? "INFO" :
           max_severity == 2 ? "WARNING" :
           max_severity == 3 ? "ERROR" : "CRITICAL");
    printf("Average message length: %.2f\n", count > 0 ? (double)total_chars / count : 0.0);
}

int main() {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    char line[MAX_LINE_LEN];

    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Enter 'END' on a separate line to finish input.\n");

    while (log_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
        if (strlen(line) > 0 && line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }
        if (strcmp(line, "END") == 0) {
            break;
        }
        if (strlen(line) == 0) {
            continue;
        }
        if (process_log_entry(line, &logs[log_count])) {
            log_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }

    if (log_count > 0) {
        analyze_logs(logs, log_count);
    } else {
        printf("No valid log entries to analyze.\n");
    }

    return 0;
}