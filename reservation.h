#ifndef RESERVATION_H
#define RESERVATION_H

#include "models.h"

int insertReservation(void);
int editReservation(void);
int deleteReservation(void);
void viewAllReservations(void);
void viewReservationById(void);
void printTicket(int r_id);
void checkBusAvailabilityUI(void);

int getBookedSeats(int b_id, const char *r_date, int exclude_r_id);
int getAvailableSeats(int b_id, const char *r_date, int exclude_r_id);
int reservationExists(int r_id);
int findReservation(int r_id, Reservation *outRes);
int getReservationCount(void);

#endif /* RESERVATION_H */
