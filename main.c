#include <stdio.h>
#include <stdlib.h>
#include "models.h"
#include "utils.h"
#include "bus.h"
#include "passenger.h"
#include "reservation.h"

void busMenu(void);
void passengerMenu(void);
void reservationMenu(void);
void seedSampleData(void);
void showSystemSummary(void);

int main(void) {
    int choice;

    while (1) {
        clearScreen();
        printBanner("MAIN SYSTEM MENU");
        printf("  1. BUS MANAGEMENT           (Insert, Edit, Delete, View)\n");
        printf("  2. PASSENGER MANAGEMENT     (Insert, Edit, Delete, View)\n");
        printf("  3. RESERVATION MANAGEMENT   (Book, Edit, Cancel, View)\n");
        printf("  4. GENERATE / PRINT TICKET  (Boarding Pass)\n");
        printf("  5. CHECK SEAT AVAILABILITY  (By Bus & Date)\n");
        printf("  6. LOAD SAMPLE DEMO DATA    (Seed Initial Records)\n");
        printf("  7. SYSTEM SUMMARY           (Record Counts & Files)\n");
        printf("  8. EXIT\n");
        printf("--------------------------------------------------------------------------------\n");

        choice = getInt("Select an option (1-8): ", 1, 8);

        switch (choice) {
            case 1:
                busMenu();
                break;
            case 2:
                passengerMenu();
                break;
            case 3:
                reservationMenu();
                break;
            case 4: {
                clearScreen();
                printBanner("PRINT TICKET");
                int r_id = getInt("Enter Reservation ID: ", 1, 999999);
                printTicket(r_id);
                pauseScreen();
                break;
            }
            case 5:
                checkBusAvailabilityUI();
                break;
            case 6:
                seedSampleData();
                break;
            case 7:
                showSystemSummary();
                break;
            case 8:
                clearScreen();
                printBanner("THANK YOU");
                printf("\n  Exiting Bus Reservation System.\n");
                printf("  Project by: MCA 2024-2026 Batch - Group No: 2\n");
                printf("  Department of Computer Applications\n");
                printf("  Mar Athanasios College for Advanced Studies Tiruvalla (MACFAST)\n");
                printf("  Faculty Coordinator: Ms. Kripa Thankachan\n\n");
                exit(0);
            default:
                break;
        }
    }

    return 0;
}

void busMenu(void) {
    int choice;
    while (1) {
        clearScreen();
        printBanner("BUS MANAGEMENT MENU");
        printf("  1. Insert a Record  (Add New Bus)\n");
        printf("  2. Edit a Record    (Modify Bus Details)\n");
        printf("  3. Delete a Record  (Remove Bus)\n");
        printf("  4. View All Records (List all Buses in Table)\n");
        printf("  5. View Record by ID\n");
        printf("  6. Back to Main Menu\n");
        printf("--------------------------------------------------------------------------------\n");

        choice = getInt("Enter your choice (1-6): ", 1, 6);

        switch (choice) {
            case 1:
                insertBus();
                break;
            case 2:
                editBus();
                break;
            case 3:
                deleteBus();
                break;
            case 4:
                viewAllBuses();
                break;
            case 5:
                viewBusById();
                break;
            case 6:
                return;
            default:
                break;
        }
    }
}

void passengerMenu(void) {
    int choice;
    while (1) {
        clearScreen();
        printBanner("PASSENGER MANAGEMENT MENU");
        printf("  1. Insert a Record  (Add New Passenger)\n");
        printf("  2. Edit a Record    (Modify Passenger Details)\n");
        printf("  3. Delete a Record  (Remove Passenger)\n");
        printf("  4. View All Records (List all Passengers in Table)\n");
        printf("  5. View Record by ID\n");
        printf("  6. Back to Main Menu\n");
        printf("--------------------------------------------------------------------------------\n");

        choice = getInt("Enter your choice (1-6): ", 1, 6);

        switch (choice) {
            case 1:
                insertPassenger();
                break;
            case 2:
                editPassenger();
                break;
            case 3:
                deletePassenger();
                break;
            case 4:
                viewAllPassengers();
                break;
            case 5:
                viewPassengerById();
                break;
            case 6:
                return;
            default:
                break;
        }
    }
}

