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
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARN") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "DEBUG") == 0) return 0;
    return -1;
}

int read_log_entries(struct LogEntry* entries, int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
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
    int severity_counts[4] = {0};
    int i = 0;
    
    do {
        if (entries[i].severity >= 0 && entries[i].severity < 4) {
            severity_counts[entries[i].severity]++;
        }
        i++;
    } while (i < count);
    
    printf("Severity Analysis:\n");
    printf("DEBUG: %d\n", severity_counts[0]);
    printf("INFO:  %d\n", severity_counts[1]);
    printf("WARN:  %d\n", severity_counts[2]);
    printf("ERROR: %d\n", severity_counts[3]);
}

void find_most_frequent_level(struct LogEntry* entries, int count) {
    int level_counts[4] = {0};
    const char* level_names[] = {"DEBUG", "INFO", "WARN", "ERROR"};
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 0 && entries[i].severity < 4) {
            level_counts[entries[i].severity]++;
        }
    }
    
    int max_count = 0;
    int max_level = 0;
    int j = 0;
    
    while (j < 4) {
        if (level_counts[j] > max_count) {
            max_count = level_counts[j];
            max_level = j;
        }
        j++;
    }
    
    printf("Most frequent level: %s (%d occurrences)\n", level_names[max_level], max_count);
}

void search_messages(struct LogEntry* entries, int count, const char* keyword) {
    printf("Messages containing '%s':\n", keyword);
    int found = 0;
    int k = 0;
    
    for (; k < count; k++) {
        if (strstr(entries[k].message, keyword) != NULL) {
            printf("%s %s: %s\n", entries[k].timestamp, entries[k].level, entries[k].message);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No matches found.\n");
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Press Ctrl+D (Linux) to finish input.\n\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("\nRead %d log entries\n\n", count);
    
    analyze_by_severity(entries, count);
    printf("\n");
    
    find_most_frequent_level(entries, count);
    printf("\n");
    
    char search_term[64];
    printf("Enter search term: ");
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