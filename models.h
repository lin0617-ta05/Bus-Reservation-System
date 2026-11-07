#ifndef MODELS_H
#define MODELS_H

#define BUS_FILE "bus.dat"
#define PASSENGER_FILE "passenger.dat"
#define RESERVATION_FILE "reservation.dat"

/*
 * MAR ATHANASIOS COLLEGE FOR ADVANCED STUDIES TIRUVALLA
 * DEPARTMENT OF COMPUTER APPLICATIONS
 * MCA 2024-2026 BATCH - C PROJECT
 * Group No: 2 | Bus Reservation System
 * Faculty Coordinator: Ms. Kripa Thankachan
 */

/*
 * 1. BUS FILE
 * Attributes as per project document:
 * - b_id    : Int
 * - b_name  : Char[20]
 * - b_from  : Char[20]
 * - b_to    : Char[20]
 * - b_seats : Int
 * - b_fare  : int
 */
typedef struct {
    int b_id;
    char b_name[21];   /* 20 chars + null terminator */
    char b_from[21];   /* 20 chars + null terminator */
    char b_to[21];     /* 20 chars + null terminator */
    int b_seats;
    int b_fare;
} Bus;

/*
 * 2. PASSENGER FILE
 * Attributes as per project document:
 * - p_id   : Int
 * - p_name : Char[20]
 * - p_addr : Char[40]
 */
typedef struct {
    int p_id;
    char p_name[21];   /* 20 chars + null terminator */
    char p_addr[41];   /* 40 chars + null terminator */
} Passenger;

/*
 * 3. BUS RESERVATION FILE
 * Attributes as per project document:
 * - r_id    : int
 * - b_id    : int
 * - p_id    : Char[25]
 * - r_date  : Char[10]
 * - r_seats : int
 */
typedef struct {
    int r_id;
    int b_id;
    char p_id[26];     /* Char[25] + null terminator */
    char r_date[12];   /* Char[10] (e.g. YYYY-MM-DD) + null terminator */
    int r_seats;       /* Number of seats reserved */
} Reservation;

#endif /* MODELS_H */
