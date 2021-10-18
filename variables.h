//
// Created by user on 18.10.2021.
//

#ifndef RFID_VARIABLES_H
#define RFID_VARIABLES_H
#include <tuple>
#include <vector>

const int NUM_TAGS = 30;

std::tuple<double, double, double, double, double> getVariablesFromTari(double tari);
std::tuple<double, double> getBitrate(double rtcal, double blf, int symPerBit);
std::tuple<double, double, int> getPreamble(double tari, double rtcal, double trcal,
                                            int trext, int symPerBit);
std::tuple<double, double, double, double, double> getDurationFromReader(double readerBitrate,
                                                                         double tFullPreamble,
                                                                         double tSyncPreamble);
std::tuple<double, double, double, double> getDurationFromTag(double tagPreambleLen,
                                                              double tagBitrate);
std::tuple<double, double, double, double> getProbabilitySuccessMessage(double ber);
std::tuple<double, std::vector<double>,  std::vector<double>> getVariablesForTimes(double velocity);

#endif //RFID_VARIABLES_H
