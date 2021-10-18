#include <iostream>
#include "variables.h"


int main() {
    auto [ trcal, rtcal, blf, t1_and_t2, t1_and_t3 ] = getVariablesFromTari(6.25);
    std::cout << "trcal=" << trcal << ", "
              << "rtcal= " << rtcal << ", "
              << "blf= " << blf << ", "
              << "t1_and_t2= " << t1_and_t2 << ", "
              << "t1_and_t3= " << t1_and_t3 << '\n';

    auto [ totalDuration, timeEnter, timeExit ] = getVariablesForTimes(30.0);
    std::cout << "Total duration = " << totalDuration << '\n';

    for (int tag = 0; tag < NUM_TAGS; tag++)
    {
        std::cout << "TimeEnter[" << tag << "]="
                  << timeEnter[tag] << "\n";
    }
    for (int tag = 0; tag < NUM_TAGS; tag++)
    {
        std::cout << "TimeExit[" << tag << "]="
                  << timeExit[tag] << "\n";
    }
}
