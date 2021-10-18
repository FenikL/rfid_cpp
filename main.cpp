#include <iostream>
#include <tuple>
#include <cmath>
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


std::tuple<double, double, double, double, double> getVariablesFromTari(double tari)
{
    double trcal = 3 * tari * MICRO;
    double rtcal = 2 * tari * MICRO;
    double blf = DR / trcal;
    double tpri = 1 / blf;
    double RtcalOrTpri = std::max(rtcal, 10*tpri);
    double t1Andt2 = 1.1*RtcalOrTpri + 2*MICRO + 20*tpri;
    double t1Andt3 = 2 * 1.1 * RtcalOrTpri;
    return std::make_tuple(trcal, rtcal, blf, t1Andt2, t1Andt3);
}

std::tuple<double, double> getBitrate(double rtcal, double blf, int symPerBit)
{
    double readerBitrate = 2 / rtcal;
    double tagBitrate = blf / symPerBit;
    return std::make_tuple(readerBitrate, tagBitrate);
}

std::tuple<double, double, int> getPreamble(double tari, double rtcal, double trcal,
                                            int trext, int symPerBit)
{
    int tagPreambleLen;
    double tSyncPreamble = MICRO*(12.5 + tari) + rtcal;
    double tFullPreamble = tSyncPreamble + trcal;
    if (trext == 0) {
        if (symPerBit == 1) {
            tagPreambleLen = 6;
        } else {
            tagPreambleLen = 10;
        }
    } else {
        if (symPerBit == 1) {
            tagPreambleLen = 18;
        } else {
            tagPreambleLen = 22;
        }
    }
    return std::make_tuple(tSyncPreamble, tFullPreamble, tagPreambleLen);
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
