#ifndef BUS_H
#define BUS_H

#include "models.h"

int insertBus(void);
int editBus(void);
int deleteBus(void);
void viewAllBuses(void);
void viewBusById(void);

int findBus(int b_id, Bus *outBus);
int busExists(int b_id);
int getBusCount(void);

#endif /* BUS_H */
