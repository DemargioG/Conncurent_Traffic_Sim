#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;


// FP.3 Define a class „MessageQueue“ which has the public methods send and receive. 
// Send should take an rvalue reference of type TrafficLightPhase whereas receive should return this type. 
// Also, the class should define an std::dequeue called _queue, which stores objects of type TrafficLightPhase. 
// Also, there should be an std::condition_variable as well as an std::mutex as private members. 

template <class T>
class MessageQueue
{
public:

T MessageQueue<T>::receive();


void MessageQueue<T>::send(T &&msg);


private:
// creating _queue, _mutex, and _conditionVariable
std::deque<TrafficLight::TrafficLightPhase> _queue;
std::mutex _mutexd;
std::condition_variable _conditionVariable;

    
};

// FP.1 : Define a class „TrafficLight“ which is a child class of TrafficObject. 
// The class shall have the public methods „void waitForGreen()“ and „void simulate()“ 
// as well as „TrafficLightPhase getCurrentPhase()“, where TrafficLightPhase is an enum that 
// can be either „red“ or „green“. Also, add the private method „void cycleThroughPhases()“. 
// Furthermore, there shall be the private member _currentPhase which can take „red“ or „green“ as its value. 

class TrafficLight : public TrafficObject
{
public:

    enum TrafficLightPhase {red,green};

    // constructor / desctructor
    //TrafficLight(){setCurrentPhase(red);} // When a light is created, its phase begins as RED. (There is already a Default constuctor.)

    // getters / setters
    TrafficLightPhase TrafficLight::getCurrentPhase(); // Phase getter
    //void setCurrentPhase(TrafficLight::TrafficLightPhase phase); // Phase setter (I was only using it for my Default Constructor.)

    // typical behaviour methods

    void waitForGreen();
    void simulate();
    

private:
    
    TrafficLightPhase TrafficLight::_currentPhase;
    
    // typical behaviour methods

    void cycleThroughPhases();


    // FP.4b : create a private member of type MessageQueue for messages of type TrafficLightPhase 
    // and use it within the infinite loop to push each new TrafficLightPhase into it by calling 
    // send in conjunction with move semantics.

    MessageQueue<TrafficLight::TrafficLightPhase> _lightQueue; 
    std::condition_variable _condition;
    std::mutex _mutex;
};

#endif