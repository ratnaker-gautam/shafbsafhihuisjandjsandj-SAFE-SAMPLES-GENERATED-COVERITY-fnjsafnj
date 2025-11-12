//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256
#define MAX_ENTRIES 100

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH - 30];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
    char* parts[3];
    parts[0] = temp;
    int part_count = 1;
    
    for (char* p = temp; *p && part_count < 3; p++) {
        if (*p == '|') {
            *p = '\0';
            parts[part_count] = p + 1;
            part_count++;
        }
    }
    
    if (part_count != 3) return 0;
    
    if (strlen(parts[0]) >= 19) return 0;
    strncpy(entry->timestamp, parts[0], 19);
    entry->timestamp[19] = '\0';
    
    if (strlen(parts[1]) >= 9) return 0;
    strncpy(entry->level, parts[1], 9);
    entry->level[9] = '\0';
    
    if (strlen(parts[2]) >= MAX_LINE_LENGTH - 31) return 0;
    strncpy(entry->message, parts[2], MAX_LINE_LENGTH - 31);
    entry->message[MAX_LINE_LENGTH - 31] = '\0';
    
    return 1;
}

int main() {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: timestamp|level|message):\n");
    printf("Type 'END' on a separate line to finish.\n");
    
    char line[MAX_LINE_LENGTH];
    int i = 0;
    while (i < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
            i++;
        } else {
            printf("Invalid log format. Skipping.\n");
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    int j = 0;
    do {
        if (strcmp(entries[j].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[j].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[j].level, "INFO") == 0) {
            info_count++;
        }
        j++;
    } while (j < entry_count);
    
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nRecent ERROR entries:\n");
    int found_errors = 0;
    for (int k = entry_count - 1; k >= 0 && found_errors < 3; k--) {
        if (strcmp(entries[k].level, "ERROR") == 0) {
            printf("- %s: %s\n", entries[k].timestamp, entries[k].message);
            found_errors++;
        }
    }
    
    if (found_errors == 0) {
        printf("No ERROR entries found.\n");
    }
    
    return 0;
}