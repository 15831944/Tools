#pragma once

#define MESSAGE_OK 11111

namespace SoftInfo{

typedef struct tagColInfo{
	CString name;				//!< �ֶ���
	bool bShow;					//!< �Ƿ���ʾ
	UINT width;					//!< ����
	void Init(CString n, bool show, UINT w){name = n; bShow = show; width = w;}
}SColInfo;

class CSoftInfo
{
public:
	std::vector<SColInfo> m_vtColInfo;	//!< ������������ʾ����
	std::vector<SColInfo> m_vtCamCol;	//!< ����ͷ��������ʾ����

private:
	//!< ��������
	UINT m_uiLanguage;					//!< ��ǰ���ԣ�0���ļ��壬1Ӣ�ġ���
	bool m_bHDevMap;					//!< �豸���˵���ʾ����1����0����
	bool m_bTooltip;					//!< ��ͣ��ʾ��Ҫ��Ҫ��
	bool m_bUploadBehaviorFirst;		//!< �Ƿ��������ȶ�һ����Ϊ
	bool m_bShowAddr;					//!< �Ƿ���������ֱ����ʾ�豸��ַ
	UINT m_uiZoomSize;					//!< ���ű�����X%
	CString m_strTPFontName;			//!< ����ͼ�е����ֵ�����
	LOGFONT m_lgTuopuFontInfo;			//!< ����ͼ�е�������Ϣ��ȫ��ֻ�������������塢�ֺ�

	UINT m_uiFreshDataTime;				//!< ˢ�����ݵ�Ĭ������
	bool m_bIsOutPut;					//!< �Ƿ����������������
	bool m_bIsOutWarning;				//!< �Ƿ����������Ϣ
	UINT m_uiCloneNumber;				//!< ��¡�����������ּ������
	UINT m_uiCloneFloat ;				//!< ��¡�������Ƹ����λ��
	UINT m_uiItemFloatWidth;			//!< ����ֵС���㱣��λ��
	bool m_bPrintItemWaterMark;			//!< �Ƿ��ӡˮӡ
	bool m_bShowHex;					//!< �Ƿ�ʮ��������ʾ

	bool m_bCplXml;						//!< �����Ƿ��������true�棬false����
	bool m_bScanXml;					//!< ����ɨ���Ƿ��������true�棬false����
	int m_nCmplOverTime;				//!< ���볬ʱʱ��
	CString m_strItemNabled;			//!< ���������Ƿ����޸�

	CString m_strSerialNo;				//!< ���к�
	bool m_bModbus485;					//!< �Ƿ�ʹ��Modbus485����Ĺ���,������������Ե��뵽485������

private:
	CSoftInfo(void);
	~CSoftInfo(void);

private:
	CString GetFontText(LOGFONT& lf);								//!< ����������Ϣת���ַ��������ڱ���
	void SetFont(LOGFONT&lf, CString strFont);						//!< ���ַ���������������Ϣ

public:
	const CString getSerialNo(){return m_strSerialNo;}				//!< �����ע������к�
	const bool getHDevMap(){return m_bHDevMap;}						//!< ����豸���˵���ʾ����1����0����
	const bool getTooltip(){return m_bTooltip;}						//!< ������˱༭ʱ�Ƿ�Ҫ�������ͣ����ʾ��
	const bool getShowAddr(){return m_bShowAddr;}					//!< �Ƿ���������ֱ����ʾ�豸��ַ
	const bool IsModbus485(){return m_bModbus485;}					//!< �Ƿ�ʹ��Modbus485����Ĺ���,������������Ե��뵽485������}
	const bool getUploadBFirst(){return m_bUploadBehaviorFirst;}	//!< ����Ƿ��������ȶ�һ����Ϊ
	const bool IsCplXml(){return m_bCplXml;}						//!< ��ñ����Ƿ��������true�棬false����
	const bool IsScanXml(){return m_bScanXml;}						//!< ��ñ���ɨ���Ƿ��������true�棬false����
	const UINT getZoomSize(){return m_uiZoomSize;}					//!< ������ű�������λ%
	const UINT getFreshDataTime(){return m_uiFreshDataTime;}		//!< ���ˢ�����ݵ�Ĭ������
	const bool isShowHex(){return m_bShowHex;}						//!< �Ƿ�ʮ��������ʾ
	const bool isOutPut(){return m_bIsOutPut;}						//!< ����Ƿ����������
	const bool isOutWarning(){return m_bIsOutWarning;}				//!< ����Ƿ����������Ϣ
	const void getTuopuFontInfo(LOGFONT& lgFont)
	{memcpy(&lgFont, &m_lgTuopuFontInfo, sizeof(LOGFONT));}			//!< �������ͼ������Ϣ
	const UINT getCloneNumber(){return m_uiCloneNumber;}
	const UINT getCloneFloat(){return m_uiCloneFloat;}
	const UINT getItemFloatWidth(){return m_uiItemFloatWidth;}
	const bool isItemWaterMark(){return m_bPrintItemWaterMark;}		//!< ��ӡ������ʱ�Ƿ��ӡˮӡ
	const bool isItemNabled(){return m_strItemNabled == _T("Yes");}	//!< ���������Ƿ����޸�
	const int getCompileOverTime(){return m_nCmplOverTime;}			//!< ���볬ʱʱ��

