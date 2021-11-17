#ifndef RFID_VARIABLES_H
#define RFID_VARIABLES_H
#include <vector>


constexpr int NumTags = 30;
const double Interval = 1.0;
const double AreaLength = 12.0;

const double Micro = 1e-6;

const double DR = 64.0 / 3;

const int QueryLength = 22;
const int QrepLength = 4;
const int AckLength = 18;
const int ReqRnLength = 40;
const int ReadLength = 58;
const int Rn16Length = 16;
const int NewRn16Length = 32;
const int EpcidLength = 128;
const int TidLength = 97;


struct VariablesFromTari
{
    double trcal;
    double rtcal;
    double blf;
    double t1_and_t2;
    double t1_and_t3;
};

struct Bitrate
{
    double reader;
    double tag;
};

struct Preamble
{
    double t_sync;
    double t_full;
    int tag_length;
};

struct DurationFromReader
{
    double query;
    double qrep;
    double ack;
    double req_rn;
    double read;
};

struct DurationFromTag
{
    double rn16;
    double new_rn16;
    double epcid;
    double tid;
};

struct VariablesForTimes
{
    double total_duration;
    std::vector<double> time_enter;
    std::vector<double> time_exit;
};


VariablesFromTari GetVariablesFromTari(double tari);
Bitrate GetBitrate(double rtcal, double blf, int sym_per_bit);
Preamble
GetPreamble(
        double tari,
        double rtcal,
        double trcal,
        int trext,
        int sym_per_bit);
DurationFromReader
GetDurationFromReader(
        double reader_bitrate,
        double t_full_preamble,
        double t_sync_preamble);
DurationFromTag GetDurationFromTag(double tag_preamble_len, double tag_bitrate);
double GetProbabilitySuccessMessage(double ber);
VariablesForTimes GetVariablesForTimes(double velocity);

#endif //RFID_VARIABLES_H
