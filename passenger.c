#include "passenger.h"
#include "utils.h"

int passengerExists(int p_id) {
    FILE *fp = fopen(PASSENGER_FILE, "rb");
    if (!fp) return 0;

    Passenger p;
    while (fread(&p, sizeof(Passenger), 1, fp) == 1) {
        if (p.p_id == p_id) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

int findPassenger(int p_id, Passenger *outPass) {
    FILE *fp = fopen(PASSENGER_FILE, "rb");
    if (!fp) return 0;

    Passenger p;
    while (fread(&p, sizeof(Passenger), 1, fp) == 1) {
        if (p.p_id == p_id) {
            if (outPass) *outPass = p;
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

int getPassengerCount(void) {
    FILE *fp = fopen(PASSENGER_FILE, "rb");
    if (!fp) return 0;

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    return (int)(size / sizeof(Passenger));
}

int insertPassenger(void) {
    clearScreen();
    printBanner("PASSENGER MANAGEMENT - INSERT A RECORD");

    Passenger p;
    memset(&p, 0, sizeof(Passenger));

    printf("Enter Passenger Details:\n");
    printf("--------------------------------------------------------------------------------\n");

    while (1) {
        p.p_id = getInt(" 1. Enter Passenger ID (p_id, e.g. 201): ", 1, 99999);
        if (passengerExists(p.p_id)) {
            printf(" [!] Error: Passenger with ID %d already exists. Enter a unique ID.\n", p.p_id);
        } else {
            break;
        }
    }

    getString(" 2. Enter Passenger Name (p_name, max 20 chars): ", p.p_name, 20, 0);
    getString(" 3. Enter Passenger Address (p_addr, max 40 chars): ", p.p_addr, 40, 0);

    FILE *fp = fopen(PASSENGER_FILE, "ab");
    if (!fp) {
        printf("\n [!] Error opening file %s for writing!\n", PASSENGER_FILE);
        pauseScreen();
        return 0;
    }

    if (fwrite(&p, sizeof(Passenger), 1, fp) == 1) {
        fclose(fp);
        printf("\n [OK] Passenger record (ID: %d - %s) successfully inserted into %s!\n", p.p_id, p.p_name, PASSENGER_FILE);
    } else {
        fclose(fp);
        printf("\n [!] Error writing record to %s!\n", PASSENGER_FILE);
    }

    pauseScreen();
    return 1;
}

int editPassenger(void) {
    clearScreen();
    printBanner("PASSENGER MANAGEMENT - EDIT A RECORD");

    FILE *fp = fopen(PASSENGER_FILE, "rb");
    if (!fp) {
        printf(" [!] No passenger records found in %s.\n", PASSENGER_FILE);
        pauseScreen();
        return 0;
    }

    int target_id = getInt("Enter Passenger ID to edit: ", 1, 99999);
    Passenger p;
    int found = 0;

    FILE *temp = fopen("passenger_temp.dat", "wb");
    if (!temp) {
        printf(" [!] Error creating temporary file.\n");
        fclose(fp);
        pauseScreen();
        return 0;
    }

    while (fread(&p, sizeof(Passenger), 1, fp) == 1) {
        if (p.p_id == target_id) {
            found = 1;
            printf("\n--- Current Passenger Details (ID: %d) ---\n", p.p_id);
            printf(" Name    : %s\n", p.p_name);
            printf(" Address : %s\n", p.p_addr);
            printf("------------------------------------------\n");
            printf("Enter updated details (press Enter to keep existing):\n\n");

            char tempStr[128];

            getString(" New Name (press Enter to keep current): ", tempStr, 20, 1);
            if (strlen(tempStr) > 0) {
                strncpy(p.p_name, tempStr, 20);
                p.p_name[20] = '\0';
            }

            getString(" New Address (press Enter to keep current): ", tempStr, 40, 1);
            if (strlen(tempStr) > 0) {
                strncpy(p.p_addr, tempStr, 40);
                p.p_addr[40] = '\0';
            }
        }
        fwrite(&p, sizeof(Passenger), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove(PASSENGER_FILE);
        rename("passenger_temp.dat", PASSENGER_FILE);
        printf("\n [OK] Passenger record (ID: %d) successfully updated!\n", target_id);
    } else {
        remove("passenger_temp.dat");
        printf("\n [!] Passenger ID %d was not found in %s.\n", target_id, PASSENGER_FILE);
    }

    pauseScreen();
    return found;
}

int deletePassenger(void) {
    clearScreen();
    printBanner("PASSENGER MANAGEMENT - DELETE A RECORD");

    FILE *fp = fopen(PASSENGER_FILE, "rb");
    if (!fp) {
        printf(" [!] No passenger records found in %s.\n", PASSENGER_FILE);
        pauseScreen();
        return 0;
    }

    int target_id = getInt("Enter Passenger ID to delete: ", 1, 99999);
    Passenger p;
    int found = 0;

    FILE *temp = fopen("passenger_temp.dat", "wb");
    if (!temp) {
        printf(" [!] Error creating temporary file.\n");
        fclose(fp);
        pauseScreen();
        return 0;
    }

    while (fread(&p, sizeof(Passenger), 1, fp) == 1) {
        if (p.p_id == target_id) {
            found = 1;
            printf("\n--- Passenger Found for Deletion ---\n");
            printf(" Passenger ID : %d\n", p.p_id);
            printf(" Name         : %s\n", p.p_name);
            printf(" Address      : %s\n", p.p_addr);
            printf("------------------------------------\n");

            if (confirmAction(" Are you sure you want to permanently delete this passenger? (y/n): ")) {
                printf(" Record marked for deletion.\n");
                continue;
            } else {
                printf(" Deletion cancelled by user.\n");
                fwrite(&p, sizeof(Passenger), 1, temp);
                continue;
            }
        }
        fwrite(&p, sizeof(Passenger), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove(PASSENGER_FILE);
        rename("passenger_temp.dat", PASSENGER_FILE);
        printf("\n [OK] Passenger record deletion completed.\n");
    } else {
        remove("passenger_temp.dat");
        printf("\n [!] Passenger ID %d was not found.\n", target_id);
    }

    pauseScreen();
    return found;
}

void viewAllPassengers(void) {
    clearScreen();
    printBanner("PASSENGER MANAGEMENT - VIEW ALL RECORDS");

    FILE *fp = fopen(PASSENGER_FILE, "rb");
    if (!fp) {
        printf(" [!] No passenger records found or file %s cannot be opened.\n", PASSENGER_FILE);
        pauseScreen();
        return;
    }

    Passenger p;
    int count = 0;

    printf("+--------------+----------------------+------------------------------------------+\n");
    printf("| Passenger ID | Passenger Name       | Address                                  |\n");
    printf("+--------------+----------------------+------------------------------------------+\n");

    while (fread(&p, sizeof(Passenger), 1, fp) == 1) {
        printf("| %-12d | %-20s | %-40s |\n",
               p.p_id, p.p_name, p.p_addr);
        count++;
    }

    printf("+--------------+----------------------+------------------------------------------+\n");
    printf(" Total Passengers: %d\n", count);
    fclose(fp);
    pauseScreen();
}

void viewPassengerById(void) {
    clearScreen();
    printBanner("PASSENGER MANAGEMENT - VIEW PASSENGER BY ID");

    int id = getInt("Enter Passenger ID to view: ", 1, 99999);
    Passenger p;

    if (findPassenger(id, &p)) {
        printf("\n=======================================================\n");
        printf("                 PASSENGER DETAILS\n");
        printf("=======================================================\n");
        printf(" Passenger ID : %d\n", p.p_id);
        printf(" Name         : %s\n", p.p_name);
        printf(" Address      : %s\n", p.p_addr);
        printf("=======================================================\n");
    } else {
        printf("\n [!] Passenger ID %d not found in records.\n", id);
    }

    pauseScreen();
}
