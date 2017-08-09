
namespace Tool{
class CMyDes
{
public:
	CMyDes(void){;}
	~CMyDes(){;}

public:
	enum    {ENCRYPT,DECRYPT};
	static bool Des_Go(char *Out,char *In,long datalen,const char *Key,int keylen,bool Type = ENCRYPT);
};
}
