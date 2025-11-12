//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LEN];
    strncpy(temp_line, line, MAX_LINE_LEN - 1);
    temp_line[MAX_LINE_LEN - 1] = '\0';
    
    char* parts[3];
    char* token = strtok(temp_line, "|");
    int part_count = 0;
    
    while (token != NULL && part_count < 3) {
        parts[part_count++] = token;
        token = strtok(NULL, "|");
    }
    
    if (part_count != 3) return 0;
    
    if (strlen(parts[0]) >= sizeof(entry->timestamp) ||
        strlen(parts[1]) >= sizeof(entry->level) ||
        strlen(parts[2]) >= sizeof(entry->message)) {
        return 0;
    }
    
    strncpy(entry->timestamp, parts[0], sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, parts[1], sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, parts[2], sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int is_valid_level(const char* level) {
    const char* valid_levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        if (strcmp(level, valid_levels[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: timestamp|level|message):\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (is_valid_level(entry.level)) {
                entries[entry_count] = entry;
                entry_count++;
            } else {
                printf("Invalid log level: %s\n", entry.level);
            }
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    int info_count = 0, warning_count = 0, error_count = 0, debug_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARNING") == 0) warning_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(entries[i].level, "DEBUG") == 0) debug_count++;
    }
    
    printf("Total entries: %d\n", entry_count);
    printf("INFO: %d\n", info_count);
    printf("WARNING: %d\n", warning_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);
    
    printf("\nRecent entries:\n");
    int display_count = (entry_count < 5) ? entry_count : 5;
    for (int i = entry_count - display_count; i < entry_count; i++) {
        printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    return 0;
}