//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 100

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LEN - 30];
};

int parse_log_line(char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char *ptr = line;
    int field = 0;
    
    while (*ptr && field < 3) {
        while (*ptr && isspace(*ptr)) ptr++;
        
        char *start = ptr;
        while (*ptr && !isspace(*ptr)) ptr++;
        
        size_t len = ptr - start;
        
        if (field == 0 && len < sizeof(entry->timestamp)) {
            strncpy(entry->timestamp, start, len);
            entry->timestamp[len] = '\0';
        } else if (field == 1 && len < sizeof(entry->level)) {
            strncpy(entry->level, start, len);
            entry->level[len] = '\0';
        } else if (field == 2) {
            while (*ptr && isspace(*ptr)) ptr++;
            char *msg_start = ptr;
            char *msg_end = msg_start;
            
            while (*msg_end && *msg_end != '\n') msg_end++;
            
            size_t msg_len = msg_end - msg_start;
            if (msg_len >= sizeof(entry->message)) {
                msg_len = sizeof(entry->message) - 1;
            }
            
            strncpy(entry->message, msg_start, msg_len);
            entry->message[msg_len] = '\0';
            break;
        }
        
        field++;
    }
    
    return (field >= 2);
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: timestamp level message):\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }
        
        char *newline = line;
        while (*newline && *newline != '\n') newline++;
        *newline = '\0';
        
        if (parse_log_line(line, entries + entry_count)) {
            entry_count++;
        } else {
            printf("Invalid log format. Skipping line.\n");
        }
    }
    
    printf("\nParsed %d log entries:\n", entry_count);
    printf("====================\n");
    
    struct LogEntry *current = entries;
    struct LogEntry *end = entries + entry_count;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    while (current < end) {
        printf("Time: %s | Level: %s | Message: %s\n", 
               current->timestamp, current->level, current->message);
        
        if (strcmp(current->level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(current->level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(current->level, "INFO") == 0) {
            info_count++;
        }
        
        current++;
    }
    
    printf("\nSummary:\n");
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    printf("Total entries: %d\n", entry_count);
    
    return 0;
}