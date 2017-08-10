#pragma once
#include "ObjectInProperty.h"

namespace MVC{namespace Item{
class CItem;
class CPropertyItems : public Dialog::CObjectInProperty
{
private:
	std::list< std::shared_ptr<CItem> > m_ltEditItem;	//!< Ҫ���޸ĵ����б���
	std::shared_ptr<CItem> m_ShowItem;				//!< ��ʾ��������ʱ����

	BOOL m_bValType;			//!< ����ֵ�����Ƿ���ͬ
	BOOL m_bSrcType;			//!< ����Դ�����Ƿ���ͬ
	BOOL m_bGroup;				//!< �����������Ƿ���ͬ
	BOOL m_bDefValue;			//!< Ĭ��ֵ�����Ƿ���ͬ
	BOOL m_bAccessRight;		//!< ����Ȩ���Ƿ���ͬ
	BOOL m_bReservFlag;			//!< �Ƿ��Ǳ���ֵ�Ƿ���ͬ
	BOOL m_bReservDB;			//!< �Ƿ��Ǳ������ݿ��Ƿ���ͬ
	BOOL m_bModbus485;			//!< �Ƿ񵼳�Modbus����
	BOOL m_bDev;				//!< �����豸�Ƿ���ͬ
	BOOL m_bFreshTime;			//!< ˢ��ʱ���Ƿ���ͬ
	BOOL m_bDelayFlag;			//!< �Ƿ������ͺ��Ƿ���ͬ
	BOOL m_bConvertFlag;		//!< ת�������Ƿ���ͬ
	BOOL m_bMinProj;			//!< ��С����ֵ�Ƿ���ͬ
	BOOL m_bMaxProj;			//!< ��󹤳�ֵ�Ƿ���ͬ
	BOOL m_bMinIO;				//!< ��Сԭʼֵ�Ƿ���ͬ
	BOOL m_bMaxIO;				//!< ���ԭʼֵ�Ƿ���ͬ
	BOOL m_bBitAlarmType;		//!< ���ر��������Ƿ���ͬ
	BOOL m_bDeadArea;			//!< ��ֵ�����Ƿ���ͬ
	BOOL m_bLoloActive;			//!< �����ޱ��������Ƿ���ͬ
	BOOL m_bLoloValue;			//!< �����ޱ���ֵ�Ƿ���ͬ
	BOOL m_bLowActive;			//!< ���ޱ��������Ƿ���ͬ
	BOOL m_bLowValue;			//!< ���ޱ���ֵ�Ƿ���ͬ
	BOOL m_bHighActive;			//!< ���ޱ��������Ƿ���ͬ
	BOOL m_bHighValue;			//!< ���ޱ���ֵ�Ƿ���ͬ
	BOOL m_bHihiActive;			//!< �����ޱ��������Ƿ���ͬ
	BOOL m_bHihiValue;			//!< �����ޱ���ֵ�Ƿ���ͬ
	BOOL m_bAimActive;			//!< Ŀ�걨�������Ƿ���ͬ
	BOOL m_bAimPercent;			//!< Ŀ�걨���ٷֱ��Ƿ���ͬ
	BOOL m_bAimDeadPercent;		//!< Ŀ�걨���ٷֱ������Ƿ���ͬ
	BOOL m_bAimValue;			//!< Ŀ�걨��ֵ�Ƿ���ͬ
	BOOL m_bShiftActive;		//!< �仯�ʱ��������Ƿ���ͬ
	BOOL m_bShiftPercent;		//!< �仯�ʱ����ٷֱ��Ƿ���ͬ
	BOOL m_bShiftTime;			//!< �仯��ȡֵʱ���Ƿ���ͬ

public:
	CPropertyItems(void);
	~CPropertyItems(void);
	void SetItemList(std::list< std::shared_ptr<CItem> >& ltItem){m_ltEditItem = ltItem;}

private:
	void CreateEdit();												//!< �༭ǰ������׼������
	virtual void OnShowHelp();										//!< ��ʾ������Ϣ
	virtual void ShowInfo(CXTPPropertyGrid& grid);					//!< ��ʾ���ĺ���
	virtual CString GetTitle();										//!< ��ñ����������
	virtual bool OnSaveModify(CXTPPropertyGrid& grid);				//!< Ҫ���汻�ı������
	virtual void OnItemModify(CXTPPropertyGrid& grid, UINT id);		//!< ��ĳһ��޸�ʱ
};
}}