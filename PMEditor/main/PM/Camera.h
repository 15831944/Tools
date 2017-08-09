#pragma once

namespace MVC{namespace Camera{
//!< ����ͷ����
class CCamera
{
private:
	int	m_nID;						//!< ���
	CString m_strTag;				//!< ˵��
	bool m_bRecord;					//!< �Ƿ�����¼�ƹ���
	//bool m_bAutoSave;				//!< �Ƿ��Զ�������Ƶ�ļ�
	CString m_strFilePath;			//!< ��Ƶ�ļ��洢λ��,����'\',Ĭ���ҵ��ĵ�
	CString m_strEncodeType;		//!< ��Ƶ�����ʽ,Ŀǰֻ��ѡWMV2
	CString m_strDistinguish;		//!< ¼�Ʒֱ���,160*120,176*144,......,720*480,Ĭ��320*240
	CString m_strFPS;				//!< ¼��֡����29.97,15,10,5,1,0.5 fps,Ĭ��1fps
	bool m_bBroadCast;				//!< �Ƿ���������㲥
	int m_nPort;					//!< �㲥�˿ں�,8080
	int m_nMaxClient;				//!< ���������,Ĭ��5,��Χ0~50
	CString m_strBroadEncodeType;	//!< �㲥��Ƶ�����ʽ,Ŀǰֻ��ѡWMV2
	CString m_strBroadDistinguish;	//!< �㲥�ֱ���,160*120,176*144,......,720*480
	CString m_strBroadFPS;			//!< �㲥֡����29.97,15,10,5,1,0.5 fps

public:
	int GetID(){return m_nID;}										//!< ���
	CString GetTag(){return m_strTag;}								//!< ˵��
	bool IsRecord(){return m_bRecord;}								//!< �Ƿ�����¼�ƹ���
	//bool IsAutoSave(){return m_bAutoSave;}							//!< �Ƿ��Զ�������Ƶ�ļ�
	CString GetFilePath(){return m_strFilePath;}					//!< ��Ƶ�ļ��洢λ��,����'\',Ĭ���ҵ��ĵ�
	CString GetEncodeType(){return m_strEncodeType;}				//!< ��Ƶ�����ʽ,Ŀǰֻ��ѡWMV2
	CString GetDistinguish(){return m_strDistinguish;}				//!< ¼�Ʒֱ���,160*120,176*144,......,720*480,Ĭ��320*240
	CString GetFPS(){return m_strFPS;}								//!< ¼��֡����29.97,15,10,5,1,0.5 fps,Ĭ��1fps
	bool IsBroadCast(){return m_bBroadCast;}						//!< �Ƿ���������㲥
	int GetPort(){return m_nPort;}									//!< �㲥�˿ں�,8080
	int GetMaxClient(){return m_nMaxClient;}						//!< ���������,Ĭ��5,��Χ0~50
	CString GetBroadEncodeType(){return m_strBroadEncodeType;}		//!< �㲥��Ƶ�����ʽ,Ŀǰֻ��ѡWMV2
	CString GetBroadDistinguish(){return m_strBroadDistinguish;}	//!< �㲥�ֱ���,160*120,176*144,......,720*480
	CString GetBroadFPS(){return m_strBroadFPS;}					//!< �㲥֡����29.97,15,10,5,1,0.5 fps

	void SetID(int id);												//!< ���
	void SetTag(CString strTag);									//!< ˵��
	void SetRecord(bool bRecord);									//!< �Ƿ�����¼�ƹ���
	//void SetAutoSave(bool bAutoSave);								//!< �Ƿ��Զ�������Ƶ�ļ�
	void SetFilePath(CString strFilePath);							//!< ��Ƶ�ļ��洢λ��,����'\',Ĭ���ҵ��ĵ�
	void SetEncodeType(CString strEncodeType);						//!< ��Ƶ�����ʽ,Ŀǰֻ��ѡWMV2
	void SetDistinguish(CString strDistinguish);					//!< ¼�Ʒֱ���,160*120,176*144,......,720*480,Ĭ��320*240
	void SetFPS(CString strFPS);									//!< ¼��֡����29.97,15,10,5,1,0.5 fps,Ĭ��1fps
	void SetBroadCast(bool bBroadCast);								//!< �Ƿ���������㲥
	void SetPort(int nPort);										//!< �㲥�˿ں�,8080
	void SetMaxClient(int nMaxClient);								//!< ���������,Ĭ��5,��Χ0~50
	void SetBroadEncodeType(CString strBroadEncodeType);			//!< �㲥��Ƶ�����ʽ,Ŀǰֻ��ѡWMV2
	void SetBroadDistinguish(CString strBroadDistinguish);			//!< �㲥�ֱ���,160*120,176*144,......,720*480
	void SetBroadFPS(CString strBroadFPS);							//!< �㲥֡����29.97,15,10,5,1,0.5 fps

private:
	void SetModify();												//!< ����Ϊ�ı�

public:
	CCamera(void);
	~CCamera(void);
	bool SerializeXml(TiXmlElement* pNode, bool bRead);				//!< ����xml��ʽ�ı༭��Ϣ,iExport=false����
	bool DoSearch(CString str, bool bMatchWhole, bool bAllCase, bool bRegex = false);	//!< ���ұ�������Ҫƥ����ı���ȫ�ַ�ƥ�䣬��Сдƥ��
	CCamera& operator = (CCamera& item);							//!< ��ֵ����������ID������Ҳȫ������ȥ��
	bool operator == (CCamera& item) const;							//!< �жϱ������Ƿ���item����һ��
};
}}