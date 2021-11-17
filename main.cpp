#include <iostream>
#include <vector>
#include <random>
#include <list>
#include "variables.h"
#include "inderectVariables.h"
#include "snr_to_ber.h"
#include <chrono>
#include <cstdlib>
#include <map>
#include <ctime>


std::vector<int> GetTagsInArea(double time, const std::vector<double>& time_enter, const std::vector<double>& time_exit,
                         std::list<int>& list_of_tags, std::vector<int>& num_rounds_per_tag)
{
    std::vector<int> tags_in_area;
    auto start_time = std::chrono::steady_clock::now();
    bool delete_first = false;
    for (int tag : list_of_tags) {
        if (time < time_enter[tag]) {
            break;
        }

        if ((time_enter[tag] <= time) and (time < time_exit[tag])) {
            tags_in_area.push_back(tag);
            /* std::cout << timeEnter[tag] << "<=" << time << "<" << timeExit[tag] << "\n"; */
            ++num_rounds_per_tag[tag];
        }

        if (time >= time_exit[tag]) {
            delete_first = true;
        }
    }
    if (delete_first) {
        list_of_tags.pop_front();
    }
    auto end_time = std::chrono::steady_clock::now();
    auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
    std::cout << elapsed_ns.count() << " ns\n";
    return tags_in_area;
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

int GetRandomNumber(int max)
{
    int num = std::rand() % (max + 1);
    return num;
}

double GetPosition(double time, double time_enter, double velocity) {
    return AreaLength - (time - time_enter) * velocity;
}

double GetBer(double time, double velocity, double time_enter, int miller, double preamble_duration, double blf) {
    double x = GetPosition(time, time_enter, velocity);
    double rx_power = GetTagPower(x);
    double snr = GetSnr(rx_power, miller, preamble_duration, blf);
    return GetBerOverRayleigh(snr);
}

double RunModel(double velocity, int q, bool tid, double tari) {
    int slot_range = pow(2, q);
    for (int _=0; _<NumIterations; ++_) {
        double time = 0;
        std::vector<bool> identified_epc(NumTags, false);
        std::vector<bool> identified_epc_and_tid(NumTags, false);
        std::vector<int> num_rounds_per_tag(NumTags, 0);
        std::list<int> list_of_tags;
        for (int i=0; i<NumTags; ++i) {
            list_of_tags.push_back(i);
        }
        while (time < total_duration) {
            std::vector<int> tags_in_area = GetTagsInArea(time, time_enter, time_exit, list_of_tags, num_rounds_per_tag);
            std::map<int, int> tags_slots;
            for (int tag : tags_in_area) {
                tags_slots[tag] = GetRandomNumber(slot_range);
            }
            int t_round_started = time;
            time += t_query - t_qrep;

            for (int slot=0; slot<slot_range; ++slot) {
                std::vector<int> responding_tags;
                for (int tag : tags_in_area) {
                    if (tags_slots[tag] == 0) {
                        responding_tags.push_back(tag);
                    }
                }
                if (responding_tags.size() ==0) {
                    time += empty_slot;
                }
                else if (responding_tags.size() == 1) {
                    int tag = responding_tags[0];
                    double x = GetPosition(time, time_enter[tag], velocity);
                    double rx_power = GetTagPower(x);
                    double snr = GetSnr(x, miller, preamble_duration, blf);
                    double ber = GetBerOverRayleigh(snr);
                    double probability_success_message = GetProbabilitySuccessMessage(ber);

                }
            }


        }
    }
}

int main() {
    std::srand(std::time(NULL));
    VariablesForTimes variables_for_time{GetVariablesForTimes(10)};

    std::list<int> list_of_tags;
    for (int i = 0; i < NumTags; ++i)
    {
        list_of_tags.push_back(i);
    }


    std::vector<int> num_rounds_per_tag(NumTags, 0);
    std::vector<int> tags_in_area;

    double count = variables_for_time.total_duration / 0.01;

    for (int i = 0; i <= count; ++i)
    {
        double time = i * 0.01;
        tags_in_area = GetTagsInArea(
                                    time, variables_for_time.time_enter, variables_for_time.time_exit,
                                    list_of_tags,
                                    num_rounds_per_tag);

        for (int v : tags_in_area)
            std::cout << v << " ";
        std::cout << "\n";
    }

    IdTransmission tid = SimulateIdTransmission(true, 0.03, false);
    int count_ = 0;
    for (int i=0; i < 1000; ++i)
    {
        IdTransmission tid = SimulateIdTransmission(true, 0.03, false);
        if (tid.identified)
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
    std::cout << "preamble_duration" << " " << preamble_duration << " " << miller << " " << val_tari.blf << "\n";
    std::cout << "BER" << "\n";
    for (double i=1; i<14; ++i) {
        std::cout << i << "\n";
        rx_power = GetTagPower(i);
        std::cout << rx_power << "\n";
        snr = GetSnr(rx_power, miller, preamble_duration,
                     val_tari.blf);
        std::cout << GetBerOverRayleigh(snr) << "\n";
    }
    std::cout << "random=" << GetRandomNumber(std::pow(2, 2))<<std::endl;
    std::cout << "random=" << GetRandomNumber(std::pow(2, 2));

    return 0;
}
