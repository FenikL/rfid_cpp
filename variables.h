//
// Created by user on 18.10.2021.
//

#ifndef RFID_VARIABLES_H
#define RFID_VARIABLES_H
#include <tuple>
#include <vector>

const int NUM_TAGS = 30;
const double INTERVAL = 1.0;
const double AREA_LENGTH = 12.0;

const double MICRO = 1e-6;

const double DR = 64.0 / 3;

const int QUERY_LENGTH = 22;
const int QREP_LENGTH = 4;
const int ACK_LENGTH = 18;
const int REQ_RN_LENGTH = 40;
const int READ_LENGTH = 58;
const int RN16_LENGTH = 16;
const int NEW_RN16_LENGTH = 32;
const int EPCID_LENGTH = 128;
const int TID_LENGTH = 97;

std::tuple<double, double, double, double, double> getVariablesFromTari(double tari) noexcept;
std::tuple<double, double> getBitrate(double rtcal, double blf, int symPerBit);
std::tuple<double, double, int> getPreamble(double tari, double rtcal, double trcal,
                                            int trext, int symPerBit);

std::tuple<double, double, double, double, double>
getDurationFromReader(
        double readerBitrate,
        double tFullPreamble,
        double tSyncPreamble);

std::tuple<double, double, double, double> getDurationFromTag(double tagPreambleLen,
                                                              double tagBitrate);
std::tuple<double, double, double, double> getProbabilitySuccessMessage(double ber);
std::tuple<double, std::vector<double>,  std::vector<double>> getVariablesForTimes(double velocity);

#endif //RFID_VARIABLES_H
