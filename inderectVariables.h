#ifndef RFID_INDERECTVARIABLES_H
#define RFID_INDERECTVARIABLES_H


struct DurationSuccessCommands
{
    double epc;
    double tid;
};

struct DurationInvalidCommands
{
    double empty_slot;
    double collided_slot;
    double rn16;
    double new_rn_16;
};


DurationSuccessCommands GetDurationSuccessCommands();
DurationInvalidCommands GetDurationInvalidCommands();

#endif //RFID_INDERECTVARIABLES_H
