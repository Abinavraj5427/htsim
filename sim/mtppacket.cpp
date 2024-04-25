#include "mtppacket.h"

PacketDB<MtpPacket> MtpPacket::_packetdb;
PacketDB<MtpAck> MtpAck::_packetdb;
PacketDB<MtpAtx> MtpAtx::_packetdb;
PacketDB<MtpArx> MtpArx::_packetdb;