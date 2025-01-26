#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>

using namespace std;

mutex mtx;
int currentGreenLightIntersection1 = 0;
int currentGreenLightIntersection2 = 0;
const int NUM_ROADS_PER_INTERSECTION = 4;

class Car {
public:
    string brand;
    int lane;
    int intersection;

    Car(string b, int l, int inter) {
        brand = b;
        lane = l;
        intersection = inter;
    }

    void move() {
        while (true) {
            {
                unique_lock<mutex> lock(mtx);
                if ((intersection == 1 && lane == currentGreenLightIntersection1) ||
                    (intersection == 2 && lane == currentGreenLightIntersection2)) {
                    cout << "Car " << brand << " is moving from intersection " << intersection 
                         << ", lane " << lane << endl;
                    break;
                }
            }
            this_thread::yield();
        }
    }
};

void trafficLightControlIntersection1();
void trafficLightControlIntersection2();

int main() {
    vector<Car> cars = {
        Car("Toyota", 0, 1), // Cars in Intersection 1
        Car("Honda", 1, 1),
        Car("Ford", 2, 1),
        Car("Nissan", 3, 1),
        Car("BMW", 0, 2),    // Cars in Intersection 2
        Car("Hyundai", 1, 2),
        Car("Tesla", 2, 2),
        Car("Chevrolet", 3, 2)
    };

    thread trafficThreadIntersection1(trafficLightControlIntersection1);
    thread trafficThreadIntersection2(trafficLightControlIntersection2);

    vector<thread> carThreads;
    for (Car& car : cars) {
        carThreads.emplace_back(&Car::move, &car);
    }

    for (thread& t : carThreads) {
        t.join();
    }

    trafficThreadIntersection1.detach();
    trafficThreadIntersection2.detach();

    return 0;
}

void trafficLightControlIntersection1() {
    while (true) {
        for (int i = 0; i < NUM_ROADS_PER_INTERSECTION; ++i) {
            {
                unique_lock<mutex> lock(mtx);
                currentGreenLightIntersection1 = i;
                cout << "Intersection 1, Traffic light for lane " << i << ": GREEN" << endl;
            }
            this_thread::sleep_for(chrono::seconds(5));

            {
                unique_lock<mutex> lock(mtx);
                cout << "Intersection 1, Traffic light for lane " << i << ": YELLOW" << endl;
            }
            this_thread::sleep_for(chrono::seconds(2));

            {
                unique_lock<mutex> lock(mtx);
                cout << "Intersection 1, Traffic light for lane " << i << ": RED\n" << endl;
            }
            this_thread::sleep_for(chrono::seconds(2));
        }
    }
}

void trafficLightControlIntersection2() {
    while (true) {
        for (int i = 0; i < NUM_ROADS_PER_INTERSECTION; ++i) {
            {
                unique_lock<mutex> lock(mtx);
                currentGreenLightIntersection2 = i;
                cout << "Intersection 2, Traffic light for lane " << i << ": GREEN" << endl;
            }
            this_thread::sleep_for(chrono::seconds(5));

            {
                unique_lock<mutex> lock(mtx);
                cout << "Intersection 2, Traffic light for lane " << i << ": YELLOW" << endl;
            }
            this_thread::sleep_for(chrono::seconds(2));

            {
                unique_lock<mutex> lock(mtx);
                cout << "Intersection 2, Traffic light for lane " << i << ": RED\n" << endl;
            }
            this_thread::sleep_for(chrono::seconds(2));
        }
    }
}
