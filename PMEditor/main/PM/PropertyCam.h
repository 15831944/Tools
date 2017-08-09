#pragma once
#include "ObjectInProperty.h"

namespace MVC{
namespace Camera{
class CCamera;
class CPropertyCamera : public Dialog::CObjectInProperty
{
private:
	bool m_bAdd;									//!< 表示是否是添加状态，true表示添加变量，false表示修改变量
	CCamera* m_Item;								//!< 当前所指向的变量，如果是新建，这个为空
	boost::shared_ptr<CCamera> m_ShowItem;			//!< 要显示用的变量

public:
	boost::shared_ptr<CCamera> m_NewItem;			//!< 记录上次添加的变量的所有信息，这次添加时显示上次添加的信息
	void SetType(bool add = true){m_bAdd = add;}
	void SetItem(CCamera* item){m_Item = item;}

public:
	CPropertyCamera(void);
	~CPropertyCamera(void);

private:
	virtual void OnShowHelp();										//!< 显示帮助
	virtual void ShowInfo(CXTPPropertyGrid& grid);					//!< 显示表格的函数
	virtual void OnGridFirstShow(CXTPPropertyGrid& grid);			//!< 表格初始化完成后
	virtual CString GetTitle();										//!< 获得本窗体的名称
	virtual void OnCloseGrid();										//!< 当关闭表格时
	virtual void OnItemModify(CXTPPropertyGrid& grid, UINT id);		//!< 当某一项被修改时
	virtual bool OnSaveModify(CXTPPropertyGrid& grid);				//!< 要保存被改变的内容
	virtual void OnButtonClick(CXTPPropertyGrid& grid, UINT btID);	//!< 按钮被按下
	void CreateNew();
	void CreateEdit();

	void ShowAndHide(CXTPPropertyGrid& grid){;}						//!< 在这里统一做显示与隐藏
	void EnableAndDisenable(CXTPPropertyGrid& grid);				//!< 在这里统一使能
	bool ExistSamePort(int nPort);									//!< 端口是否重了
};
}}
