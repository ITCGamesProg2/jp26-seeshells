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


    /// <summary>
    /// 'Notify' all Observers that some event happened to this subject 
    /// and call the corresponding response/update function for them
    /// </summary>
    /// <param name="t_event"></param>
    void notifyAll(Event t_event) {
        for (auto observer : observers) {
            observer->update(t_event);
        }
    }
};