#ifndef PARSER_H
#define PARSER_H

#include "types.h"


class Parser
{
public:
    Parser();

    void run();
    int getPacketLength(FILE *file);
    TSHeader getTSHeader(FILE *file, PacketInfo packetInfo);
    AFHeader getAFHeader(FILE *file, PacketInfo packetInfo);

private:

};

#endif // PARSER_H