void reservationMenu(void) {
    int choice;
    while (1) {
        clearScreen();
        printBanner("BUS RESERVATION MANAGEMENT MENU");
        printf("  1. Insert a Record  (Book New Reservation)\n");
        printf("  2. Edit a Record    (Modify Reservation/Seats/Date)\n");
        printf("  3. Delete a Record  (Cancel Reservation)\n");
        printf("  4. View All Records (List all Reservations)\n");
        printf("  5. View Record by ID (Boarding Pass / Ticket)\n");
        printf("  6. Back to Main Menu\n");
        printf("--------------------------------------------------------------------------------\n");

        choice = getInt("Enter your choice (1-6): ", 1, 6);

        switch (choice) {
            case 1:
                insertReservation();
                break;
            case 2:
                editReservation();
                break;
            case 3:
                deleteReservation();
                break;
            case 4:
                viewAllReservations();
                break;
            case 5:
                viewReservationById();
                break;
            case 6:
                return;
            default:
                break;
        }
    }
}

void showSystemSummary(void) {
    clearScreen();
    printBanner("SYSTEM SUMMARY & FILE STATUS");

    int numBuses = getBusCount();
    int numPass = getPassengerCount();
    int numRes = getReservationCount();

    printf("  Project Metadata:\n");
    printf("  - College        : Mar Athanasios College for Advanced Studies Tiruvalla\n");
    printf("  - Department     : Department of Computer Applications\n");
    printf("  - Program & Batch: MCA 2024-2026 Batch\n");
    printf("  - Group Number   : Group 2\n");
    printf("  - Project Name   : Bus Reservation System in C\n");
    printf("  - Coordinator    : Ms. Kripa Thankachan\n\n");

    printf("  File Statistics:\n");
    printf("  - BUS FILE            (%s)         : %d records\n", BUS_FILE, numBuses);
    printf("  - PASSENGER FILE      (%s)   : %d records\n", PASSENGER_FILE, numPass);
    printf("  - BUS RESERVATION FILE(%s) : %d records\n", RESERVATION_FILE, numRes);
    printf("--------------------------------------------------------------------------------\n");

    pauseScreen();
}

void seedSampleData(void) {
    clearScreen();
    printBanner("LOAD SAMPLE DEMO DATA");

    printf(" This will add sample buses, passengers, and reservations to the database\n");
    printf(" for immediate testing and demonstration.\n\n");

    if (!confirmAction(" Proceed with loading sample demo records? (y/n): ")) {
        printf(" Operation cancelled.\n");
        pauseScreen();
        return;
    }

    /* 1. Sample Buses */
    Bus sampleBuses[] = {
        {101, "KSRTC Swift Super", "Tiruvalla", "Trivandrum", 40, 280},
        {102, "Garuda Maharaja",   "Tiruvalla", "Bangalore",  36, 1250},
        {103, "Minnal Express",    "Kottayam",  "Kozhikode",  45, 420},
        {104, "Low Floor AC",      "Ernakulam", "Tiruvalla",  35, 210}
    };
    int busCount = sizeof(sampleBuses) / sizeof(sampleBuses[0]);

    FILE *fbus = fopen(BUS_FILE, "ab");
    if (fbus) {
        for (int i = 0; i < busCount; i++) {
            if (!busExists(sampleBuses[i].b_id)) {
                fwrite(&sampleBuses[i], sizeof(Bus), 1, fbus);
            }
        }
        fclose(fbus);
    }

    /* 2. Sample Passengers */
    Passenger samplePass[] = {
        {201, "Rahul Nair",      "College Road, Tiruvalla"},
        {202, "Ananya Thomas",   "Kottayam Town, Kerala"},
        {203, "Mathew Varghese", "MG Road, Ernakulam"}
    };
    int passCount = sizeof(samplePass) / sizeof(samplePass[0]);

    FILE *fpass = fopen(PASSENGER_FILE, "ab");
    if (fpass) {
        for (int i = 0; i < passCount; i++) {
            if (!passengerExists(samplePass[i].p_id)) {
                fwrite(&samplePass[i], sizeof(Passenger), 1, fpass);
            }
        }
        fclose(fpass);
    }

    /* 3. Sample Reservations */
    Reservation sampleRes[] = {
        {1001, 101, "201", "2026-10-25", 2},
        {1002, 102, "202", "2026-10-26", 1},
        {1003, 101, "203", "2026-10-25", 3}
    };
    int resCount = sizeof(sampleRes) / sizeof(sampleRes[0]);

    FILE *fres = fopen(RESERVATION_FILE, "ab");
    if (fres) {
        for (int i = 0; i < resCount; i++) {
            if (!reservationExists(sampleRes[i].r_id)) {
                fwrite(&sampleRes[i], sizeof(Reservation), 1, fres);
            }
        }
        fclose(fres);
    }

    printf("\n [OK] Sample demo data loaded successfully!\n");
    printf("   - %d buses checked/seeded.\n", busCount);
    printf("   - %d passengers checked/seeded.\n", passCount);
    printf("   - %d reservations checked/seeded.\n", resCount);

    pauseScreen();
}
