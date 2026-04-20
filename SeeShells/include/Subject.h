#pragma once

#include "Observer.h"
#include <vector>

class Subject {
    std::vector<Observer*> observers;

public:
    void addObserver(Observer* observer) {
        observers.push_back(observer);
    }
    void removeObserver(Observer* observer) {
        observers.erase(remove(observers.begin(), observers.end(), observer), observers.end());
    }
    void notifyAll(Event t_event) {
        for (auto observer : observers) {
            observer->update(t_event);
        }
    }
};