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

int parse_log_level(const char* level) {
    if (strcmp(level, "DEBUG") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "WARNING") == 0) return 3;
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "CRITICAL") == 0) return 5;
    return 0;
}

int validate_timestamp(const char* timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char buffer[MAX_LINE_LEN];
    
    printf("Log Analyzer - Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\\n");
    printf("Enter 'END' on a separate line to finish input\\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        buffer[strcspn(buffer, "\\n")] = 0;
        
        if (strcmp(buffer, "END") == 0) {
            break;
        }
        
        if (strlen(buffer) == 0) {
            continue;
        }
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(buffer, "%31s %15s %255[^\\n]", timestamp, level, message);
        if (parsed != 3) {
            printf("Invalid format. Use: YYYY-MM-DD HH:MM:SS LEVEL message\\n");
            continue;
        }
        
        if (!validate_timestamp(timestamp)) {
            printf("Invalid timestamp format: %s\\n", timestamp);
            continue;
        }
        
        int severity = parse_log_level(level);
        if (severity == 0) {
            printf("Invalid log level: %s\\n", level);
            continue;
        }
        
        if (strlen(message) == 0) {
            printf("Message cannot be empty\\n");
            continue;
        }
        
        struct LogEntry* entry = &entries[entry_count];
        strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
        strncpy(entry->level, level, sizeof(entry->level) - 1);
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->severity = severity;
        entry_count++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze\\n");
        return 0;
    }
    
    int* severity_counts = malloc(6 * sizeof(int));
    if (severity_counts == NULL) {
        printf("Memory allocation failed\\n");
        return 1;
    }
    
    for (int i = 0; i < 6; i++) {
        severity_counts[i] = 0;
    }
    
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].severity >= 1 && entries[i].severity <= 5) {
            severity_counts[entries[i].severity]++;
        }
    }
    
    printf("\\nLog Analysis Results:\\n");
    printf("Total entries: %d\\n", entry_count);
    printf("DEBUG: %d\\n", severity_counts[1]);
    printf("INFO: %d\\n", severity_counts[2]);
    printf("WARNING: %d\\n", severity_counts[3]);
    printf("ERROR: %d\\n", severity_counts[4]);
    printf("CRITICAL: %d\\n", severity_counts[5]);
    
    int max_severity = 0;
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].severity > max_severity) {
            max_severity = entries[i].severity;
        }
    }
    
    printf("\\nHighest severity entries:\\n");
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].severity == max_severity) {
            printf("%s %s %s\\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
    
    free(severity_counts);
    return 0;
}