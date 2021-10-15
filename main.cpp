#include <iostream>
#include <tuple>

const double MICRO = 1e-6;
const double DR = 64.0 / 3;
const int QUERY_LENGTH = 22;
const int QREP_LENGTH = 4;
const int ACK_LENGTH = 18;
const int REQ_RN_LENGTH = 40;
const int READ_LENGTH = 58;


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

int main() {
    auto [ trcal, rtcal, blf, t1_and_t2, t1_and_t3 ] = getVariablesFromTari(6.25);
    std::cout << "trcal=" << trcal << ", "
              << "rtcal= " << rtcal << ", "
              << "blf= " << blf << ", "
              << "t1_and_t2= " << t1_and_t2 << ", "
              << "t1_and_t3= " << t1_and_t3 << '\n';
}
