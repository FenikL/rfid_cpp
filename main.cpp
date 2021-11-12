#include <iostream>
#include <vector>
#include <random>
#include <list>
#include "variables.h"
#include "inderectVariables.h"
#include "snr_to_ber.h"


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

struct Rn16Transmission
{
    float time;
    bool last_event_is_success;
};

Rn16Transmission SimulateRn16Transmission(float time, float probability_success_message,
                                          float duration_event_success,
                                          float duration_event_invalid)
{
    std::random_device rnd;
    std::mt19937 gen(rnd());
    std::uniform_real_distribution<> dis(0,1.0);
    Rn16Transmission variable{};
    variable.time = time;

    if (dis(gen) < probability_success_message)
    {
        variable.time += duration_event_success;
        variable.last_event_is_success = true;
    }
    else
    {
        variable.time += duration_event_invalid;
        variable.last_event_is_success = false;
    }

    return variable;
}

struct IdTransmission
{
    bool identified;
    bool last_event_is_success;
};

IdTransmission SimulateIdTransmission(bool last_event_success, float probability_success_message,
                                      bool identified)
{
    std::random_device rnd;
    std::mt19937 gen(rnd());
    std::uniform_real_distribution<> dis(0,1.0);
    IdTransmission variable{};
    variable.last_event_is_success = last_event_success;
    variable.identified = identified;

    if (dis(gen) < probability_success_message)
    {
        variable.identified = true;
        variable.last_event_is_success = true;
    }
    else
    {
        variable.last_event_is_success = false;
    }

    return variable;
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
        TagsInArea list{GetTagsInArea(
                                                time, variables_for_time.time_enter,variables_for_time.time_exit,
                                                list_of_tags,
                                                num_rounds_per_tag)};

        for (int v : list.tags_in_area)
            std::cout << v << " ";
        std::cout << "\n";
    }

    IdTransmission tid = SimulateIdTransmission(true, 0.03, false);
    int count_ = 0;
    for (int i=0; i < 1000; ++i)
    {
        IdTransmission tid = SimulateIdTransmission(true, 0.03, false);
        if (tid.identified == true)
        {
            ++count_;
        }

    }
    std::cout << (count_);

    double rx_power, snr;
    int miller = 1;
    VariablesFromTari val_tari = GetVariablesFromTari(6.25);
    Preamble preamble = GetPreamble(6.25, val_tari.rtcal,
                                    val_tari.trcal,
                                    0, miller);
    Bitrate bitrate = GetBitrate(val_tari.rtcal, val_tari.blf, miller);
    double preamble_duration = preamble.tag_length / bitrate.tag;
    std::cout << "BER" << "\n";
    for (double i=1; i<14; ++i) {
        std::cout << i << "\n";
        rx_power = GetTagPower(i);
        std::cout << rx_power << "\n";
        snr = GetSnr(rx_power, miller, preamble_duration,
                     val_tari.blf);
        std::cout << GetBerOverRayleigh(snr) << "\n";
    }


}
