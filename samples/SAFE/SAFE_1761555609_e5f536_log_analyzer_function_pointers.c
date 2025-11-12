//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_entries(LogEntry* entry) {
    static int count = 0;
    count++;
    printf("Processed %d entries\n", count);
}

void show_errors(LogEntry* entry) {
    if (strcmp(entry->level, "ERROR") == 0) {
        printf("ERROR: %s - %s\n", entry->timestamp, entry->message);
    }
}

void show_warnings(LogEntry* entry) {
    if (strcmp(entry->level, "WARN") == 0) {
        printf("WARNING: %s - %s\n", entry->timestamp, entry->message);
    }
}

void process_log_file(const char* filename, LogProcessor processor) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return;
    }

    char line[MAX_LINE_LENGTH];
    int line_count = 0;

    while (fgets(line, sizeof(line), file)) {
        line_count++;
        if (line_count > MAX_ENTRIES) {
            fprintf(stderr, "Warning: Maximum entries reached\n");
            break;
        }

        if (strlen(line) >= MAX_LINE_LENGTH - 1) {
            fprintf(stderr, "Error: Line too long at line %d\n", line_count);
            continue;
        }

        LogEntry entry;
        int parsed = sscanf(line, "%19s %9s %[^\n]", 
                           entry.timestamp, entry.level, entry.message);
        
        if (parsed == 3) {
            processor(&entry);
        } else {
            fprintf(stderr, "Warning: Invalid log format at line %d\n", line_count);
        }
    }

    if (fclose(file) != 0) {
        fprintf(stderr, "Error: Failed to close file\n");
    }
}

int main() {
    char filename[256];
    
    printf("Enter log filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len-1] == '\n') {
        filename[len-1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        fprintf(stderr, "Error: Empty filename\n");
        return 1;
    }

    printf("\n1. Count all entries\n");
    printf("2. Show errors only\n");
    printf("3. Show warnings only\n");
    printf("Choose option: ");
    
    char choice[10];
    if (fgets(choice, sizeof(choice), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    LogProcessor processors[] = {count_entries, show_errors, show_warnings};
    int option = atoi(choice);
    
    if (option < 1 || option > 3) {
        fprintf(stderr, "Error: Invalid option\n");
        return 1;
    }
    
    process_log_file(filename, processors[option - 1]);
    
    return 0;
}