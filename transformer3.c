#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Entry {
    char key[50];       // Stores Agent ID or State Code
    double max_rating;  // Stores the maximum rating found so far
    int is_set;
};

int main() {
    // Create buffer
    char line[1024];
    struct Entry agents[200];
    struct Entry states[100];

    // Initialize arrays
    memset(agents, 0, sizeof(agents));
    memset(states, 0, sizeof(states));

    while (fgets(line, sizeof(line), stdin)) {
        if (strlen(line) < 5)
            continue;

        // Replace ", " with "|"
        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] == ',' && line[i+1] == ' ') {
                line[i] = '|';
            }
        }

        // Tokenize
        char *agent_id = strtok(line, "|");

        char *real_estate_location = strtok(NULL, "|");
        if (real_estate_location[0] == ' ')
            real_estate_location++;

        char *customer_rating_str = strtok(NULL, "|");
        if (customer_rating_str && customer_rating_str[0] == ' ')
            customer_rating_str++;
        
        if (customer_rating_str) {
            customer_rating_str[strcspn(customer_rating_str, "\n")] = 0;
        }

        // Convert customer rating to double
        double rating = atof(customer_rating_str);

        // Update Agent Max
        int found = -1;
        for (int i = 0; i < 200; i++) {
            if (agents[i].is_set && strcmp(agents[i].key, agent_id) == 0) {
                found = i; 
                break;
            }
        }

        if (found == -1) {
            for (int i = 0; i < 200; i++) {
                if (!agents[i].is_set) {
                    found = i; 
                    agents[i].is_set = 1;
                    strcpy(agents[i].key, agent_id);
                    agents[i].max_rating = -1.0;
                    break;
                }
            }
        }

        if (found != -1 && rating > agents[found].max_rating) {
            agents[found].max_rating = rating;
        }

        // Update State Max
        found = -1;
        for (int i = 0; i < 100; i++) {
            if (states[i].is_set && strcmp(states[i].key, real_estate_location) == 0) {
                found = i; 
                break;
            }
        }

        if (found == -1) {
            for (int i = 0; i < 100; i++) {
                if (!states[i].is_set) {
                    found = i; 
                    states[i].is_set = 1;
                    strcpy(states[i].key, real_estate_location);
                    states[i].max_rating = -1.0;
                    break;
                }
            }
        }

        if (found != -1 && rating > states[found].max_rating) {
            states[found].max_rating = rating;
        }
    }

    // Prints stdout
    for (int i = 0; i < 200; i++) {
        if (agents[i].is_set) {
            fprintf(stdout, "%s, %.1f\n", agents[i].key, agents[i].max_rating);
        }
    }
    
    // Prints stderr
    for (int i = 0; i < 100; i++) {
        if (states[i].is_set) {
            fprintf(stderr, "%s, %.1f\n", states[i].key, states[i].max_rating);
        }
    }
    return 0;
}
