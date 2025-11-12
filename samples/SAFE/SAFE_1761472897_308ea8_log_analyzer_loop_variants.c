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
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
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
    
    return 1;
}

void analyze_logs(struct LogEntry* entries, int count) {
    if (!entries || count <= 0) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    int i = 0;
    while (i < count) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        }
        i++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nRecent ERROR entries:\n");
    int found = 0;
    for (int j = count - 1; j >= 0 && found < 3; j--) {
        if (strcmp(entries[j].level, "ERROR") == 0) {
            printf("- %s: %s\n", entries[j].timestamp, entries[j].message);
            found++;
        }
    }
    
    if (found == 0) {
        printf("No ERROR entries found.\n");
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: timestamp|level|message):\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    char line[MAX_LINE_LENGTH];
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", line);
        }
    }
    
    if (entry_count > 0) {
        analyze_logs(entries, entry_count);
    } else {
        printf("No valid log entries to analyze.\n");
    }
    
    return 0;
}