//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 100

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LEN];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LEN];
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    char* parts[3];
    int part_count = 0;
    char* token = strtok(temp, "|");
    
    while (token != NULL && part_count < 3) {
        parts[part_count++] = token;
        token = strtok(NULL, "|");
    }
    
    if (part_count != 3) return 0;
    
    if (strlen(parts[0]) >= 20 || strlen(parts[1]) >= 10 || strlen(parts[2]) >= MAX_LINE_LEN) {
        return 0;
    }
    
    strncpy(entry->timestamp, parts[0], 19);
    entry->timestamp[19] = '\0';
    strncpy(entry->level, parts[1], 9);
    entry->level[9] = '\0';
    strncpy(entry->message, parts[2], MAX_LINE_LEN - 1);
    entry->message[MAX_LINE_LEN - 1] = '\0';
    
    return 1;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: timestamp|level|message):\n");
    printf("Type 'END' on a separate line to finish.\n");
    
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
            entries[entry_count] = entry;
            entry_count++;
        } else {
            printf("Invalid log format. Use: YYYY-MM-DD HH:MM:SS|LEVEL|message\n");
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    int i = 0;
    while (i < entry_count) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, "WARN") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        }
        i++;
    }
    
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARN entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nRecent ERROR entries:\n");
    int found_errors = 0;
    
    for (int j = entry_count - 1; j >= 0 && found_errors < 3; j--) {
        if (strcmp(entries[j].level, "ERROR") == 0) {
            printf("- %s: %s\n", entries[j].timestamp, entries[j].message);
            found_errors++;
        }
    }
    
    if (found_errors == 0) {
        printf("No ERROR entries found.\n");
    }
    
    printf("\nSearch for messages (empty to skip): ");
    if (fgets(line, sizeof(line), stdin) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) > 0) {
            printf("Matching entries:\n");
            int match_count = 0;
            
            for (int k = 0; k < entry_count; k++) {
                if (strstr(entries[k].message, line) != NULL) {
                    printf("- [%s] %s: %s\n", entries[k].level, entries[k].timestamp, entries[k].message);
                    match_count++;
                }
            }
            
            if (match_count == 0) {
                printf("No matches found.\n");
            }
        }
    }
    
    return 0;
}