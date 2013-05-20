#ifndef PARSER_H
#define PARSER_H

#include "types.h"
#include <vector>
#include <QString>
#include <QHash>

class Parser
{
public:
    Parser();

    std::vector<PacketInfo> getPacketList(QString filename);
    TSPacket getTSPacket(PacketInfo packetInfo);
    QHash<uint16_t, Program> getProgramInfo();
    QHash<uint16_t, ProgramStreamType> getPIDList();

private:
    int getPacketLength();
    std::vector<Program> parsePATPrograms(int programCount);
    void printPrograms();
    void printPIDList();
    PacketInfo getFirstPacket(uint16_t pid, std::vector<PacketInfo> list);
    uint8_t readNext1B();
    uint16_t readNext2B();
    void skipBytes(int count);
    PMT parsePMT(PacketInfo packetInfo);
    PAT getPATPacket(PacketInfo packetInfo);

    FILE *m_file;
    std::vector<Program> m_programList;

    //obsahuje PID PMT + strukturu PMT spolu s vektorom programov danej PMT
    QHash<uint16_t, Program> m_programList2;

    //pid + programNumber a streamType
    QHash<uint16_t, ProgramStreamType> m_pidList;
};

#endif // PARSER_H
