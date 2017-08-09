#pragma once

namespace Tool{
class CMyCrc
{
private:
	CMyCrc(void){;}
	~CMyCrc(void){;}

public:
	static unsigned short CRC16(unsigned char* puchMsg, unsigned short usDataLen);
};
}