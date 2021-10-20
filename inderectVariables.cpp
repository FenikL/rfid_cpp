#include <iostream>
#include <tuple>
#include "inderectVariables.h"
#include "variables.h"

double tari = 6.25;

auto [ trcal, rtcal, blf, t1Andt2, t1Andt3 ] = getVariablesFromTari(tari);
int symPerBit = 1;
auto [ readerBitrate, tagBitrate ] = getBitrate(rtcal, blf, symPerBit);
int trext = 1;
auto [ tSyncPreamble, tFullPreamble, tagPreambleLen ] = getPreamble(tari, rtcal, trcal, trext, symPerBit);
auto [ tQuery, tQrep, tAck, tReqRn, tRead ] = getDurationFromReader(readerBitrate, tFullPreamble,
                                                                    tSyncPreamble);
auto [ tRn16, tNewRn16, tEpcId, tTid ] = getDurationFromTag(tagPreambleLen, tagBitrate);

std::tuple<double, double> getDurationSuccessCommands()
{
    double tSuccessSlot = tQrep + 2*t1Andt2 + tRn16 + tAck + tEpcId;
    double tSuccessTid = tReqRn + 2*t1Andt2 + tNewRn16 + tRead + tTid;
    return std::make_tuple(tSuccessSlot, tSuccessTid);
}

std::tuple<double, double, double, double> getDurationInvalidCommands()
{
    double tEmptySlot = tQrep + t1Andt3;
    double tCollidedSlot = tQrep + t1Andt2 + tRn16;
    double tInvalidRn16 = tQrep + t1Andt2 + tRn16 + tAck + t1Andt3;
    double tInvalidNewRn16 = tReqRn + t1Andt2 + tNewRn16;
    return std::make_tuple(tEmptySlot, tCollidedSlot, tInvalidRn16, tInvalidNewRn16);
}
