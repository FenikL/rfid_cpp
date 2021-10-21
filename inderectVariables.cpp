#include <iostream>
#include <tuple>
#include "inderectVariables.h"
#include "variables.h"


double tari = 6.25;
int sym_per_bit = 1;
int trext = 1;


VariablesFromTari variable_from_tari{GetVariablesFromTari(tari)};
Bitrate bitrate{GetBitrate(variable_from_tari.rtcal, variable_from_tari.blf, sym_per_bit)};

Preamble preamble{GetPreamble(tari, variable_from_tari.rtcal, variable_from_tari.trcal, trext, sym_per_bit)};
DurationFromReader duration_from_reader{GetDurationFromReader(bitrate.reader, preamble.t_full,
                                                                    preamble.t_sync)};
DurationFromTag duration_from_tag{GetDurationFromTag(preamble.tag_length, bitrate.tag)};


DurationSuccessCommands GetDurationSuccessCommands()
{
    DurationSuccessCommands duration{};
    duration.epc = duration_from_reader.qrep + 2*variable_from_tari.t1_and_t2 + duration_from_tag.rn16 +
            duration_from_reader.ack + duration_from_tag.epcid;
    duration.tid = duration_from_reader.req_rn + 2*variable_from_tari.t1_and_t2 + duration_from_tag.new_rn16 +
            duration_from_reader.read + duration.tid;
    return duration;
}

DurationInvalidCommands GetDurationInvalidCommands()
{
    DurationInvalidCommands duration{};
    duration.empty_slot = duration_from_reader.qrep + variable_from_tari.t1_and_t3;
    duration.collided_slot = duration_from_reader.qrep + variable_from_tari.t1_and_t2 + duration_from_tag.rn16;
    duration.rn16 = duration.collided_slot + duration_from_reader.ack + variable_from_tari.t1_and_t3;
    duration.new_rn_16 = duration_from_reader.req_rn + variable_from_tari.t1_and_t2 + duration_from_tag.new_rn16;
    return duration;
}

