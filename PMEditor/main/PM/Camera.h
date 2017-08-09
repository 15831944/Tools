#pragma once

namespace MVC{namespace Camera{
//!< 摄像头对象
class CCamera
{
private:
	int	m_nID;						//!< 编号
	CString m_strTag;				//!< 说明
	bool m_bRecord;					//!< 是否启动录制功能
	//bool m_bAutoSave;				//!< 是否自动保存视频文件
	CString m_strFilePath;			//!< 视频文件存储位置,最后带'\',默认我的文档
	CString m_strEncodeType;		//!< 视频编码格式,目前只能选WMV2
	CString m_strDistinguish;		//!< 录制分辨率,160*120,176*144,......,720*480,默认320*240
	CString m_strFPS;				//!< 录制帧速率29.97,15,10,5,1,0.5 fps,默认1fps
	bool m_bBroadCast;				//!< 是否启动网络广播
	int m_nPort;					//!< 广播端口号,8080
	int m_nMaxClient;				//!< 最大连接数,默认5,范围0~50
	CString m_strBroadEncodeType;	//!< 广播视频编码格式,目前只能选WMV2
	CString m_strBroadDistinguish;	//!< 广播分辨率,160*120,176*144,......,720*480
	CString m_strBroadFPS;			//!< 广播帧速率29.97,15,10,5,1,0.5 fps

public:
	int GetID(){return m_nID;}										//!< 编号
	CString GetTag(){return m_strTag;}								//!< 说明
	bool IsRecord(){return m_bRecord;}								//!< 是否启动录制功能
	//bool IsAutoSave(){return m_bAutoSave;}							//!< 是否自动保存视频文件
	CString GetFilePath(){return m_strFilePath;}					//!< 视频文件存储位置,最后带'\',默认我的文档
	CString GetEncodeType(){return m_strEncodeType;}				//!< 视频编码格式,目前只能选WMV2
	CString GetDistinguish(){return m_strDistinguish;}				//!< 录制分辨率,160*120,176*144,......,720*480,默认320*240
	CString GetFPS(){return m_strFPS;}								//!< 录制帧速率29.97,15,10,5,1,0.5 fps,默认1fps
	bool IsBroadCast(){return m_bBroadCast;}						//!< 是否启动网络广播
	int GetPort(){return m_nPort;}									//!< 广播端口号,8080
	int GetMaxClient(){return m_nMaxClient;}						//!< 最大连接数,默认5,范围0~50
	CString GetBroadEncodeType(){return m_strBroadEncodeType;}		//!< 广播视频编码格式,目前只能选WMV2
	CString GetBroadDistinguish(){return m_strBroadDistinguish;}	//!< 广播分辨率,160*120,176*144,......,720*480
	CString GetBroadFPS(){return m_strBroadFPS;}					//!< 广播帧速率29.97,15,10,5,1,0.5 fps

	void SetID(int id);												//!< 编号
	void SetTag(CString strTag);									//!< 说明
	void SetRecord(bool bRecord);									//!< 是否启动录制功能
	//void SetAutoSave(bool bAutoSave);								//!< 是否自动保存视频文件
	void SetFilePath(CString strFilePath);							//!< 视频文件存储位置,最后带'\',默认我的文档
	void SetEncodeType(CString strEncodeType);						//!< 视频编码格式,目前只能选WMV2
	void SetDistinguish(CString strDistinguish);					//!< 录制分辨率,160*120,176*144,......,720*480,默认320*240
	void SetFPS(CString strFPS);									//!< 录制帧速率29.97,15,10,5,1,0.5 fps,默认1fps
	void SetBroadCast(bool bBroadCast);								//!< 是否启动网络广播
	void SetPort(int nPort);										//!< 广播端口号,8080
	void SetMaxClient(int nMaxClient);								//!< 最大连接数,默认5,范围0~50
	void SetBroadEncodeType(CString strBroadEncodeType);			//!< 广播视频编码格式,目前只能选WMV2
	void SetBroadDistinguish(CString strBroadDistinguish);			//!< 广播分辨率,160*120,176*144,......,720*480
	void SetBroadFPS(CString strBroadFPS);							//!< 广播帧速率29.97,15,10,5,1,0.5 fps

private:
	void SetModify();												//!< 设置为改变

public:
	CCamera(void);
	~CCamera(void);
	bool SerializeXml(TiXmlElement* pNode, bool bRead);				//!< 保存xml格式的编辑信息,iExport=false正常
	bool DoSearch(CString str, bool bMatchWhole, bool bAllCase, bool bRegex = false);	//!< 查找本变量，要匹配的文本，全字符匹配，大小写匹配
	CCamera& operator = (CCamera& item);							//!< 赋值操作，包括ID和名称也全都赋过去了
	bool operator == (CCamera& item) const;							//!< 判断本变量是否与item属性一样
};
}}