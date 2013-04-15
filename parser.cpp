#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <iostream>
#include <vector>
#include "parser.h"
#include "types.h"
#include <string>

Parser::Parser()
{
}

void Parser::run()
{
    FILE *file = NULL;
    uint8_t read_data;
    //uint64_t fsize;

    std::vector<PacketInfo> list;


    file = fopen64("/home/dev/public/ts-files/football.ts", "rb");

    //fseeko64(file, 0L, SEEK_END);
    //fsize = ftello64(file);
    //rewind(file);

    int i = 1;
    while (fread(&read_data, sizeof(uint8_t), 1, file) == 1)
    {
        if (i == 31) break;
        if (read_data == SYNC_BYTE)
        {
            uint8_t nextTwoBytes[2] = {0};

            PacketInfo tmp;
            tmp.start = (uint64_t)(ftello64(file) - 1);

            int affected;
            for (int incr = 0; incr < 2; incr++)
            {
               affected = fread(&nextTwoBytes[incr], sizeof(uint8_t), 1, file);
               if (affected != 1)
               {
                   printf("Packet not complete.");
                   return;
               }
            }

            uint16_t pidAssembly = ((nextTwoBytes[0] << 8) | nextTwoBytes[1]);

            tmp.pid = pidAssembly & 0x1fff;

            tmp.length = getPacketLength(file);

            list.push_back(tmp);



            printf("PID: %X, size of packet %d, packet started at %lu\n", tmp.pid, tmp.length, tmp.start);

            i++;
        }
        else
        {
            continue;
        }
    }


    getTSHeader(file, list.at(0));

    std::cout << "Hura, hura, zvladli sme to!" << std::endl;
}

int Parser::getPacketLength(FILE *file)
{
    int result;
    uint8_t sync;

    fseeko64(file, 185L, SEEK_CUR);
    result = fread(&sync, sizeof(uint8_t), 1, file);

    if (result != 1) return -1;
    if (sync == SYNC_BYTE)
    {
        fseeko64(file, -1L, SEEK_CUR);
        return 188;
    }


    fseeko64(file, 15L, SEEK_CUR);
    result = fread(&sync, sizeof(uint8_t), 1, file);

    if (result != 1) return -1;
    if (sync == SYNC_BYTE)
    {
        fseeko64(file, -1L, SEEK_CUR);
        return 204;
    }

    fseeko64(file, 3L, SEEK_CUR);
    result = fread(&sync, sizeof(uint8_t), 1, file);

    if (result != 1) return -1;
    if (sync == SYNC_BYTE)
    {
        fseeko64(file, -1L, SEEK_CUR);
        return 208;
    }

    return -1;
}

TSHeader Parser::getTSHeader(FILE *file, PacketInfo packetInfo)
{
    TSHeader header;
    uint8_t read_data;

    //go to first byte of file
    rewind(file);
    //seek bytes with current packet
    fseeko64(file, packetInfo.start, SEEK_CUR);

    fread(&read_data, sizeof(uint8_t), 1, file);

    if (read_data == SYNC_BYTE) std::cout << "Found SYNC_BYTE" << std::endl;
    //else return; //Cannot find SYNC_BYTE

    return header;
}
