#include <iostream>
#include <vector>
#include <list>
#include "variables.h"
#include "inderectVariables.h"


std::tuple<std::list<int>, std::vector<int>> getTagsInArea(double time, std::vector<double> timeEnter,
                                                             std::vector<double> timeExit,
                                                             std::list<int> listOfTags,
                                                             std::vector<int> numRoundsPerTag)
{
    std::list<int> tagsInArea;
    bool deleteFirst = false;
    for (int tag : listOfTags)
    {
        if (time < timeEnter[tag])
        {
            break;
        }

        if ((timeEnter[tag] <= time) and (time < timeExit[tag]))
        {
            tagsInArea.push_back(tag);
        /* std::cout << timeEnter[tag] << "<=" << time << "<" << timeExit[tag] << "\n"; */
            ++numRoundsPerTag[tag];
        }

        if (time >= timeExit[tag])
        {
            deleteFirst = true;
        }
    }
    if (deleteFirst)
    {
        listOfTags.pop_front();
    }
    return std::make_tuple(tagsInArea, numRoundsPerTag);
}

int main() {
    auto [ trcal, rtcal, blf, t1_and_t2, t1_and_t3 ] = getVariablesFromTari(6.25);
    std::cout << "trcal=" << trcal << ", "
              << "rtcal= " << rtcal << ", "
              << "blf= " << blf << ", "
              << "t1_and_t2= " << t1_and_t2 << ", "
              << "t1_and_t3= " << t1_and_t3 << '\n';
/*
    auto [ totalDuration, timeEnter, timeExit ] = getVariablesForTimes(30.0);
    std::cout << "Total duration = " << totalDuration << '\n';

    for (int tag = 0; tag < NUM_TAGS; ++tag)
    {
        std::cout << "TimeEnter[" << tag << "]="
                  << timeEnter[tag] << "\n";
    }
    for (int tag = 0; tag < NUM_TAGS; ++tag)
    {
        std::cout << "TimeExit[" << tag << "]="
                  << timeExit[tag] << "\n";
    }

    auto [ tSuccessSlot, tSuccessTid ] = getDurationSuccessCommands();
    std::cout << "tSuccessSlot=" << tSuccessSlot
            << ", tSuccessTid=" << tSuccessTid << "\n";

*/
    auto [ totalDuration, timeEnter, timeExit] = getVariablesForTimes(10);

    std::list<int> listOfTags;
    for (int i = 0; i < NUM_TAGS; ++i)
    {
        listOfTags.push_back(i);
    }


    std::vector<int> numRoundsPerTag(NUM_TAGS, 0);

    int count = totalDuration / 0.01;

    for (int i = 0; i <= count; ++i)
    {
        double time = i * 0.01;
        auto [ tagsInArea, numRoundsPerTagReturn ] = getTagsInArea(time, timeEnter, timeExit, listOfTags, numRoundsPerTag);

        for (int v : tagsInArea)
            std::cout << v << " ";
        std::cout << "\n";
    }


}
