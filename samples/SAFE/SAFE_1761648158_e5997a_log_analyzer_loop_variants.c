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

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LEN];
    strncpy(temp_line, line, MAX_LINE_LEN - 1);
    temp_line[MAX_LINE_LEN - 1] = '\0';
    
    char* parts[3];
    char* token = strtok(temp_line, "|");
    int part_count = 0;
    
    while (token != NULL && part_count < 3) {
        parts[part_count] = token;
        part_count++;
        token = strtok(NULL, "|");
    }
    
    if (part_count != 3) return 0;
    
    strncpy(entry->timestamp, parts[0], 31);
    entry->timestamp[31] = '\0';
    
    strncpy(entry->level, parts[1], 15);
    entry->level[15] = '\0';
    
    strncpy(entry->message, parts[2], 255);
    entry->message[255] = '\0';
    
    if (strcmp(entry->level, "ERROR") == 0) entry->severity = 3;
    else if (strcmp(entry->level, "WARN") == 0) entry->severity = 2;
    else if (strcmp(entry->level, "INFO") == 0) entry->severity = 1;
    else entry->severity = 0;
    
    return 1;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: timestamp|level|message), empty line to finish:\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, MAX_LINE_LEN, stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid format. Use: timestamp|level|message\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    int error_count = 0;
    int warn_count = 0;
    int info_count = 0;
    int other_count = 0;
    
    int i = 0;
    while (i < entry_count) {
        if (entries[i].severity == 3) error_count++;
        else if (entries[i].severity == 2) warn_count++;
        else if (entries[i].severity == 1) info_count++;
        else other_count++;
        i++;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("ERROR: %d\n", error_count);
    printf("WARN:  %d\n", warn_count);
    printf("INFO:  %d\n", info_count);
    printf("OTHER: %d\n", other_count);
    
    int max_severity = 0;
    int j = 0;
    do {
        if (entries[j].severity > max_severity) {
            max_severity = entries[j].severity;
        }
        j++;
    } while (j < entry_count);
    
    printf("Highest severity level: ");
    if (max_severity == 3) printf("ERROR\n");
    else if (max_severity == 2) printf("WARN\n");
    else if (max_severity == 1) printf("INFO\n");
    else printf("UNKNOWN\n");
    
    printf("\nRecent entries (last 3):\n");
    int start_idx = (entry_count > 3) ? entry_count - 3 : 0;
    
    for (int k = start_idx; k < entry_count; k++) {
        printf("%s | %s | %s\n", entries[k].timestamp, entries[k].level, entries[k].message);
    }
    
    return 0;
}