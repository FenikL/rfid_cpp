#include "variables.h"
#include <cmath>
#include <vector>


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

Bitrate GetBitrate(double rtcal, double blf, int miller)
{
    Bitrate bitrate{};
    bitrate.reader = 2 / rtcal;
    bitrate.tag = blf / miller;
    return bitrate;
}

Preamble GetPreamble(double tari, double rtcal, double trcal,
                     int trext, int miller)
{
    Preamble preamble{};
    preamble.t_sync = Micro*(12.5 + tari) + rtcal;
    preamble.t_full = preamble.t_sync + trcal;

    if (trext == 0) {
        if (miller == 1) {
            preamble.tag_length = 6;
        } else {
            preamble.tag_length = 10;
        }
    } else {
        if (miller == 1) {
            preamble.tag_length = 18;
        } else {
            preamble.tag_length = 22;
        }
    }
    return preamble;
}

DurationFromReader GetDurationFromReader(double reader_bitrate,
                                         double t_full_preamble,
                                         double t_sync_preamble)
{
    DurationFromReader duration{};
    duration.query = (QueryLength / reader_bitrate) + t_full_preamble;
    duration.qrep = (QrepLength / reader_bitrate) + t_sync_preamble;
    duration.ack = (AckLength / reader_bitrate) + t_sync_preamble;
    duration.req_rn = (ReqRnLength / reader_bitrate) + t_sync_preamble;
    duration.read = (ReadLength / reader_bitrate) + t_sync_preamble;
    return duration;
}

DurationFromTag GetDurationFromTag(double tag_preamble_len, double tag_bitrate)
{
    DurationFromTag duration{};
    duration.rn16 = (Rn16Length + tag_preamble_len + 1) / tag_bitrate;
    duration.new_rn16 = (NewRn16Length + tag_preamble_len + 1) / tag_bitrate;
    duration.epcid = (EpcidLength + tag_preamble_len + 1) / tag_bitrate;
    duration.tid = (TidLength + tag_preamble_len + 1) / tag_bitrate;
    return duration;
}

double GetProbabilitySuccessMessage(double ber, int length)
{
    double x = 1 - ber;
    return pow(x, length);
}

VariablesForTimes GetVariablesForTimes(double velocity)
{
    VariablesForTimes variable{};
    variable.time_enter = std::vector<double>(NumTags);
    variable.time_exit = std::vector<double>(NumTags);
    variable.total_duration = Interval*(NumTags - 1) + AreaLength/velocity;

    for (int tag = 0; tag < NumTags; ++tag)
    {
        variable.time_enter[tag] = Interval * tag;
    }

    for (int tag = 0; tag < NumTags; ++tag)
    {
        variable.time_exit[tag] = (AreaLength/velocity) + variable.time_enter[tag];
    }

    return variable;
}
