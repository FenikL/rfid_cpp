#include "variables.h"
#include <tuple>
#include <cmath>
#include <vector>

struct VariablesFromTari
{
    double trcal;
    double rtcal;
    double blf;
    double t1_and_t2;
    double t1_and_t3;
};

VariablesFromTari GetVariablesFromTari(double tari)
{
    VariablesFromTari variable{};
    variable.trcal = 3 * tari * Micro;
    variable.rtcal = 2 * tari * Micro;
    variable.blf = DR / variable.trcal;
    double tpri = 1 / variable.blf;
    double rtcal_or_tpri = std::max(variable.rtcal, 10*tpri);
    variable.t1_and_t2 = 1.1*rtcal_or_tpri + 2*Micro + 20*tpri;
    variable.t1_and_t3 = 2 * 1.1 * rtcal_or_tpri;
    return variable;
}

struct Bitrate
{
    double reader;
    double tag;
};

Bitrate GetBitrate(double rtcal, double blf, int sym_per_bit)
{
    Bitrate bitrate{};
    bitrate.reader = 2 / rtcal;
    bitrate.tag = blf / sym_per_bit;
    return {bitrate};
}

struct Preamble
{
    double t_sync;
    double t_full;
    int tag_length;
};

Preamble GetPreamble(double tari, double rtcal, double trcal,
                     int trext, int sym_per_bit)
{
    Preamble preamble{};
    preamble.t_sync = Micro*(12.5 + tari) + rtcal;
    preamble.t_full = preamble.t_sync + trcal;

    if (trext == 0) {
        if (sym_per_bit == 1) {
            preamble.tag_length = 6;
        } else {
            preamble.tag_length = 10;
        }
    } else {
        if (sym_per_bit == 1) {
            preamble.tag_length = 18;
        } else {
            preamble.tag_length = 22;
        }
    }
    return preamble;
}

std::tuple<double, double, double, double, double> getDurationFromReader(double readerBitrate,
                                                                         double tFullPreamble,
                                                                         double tSyncPreamble)
{
    double tQuery = (QUERY_LENGTH / readerBitrate) + tFullPreamble;
    double tQRep = (QREP_LENGTH / readerBitrate) + tSyncPreamble;
    double tAck = (ACK_LENGTH / readerBitrate) + tSyncPreamble;
    double tReqRn = (REQ_RN_LENGTH / readerBitrate) + tSyncPreamble;
    double tRead = (READ_LENGTH / readerBitrate) + tSyncPreamble;
    return std::make_tuple(tQuery, tQRep, tAck, tReqRn, tRead);
}

std::tuple<double, double, double, double> getDurationFromTag(double tagPreambleLen,
                                                              double tagBitrate)
{
    double tRn16 = (RN16_LENGTH + tagPreambleLen + 1) / tagBitrate;
    double tNewRn16 = (NEW_RN16_LENGTH + tagPreambleLen + 1) / tagBitrate;
    double tEpcId = (EPCID_LENGTH + tagPreambleLen + 1) / tagBitrate;
    double tTid = (TID_LENGTH + tagPreambleLen + 1) / tagBitrate;
    return std::make_tuple(tRn16, tNewRn16, tEpcId, tTid);
}

std::tuple<double, double, double, double> getProbabilitySuccessMessage(double ber)
{
    double x = 1 - ber;
    double rn16 = pow(x, RN16_LENGTH);
    double epcid = pow(x, EPCID_LENGTH);
    double newRn16 = pow(x, NEW_RN16_LENGTH);
    double tid = pow(x, TID_LENGTH);
    return std::make_tuple(rn16, epcid, newRn16, tid);
}

std::tuple<double, std::vector<double>,  std::vector<double>> getVariablesForTimes(double velocity)
{
    std::vector<double> timeEnter(NUM_TAGS);
    std::vector<double> timeExit(NUM_TAGS);

    double totalDuration = INTERVAL*(NUM_TAGS - 1) + AREA_LENGTH/velocity;

    for (int tag = 0; tag < NUM_TAGS; tag++)
    {
        timeEnter[tag] = INTERVAL * tag;
    }

    for (int tag = 0; tag < NUM_TAGS; tag++)
    {
        timeExit[tag] = (AREA_LENGTH/velocity) + timeEnter[tag];
    }

    return std::make_tuple(totalDuration, timeEnter, timeExit);
}