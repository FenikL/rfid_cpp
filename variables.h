#ifndef RFID_VARIABLES_H
#define RFID_VARIABLES_H
#include <tuple>
#include <vector>

const int NumTags = 30;
const double Interval = 1.0;
const double AreaLength = 12.0;

const double Micro = 1e-6;

const double DR = 64.0 / 3;

const int QuertLength = 22;
const int QrepLength = 4;
const int AckLength = 18;
const int ReqRnLength = 40;
const int ReadLength = 58;
const int Rn16Length = 16;
const int NewRn16Length = 32;
const int EpcIdLength = 128;
const int TidLength = 97;

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
