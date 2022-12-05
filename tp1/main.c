#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

const int CHARGE_MAX = 15;
int charge = 0;

pthread_mutex_t mutex;
pthread_cond_t cond;

int canEnterBridge(int vehicleWeight) {
	return vehicleWeight + charge <= CHARGE_MAX;
}

void vehicle(int id, int weight, char* name, int duration) {
	pthread_mutex_lock(&mutex);
	while (!canEnterBridge(weight)) {
		pthread_cond_wait(&cond, &mutex);	
	}
	charge += weight;
	printf("%s %d entering and traversing the bridge at a charge of: %d\n", name, id, charge);
	pthread_mutex_unlock(&mutex);
	// mimic the traversal
	sleep(duration);
	pthread_mutex_lock(&mutex);
	charge -= weight;
	pthread_cond_signal(&cond);
	printf("%s %d left. => charge = %d\n", name, id, charge);
	pthread_mutex_unlock(&mutex);
}

void* truck(void* args) {
	int weight = 15;
	int id = (int) args;
	vehicle(id, weight, "Truck", 3);
}

void* car(void* args) {
	int weight = 5;
	int id = (int) args;
	vehicle(id, weight, "Car", 1);
}

pthread_t createCar(int id) {
	pthread_t thread_id;
	pthread_attr_t attr;
	struct sched_param param;

	pthread_attr_init(&attr);
	param.sched_priority = 5;
	pthread_attr_setschedparam(&attr, &param);

	pthread_create(&thread_id, &attr, car, id);

	pthread_attr_destroy(&attr);

	return thread_id;
}

pthread_t createTruck(int id) {
	pthread_t thread_id;
	pthread_attr_t attr;
	struct sched_param param;

	pthread_attr_init(&attr);
	param.sched_priority = 3;
	pthread_attr_setschedparam(&attr, &param);

	pthread_create(&thread_id, &attr, truck, id);

	pthread_attr_destroy(&attr);

	return thread_id;
}

int main() {
	const int cars_number = 5;
	const int trucks_number = 3;
	pthread_t cars[cars_number];
	pthread_t trucks[trucks_number];
	int i;
	for(i = 0; i < 5; i++) {
		cars[i] = createCar(i);
	}

	for(i = 0; i < 3; i++) {
		trucks[i] = createTruck(i);
	}

	for (i = 0; i < cars_number; i++) {
		pthread_join(cars[i], NULL);
	}

	for(i = 0; i < trucks_number; i++) {
		pthread_join(trucks[i], NULL);
	}

	return 0;
}