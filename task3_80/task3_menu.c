#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task3_list.h"

int main(void) {
	FLIGHT *head = NULL;
	int choice;

// Display menu options and case-handling
	do {
		printf("=== Flight Reservations Menu ===\n");
		printf(" 1) Add flight\n");
		printf(" 2) Add passenger\n");
		printf(" 3) Print flight by index\n");
		printf(" 4) Find flight by destination\n");
		printf(" 5) Delete flight\n");
		printf(" 6) Change seat\n");
		printf(" 7) Find passengers flights\n");
		printf(" 8) Find frequent fliers\n");
		printf(" 9) Exit\n");

		printf("Select an option: ");
		if(scanf("%d", &choice) != 1) {
			printf("Invalid input\n");
			break;
		}

		switch(choice) {
			case 1: {
				char id[ID_LEN], dest[DEST_LEN];
				int seats, time;
				printf("Flight ID: ");
				scanf("%15s", id);
				printf("Destination: ");
				scanf("%31s", dest);
				printf("Seats: ");
				scanf("%d", &seats);
				printf("Departure time: ");
				scanf("%d", &time);

				addFlight(&head, id, dest, seats, time);
				break;
			}
			case 2: {
				char id[ID_LEN], name[NAME_LEN];
				int seat, age;
				printf("Flight ID: ");
				scanf("%15s", id);
				printf("Seat number: ");
				scanf("%d", &seat);
				printf("Name: ");
				scanf("%31s", name);
				printf("Age: ");
				scanf("%d", &age);

				addPassenger(head, id, seat, name, age);
				break;
			}
			case 3: {
				int id;
				printf("Flight index: ");
				scanf("%d", &id);

				printFlightInfo(head, id);
				break;
			}
			case 4: {
				char dest[DEST_LEN];
				printf("Destination: ");
				scanf("%31s", dest);

				flightByDestination(head, dest);
				break;
			}
			case 5: {
				char id[ID_LEN];
				printf("Flight ID: ");
				scanf("%15s", id);

				deleteFlight(&head, id);
				break;
			}
			case 6: {
				char id[ID_LEN];
				int oldSeat, newSeat;
				printf("Flight ID: ");
				scanf("%15s", id);
				printf("Old seat: ");
				scanf("%d", &oldSeat);
				printf("New seat: ");
				scanf("%d", &newSeat);

				changeSeat(head, id, oldSeat, newSeat);
				break;
			}
			case 7: {
				char name[NAME_LEN];
				printf("Name: ");
				scanf("%31s", name);

				findPassengerFlights(head, name);
				break;
			}
			case 8:
				findFrequentFliers(head);
				break;
			case 9:
				printf("Exiting...\n");
				break;
			default:
				printf("Invalig choice!\n");
		}
	} while(choice != 9);

// Cleans up all allocated memory
	freeAllFlights(head);
	return 0;
}
