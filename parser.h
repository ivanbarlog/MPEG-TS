#ifndef PARSER_H
#define PARSER_H

#include "types.h"
#include <vector>
#include <QString>

class Parser
{
public:
    Parser();

    std::vector<PacketInfo> getPacketList(QString filename);
    TSPacket getTSPacket(PacketInfo packetInfo);

private:
    int getPacketLength();
    std::vector<Program> getPAT();
    std::vector<Program> parsePATPrograms(int programCount);

    FILE *m_file;
    std::vector<PacketInfo> m_patPackets;
    std::vector<Program> m_pat;
};

#endif // PARSER_H
