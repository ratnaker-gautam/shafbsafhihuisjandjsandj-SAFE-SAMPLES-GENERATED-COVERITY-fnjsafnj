//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LEN - 30];
};

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (!line || !entry) return 0;
    
    const char *ptr = line;
    int field = 0;
    char *dest = NULL;
    size_t len = 0;
    
    while (*ptr && field < 3) {
        while (*ptr && isspace(*ptr)) ptr++;
        if (!*ptr) break;
        
        const char *start = ptr;
        while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n') ptr++;
        
        size_t field_len = ptr - start;
        if (field_len == 0) break;
        
        switch (field) {
            case 0:
                dest = entry->timestamp;
                len = sizeof(entry->timestamp) - 1;
                break;
            case 1:
                dest = entry->level;
                len = sizeof(entry->level) - 1;
                break;
            case 2:
                dest = entry->message;
                len = sizeof(entry->message) - 1;
                break;
        }
        
        if (field_len > len) field_len = len;
        if (dest) {
            memcpy(dest, start, field_len);
            *(dest + field_len) = '\0';
        }
        
        field++;
    }
    
    return field >= 2;
}

void analyze_log_levels(struct LogEntry *entries, int count) {
    if (!entries || count <= 0) return;
    
    int info_count = 0, warn_count = 0, error_count = 0, other_count = 0;
    
    for (int i = 0; i < count; i++) {
        char *level = entries[i].level;
        if (strcmp(level, "INFO") == 0) info_count++;
        else if (strcmp(level, "WARN") == 0) warn_count++;
        else if (strcmp(level, "ERROR") == 0) error_count++;
        else other_count++;
    }
    
    printf("Log Level Analysis:\n");
    printf("INFO:  %d\n", info_count);
    printf("WARN:  %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("OTHER: %d\n", other_count);
}

void search_messages(struct LogEntry *entries, int count, const char *term) {
    if (!entries || count <= 0 || !term || !*term) return;
    
    printf("Messages containing '%s':\n", term);
    int found = 0;
    
    for (int i = 0; i < count; i++) {
        char *msg = entries[i].message;
        char *ptr = msg;
        
        while (*ptr) {
            char *match = ptr;
            const char *search = term;
            
            while (*match && *search && *match == *search) {
                match++;
                search++;
            }
            
            if (!*search) {
                printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
                found++;
                break;
            }
            
            ptr++;
        }
    }
    
    if (!found) {
        printf("No matches found.\n");
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (empty line to finish):\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\n') break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (parse_log_line(line, entries + entry_count)) {
            entry_count++;
        } else {
            printf("Invalid log format. Use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    printf("\nParsed %d log entries.\n\n", entry_count);
    
    analyze_log_levels(entries, entry_count);
    
    printf("\n");
    char search_term[50];
    printf("Enter search term: ");
    if (fgets(search_term, sizeof(search_term), stdin)) {
        size_t len = strlen(search_term);
        if (len > 0 && search_term[len - 1] == '\n') {
            search_term[len - 1] = '\0';
        }
        if (search_term[0]) {
            printf("\n");
            search_messages(entries, entry_count, search_term);
        }
    }
    
    return 0;
}