#pragma once
#define UNM_HYPERLINK_CLICKED	(WM_APP + 0x100)
#include "../../Libs/XToolTip/XSuperTooltip.h"

namespace Tool{
// 帮助提示框文本结构体
typedef struct tagTooltipText{
	CString txtHead;		// 标题
	CString txtBody;		// 内容
	CString txtRemark;		// 备注

	tagTooltipText(){
		txtHead="标题";
		txtBody="内容";
		txtRemark="备注";
	}

	void init(CString head,CString body,CString remark)
	{
		txtHead=head;
		txtBody=body;
		txtRemark=remark;
	}
}TooltipText,*PTooltipText;

// CXTooltip
// 提示框窗体类
class CXTooltip : public CXSuperTooltip
{
	DECLARE_DYNAMIC(CXTooltip)

public:
	SUPER_TOOLTIP_INFO m_Sti;

public:
	CXTooltip();
	virtual ~CXTooltip();

	void AddTooltipText(CWnd* pWnd,TooltipText& txtGroup);		// 添加提示框文本

protected:
	DECLARE_MESSAGE_MAP()
};
}
