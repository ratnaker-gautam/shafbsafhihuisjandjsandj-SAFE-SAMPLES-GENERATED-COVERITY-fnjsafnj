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
    
    int i = 0;
    while (temp[i] != '\0' && i < 19) {
        if (temp[i] == ' ') break;
        entry->timestamp[i] = temp[i];
        i++;
    }
    entry->timestamp[i] = '\0';
    
    while (temp[i] != '\0' && isspace(temp[i])) i++;
    
    int j = 0;
    while (temp[i] != '\0' && !isspace(temp[i]) && j < 9) {
        entry->level[j] = temp[i];
        i++;
        j++;
    }
    entry->level[j] = '\0';
    
    while (temp[i] != '\0' && isspace(temp[i])) i++;
    
    j = 0;
    while (temp[i] != '\0' && j < MAX_LINE_LENGTH - 31) {
        entry->message[j] = temp[i];
        i++;
        j++;
    }
    entry->message[j] = '\0';
    
    return 1;
}

void analyze_log_levels(struct LogEntry* entries, int count) {
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    int k = 0;
    do {
        if (strcmp(entries[k].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[k].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[k].level, "INFO") == 0) {
            info_count++;
        }
        k++;
    } while (k < count);
    
    printf("Log Level Summary:\n");
    printf("ERROR: %d\n", error_count);
    printf("WARNING: %d\n", warning_count);
    printf("INFO: %d\n", info_count);
}

void display_recent_entries(struct LogEntry* entries, int count) {
    printf("\nRecent Log Entries:\n");
    
    int start = (count > 5) ? count - 5 : 0;
    
    for (int i = start; i < count; i++) {
        printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (empty line to finish):\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (line[0] == '\0') break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    analyze_log_levels(entries, entry_count);
    display_recent_entries(entries, entry_count);
    
    printf("\nSearch for entries containing text: ");
    if (fgets(line, sizeof(line), stdin) != NULL) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (line[0] != '\0') {
            printf("Matching entries:\n");
            int found_count = 0;
            char* search_term = line;
            
            int m = 0;
            while (m < entry_count) {
                if (strstr(entries[m].message, search_term) != NULL) {
                    printf("[%s] %s: %s\n", entries[m].timestamp, entries[m].level, entries[m].message);
                    found_count++;
                }
                m++;
            }
            
            if (found_count == 0) {
                printf("No matches found.\n");
            }
        }
    }
    
    return 0;
}