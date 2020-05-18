#include <iostream>
#include <random>
#include <chrono>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */

 
template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function.
     
    std::unique_lock<std::mutex> ulock(_mutexd);
    _conditionVariable.wait(ulock, [this] {return !_queue.empty(); });

    TrafficLight::TrafficLightPhase phase = std::move(_queue.back());
    _queue.pop_back();

    return phase; 
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.

    std::lock_guard<std::mutex> ulock(_mutexd);

    _queue.push_back(std::move(msg));
    _conditionVariable.notify_one();
}

/* Implementation of class "TrafficLight" */

 
TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.

    while(true)
    {
        if (_lightQueue.receive() == TrafficLight::TrafficLightPhase::green)
        {
            return;
        }

    }
}

TrafficLight::TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class.
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases,this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles.

    // Creating the random time generator variable 
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(4000,6000);
    int randomCycleTime = distribution(generator);
    double cycleTime = (double) randomCycleTime;

    // create variable to monitor time
    long elapsedTime;
    std::chrono::time_point<std::chrono::system_clock> startCycleTime;
    startCycleTime = std::chrono::system_clock:: now(); 

    while(true) //infinite loop
    {
        elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startCycleTime).count();
        if (elapsedTime >= cycleTime)
        {
            switch(_currentPhase)
            {
                case red:
                    //toggle light
                    _currentPhase = green;

                    // update message
                    _lightQueue.send(std::move(TrafficLight::TrafficLightPhase::green));

                    // Set new random cycle time between 4 and 6 sec
                    int randomCycleTime = distribution(generator);
                    double cycleTime = (double) randomCycleTime;

                    // Reset timer
                    startCycleTime = std::chrono::system_clock::now();

                    break;
                case green:
                    //toggle light
                    _currentPhase = red;

                    //update message
                    _lightQueue.send(std::move(TrafficLight::TrafficLightPhase::red));

                    // Set new random cycle time between 4 and 6 sec
                    int randomCycleTime = distribution(generator);
                    double cycleTime = (double) randomCycleTime;

                    // Reset timer
                    startCycleTime = std::chrono::system_clock::now();

                    break;
            }
        }

        
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); //pause to reduce CPU usage

    }
}

