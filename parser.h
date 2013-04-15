#ifndef PARSER_H
#define PARSER_H

#include "types.h"
#include <vector>
#include <QString>

class Parser
{
public:
    Parser();

    std::vector<PacketInfo> getPacketsInfo(QString filename);
    TSHeader getTSHeader(PacketInfo packetInfo);
    AFHeader getAFHeader(PacketInfo packetInfo);

private:
    int getPacketLength();

    FILE *m_file;
};

#endif // PARSER_H
