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

int read_log_entries(struct LogEntry* entries, int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (count < max_entries && fgets(line, sizeof(line), stdin) != NULL) {
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed == 3) {
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
    
    return count;
}

void analyze_by_severity(struct LogEntry* entries, int count) {
    int severity_counts[5] = {0};
    int i = 0;
    
    do {
        if (entries[i].severity >= 0 && entries[i].severity < 5) {
            severity_counts[entries[i].severity]++;
        }
        i++;
    } while (i < count);
    
    printf("Severity Analysis:\n");
    const char* levels[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    for (int j = 0; j < 5; j++) {
        printf("  %s: %d\n", levels[j], severity_counts[j]);
    }
}

void find_errors(struct LogEntry* entries, int count) {
    printf("\nError and Critical Messages:\n");
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 3) {
            printf("  [%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
}

void search_messages(struct LogEntry* entries, int count, const char* keyword) {
    printf("\nMessages containing '%s':\n", keyword);
    int found = 0;
    int i = 0;
    
    while (i < count) {
        if (strstr(entries[i].message, keyword) != NULL) {
            printf("  [%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            found = 1;
        }
        i++;
    }
    
    if (!found) {
        printf("  No matches found.\n");
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (timestamp level message):\n");
    printf("Example: 2024-01-15T10:30:00 INFO User logged in\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to finish input.\n\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("Successfully read %d log entries.\n\n", count);
    
    analyze_by_severity(entries, count);
    find_errors(entries, count);
    
    char search_term[64];
    printf("\nEnter search term: ");
    if (fgets(search_term, sizeof(search_term), stdin) != NULL) {
        size_t len = strlen(search_term);
        if (len > 0 && search_term[len - 1] == '\n') {
            search_term[len - 1] = '\0';
        }
        if (strlen(search_term) > 0) {
            search_messages(entries, count, search_term);
        }
    }
    
    return 0;
}