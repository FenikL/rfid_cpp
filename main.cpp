#include <iostream>
#include <vector>
#include <list>
#include "variables.h"
#include "inderectVariables.h"


struct TagsInArea
{
    std::list<int> tags_in_area;
    std::vector<int> num_rounds_per_tag;
};

TagsInArea GetTagsInArea(double time, std::vector<double> time_enter, std::vector<double> time_exit,
                         std::list<int> list_of_tags, std::vector<int> num_rounds_per_tag)
{
    TagsInArea list{};
    bool delete_first = false;
    for (int tag : list_of_tags)
    {
        if (time < time_enter[tag])
        {
            break;
        }

        if ((time_enter[tag] <= time) and (time < time_exit[tag]))
        {
            list.tags_in_area.push_back(tag);
            /* std::cout << timeEnter[tag] << "<=" << time << "<" << timeExit[tag] << "\n"; */
            /* ++list.num_rounds_per_tag[tag]; */
        }

        if (time >= time_exit[tag])
        {
            delete_first = true;
        }
    }
    if (delete_first)
    {
        list_of_tags.pop_front();
    }
    return list;
}


int main() {
    VariablesForTimes variables_for_time{GetVariablesForTimes(10)};

    std::list<int> list_of_tags;
    for (int i = 0; i < NumTags; ++i)
    {
        list_of_tags.push_back(i);
    }


    std::vector<int> num_rounds_per_tag(NumTags, 0);

    int count = variables_for_time.total_duration / 0.01;

    for (int i = 0; i <= count; ++i)
    {
        double time = i * 0.01;
        TagsInArea list{GetTagsInArea(time, variables_for_time.time_enter, variables_for_time.time_exit, list_of_tags, num_rounds_per_tag)};

        for (int v : list.tags_in_area)
            std::cout << v << " ";
        std::cout << "\n";
    }


}