	void setSerialNo(CString strNo){m_strSerialNo = strNo;}			//!< �������к�
	void setHDevMap(bool h){m_bHDevMap = h;}						//!< �����豸���˵���ʾ����1����0����
	void setTooltip(bool h){m_bTooltip = h;}						//!< �������˱༭ʱ�Ƿ�Ҫ�������ͣ����ʾ��
	void setUploadBFrist(bool ubf){m_bUploadBehaviorFirst = ubf;}	//!< �����Ƿ��������ȶ�һ����Ϊ
	void setShowAddr(bool bShow){m_bShowAddr = bShow;}				//!< �Ƿ���������ֱ����ʾ�豸��ַ
	void setModbus485(bool b485){m_bModbus485 = b485;}				//!< �Ƿ�ʹ��Modbus485����Ĺ���,������������Ե��뵽485������}
	void setZoomSize(UINT size){if(size >= 50 && size <= 500)		m_uiZoomSize = size;}				//!< �������ű�������λ%
	void setOutPut(bool output){m_bIsOutPut = output;}				//!< �����Ƿ����������
	void setOutWarning(bool bOut){m_bIsOutWarning = bOut;}			//!< �����Ƿ����������Ϣ
	void setTuopuFont(LOGFONT lgFont)
	{memcpy(&m_lgTuopuFontInfo, &lgFont, sizeof(LOGFONT));}			//!< ��������ͼ������Ϣ
	void setCloneNumber(UINT number){m_uiCloneNumber = number;}		
	void setCloneFloat(UINT f){m_uiCloneFloat = f;}
	void setItemFloatWidth(UINT width){m_uiItemFloatWidth = width;}
	void setItemWaterMark(bool b){m_bPrintItemWaterMark = b;}		//!< ���ô�ӡ������ʱ�Ƿ��ӡˮӡ
	void setCmpXml(bool b);											//!< ���ñ����ļ��Ƿ񱣴�����
	void setScanXml(bool b);										//!< ����ɨ���ļ��Ƿ񱣴�����

	void SerializeXml(TiXmlElement *pNode, bool bRead);
	void SerializeItem(TiXmlElement *pNode, bool bRead);
	void SerializeTuoPu(TiXmlElement *pNode, bool bRead);
	void SerializeServer(TiXmlElement *pNode, bool bRead);
	void SerializeCompile(TiXmlElement *pNode, bool bRead);
	void SerializePower(TiXmlElement *pNode, bool bRead);

public:
	static CSoftInfo& GetMe();
	void LoadFile();
	void SaveFile();

	void SetFreshDataTime(UINT fdt);
	void setShowHex(bool b);										//!< ������ʾʮ��������ֵ
};
}