#pragma once
#define UNM_HYPERLINK_CLICKED	(WM_APP + 0x100)
#include "../../Libs/XToolTip/XSuperTooltip.h"

namespace Tool{
// ������ʾ���ı��ṹ��
typedef struct tagTooltipText{
	CString txtHead;		// ����
	CString txtBody;		// ����
	CString txtRemark;		// ��ע

	tagTooltipText(){
		txtHead="����";
		txtBody="����";
		txtRemark="��ע";
	}

	void init(CString head,CString body,CString remark)
	{
		txtHead=head;
		txtBody=body;
		txtRemark=remark;
	}
}TooltipText,*PTooltipText;

// CXTooltip
// ��ʾ������
class CXTooltip : public CXSuperTooltip
{
	DECLARE_DYNAMIC(CXTooltip)

public:
	SUPER_TOOLTIP_INFO m_Sti;

public:
	CXTooltip();
	virtual ~CXTooltip();

	void AddTooltipText(CWnd* pWnd,TooltipText& txtGroup);		// �����ʾ���ı�

protected:
	DECLARE_MESSAGE_MAP()
};
}
