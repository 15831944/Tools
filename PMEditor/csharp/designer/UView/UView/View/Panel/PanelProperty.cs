using System;
using System.Windows.Forms;

namespace UView.View.Panel
{
    internal class PanelProperty : PanelControl
    {
        private MainEditor _editor;
        private System.Windows.Forms.PropertyGrid _propertyGrid = new System.Windows.Forms.PropertyGrid();

        internal PanelProperty() { }

        internal override void InitializeComponent(MainEditor editor)
        {
            _editor = editor;

            // _propertyGrid
            this._propertyGrid.BackColor = System.Drawing.SystemColors.Control;
            this._propertyGrid.Dock = System.Windows.Forms.DockStyle.Fill;
            this._propertyGrid.ImeMode = System.Windows.Forms.ImeMode.Off;
            this._propertyGrid.Location = new System.Drawing.Point(0, 0);
            this._propertyGrid.Name = "_propertyGrid";
            this._propertyGrid.Size = new System.Drawing.Size(199, 554);
            this._propertyGrid.TabIndex = 0;
            this._propertyGrid.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this._propertyGrid_PropertyValueChanged);
            this._propertyGrid.Enter += new System.EventHandler(this._propertyGrid_Enter);
            InitializeCtrl(this._propertyGrid, "Property", $"Property", DevExpress.XtraBars.Docking.DockingStyle.Right);
        }

        /// <summary>
        /// 进入属性表
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void _propertyGrid_Enter(object sender, EventArgs e)
        {
            //if (myPropertyGrid1.SelectedObject is CShape)
            //{
            //	foreach (CShape st in userCommandControl21.theglobal.SelectedShapeList)
            //	{
            //		userCommandControl21.theglobal.OldShapes.Add(st.Copy());
            //	}
            //}
        }

        private void _propertyGrid_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            //if (e.ChangedItem == null || e.OldValue == e.ChangedItem.Value)
            //	return;
            //
            //if (myPropertyGrid1.SelectedObject is CShape)
            //{
            //	CEditEnvironmentGlobal.dhp.dirtyPageAdd(CEditEnvironmentGlobal.childform.theglobal.df.name);
            //}
            //else if (myPropertyGrid1.SelectedObject is CPageProperty)
            //{
            //	if (e.ChangedItem.Label == "脚本名称")
            //	{
            //
            //	}
            //	else
            //	{
            //		CEditEnvironmentGlobal.dhp.dirtyPageAdd(CEditEnvironmentGlobal.childform.theglobal.df.name);
            //	}
            //}
            //else if (myPropertyGrid1.SelectedObject is CWebPageProperty)
            //{
            //	if (e.ChangedItem.Label == "脚本名称")
            //	{
            //
            //	}
            //	else
            //	{
            //		CEditEnvironmentGlobal.dhp.dirtyPageAdd(CEditEnvironmentGlobal.childform.theglobal.df.name);
            //	}
            //}
            //
            //if (e.ChangedItem.Label == "名称")
            //{
            //	String NewValue = (String)e.ChangedItem.Value;
            //	if (myPropertyGrid1.SelectedObject is CShape)
            //		((CShape)myPropertyGrid1.SelectedObject).Name = (String)e.OldValue;
            //	else if (myPropertyGrid1.SelectedObject is IControlShape)
            //		((IControlShape)myPropertyGrid1.SelectedObject).ID = (String)e.OldValue;
            //	List<string> temperr = CheckIOExists.ShapeInUse(CEditEnvironmentGlobal.childform.theglobal.df.name + "." + NewValue);
            //	if (temperr.Count != 0)
            //	{
            //		delPage ff = new delPage(temperr, "该图形正在被引用,是否仍继续操作.");
            //		if (ff.ShowDialog() != DialogResult.Yes)
            //			goto finish;
            //	}
            //	System.Text.RegularExpressions.Regex regex = new System.Text.RegularExpressions.Regex(@"^[a-zA-Z_][a-zA-Z0-9_]*$");
            //	if (!regex.IsMatch(NewValue))
            //	{
            //		MessageBox.Show("名称中包含非法字符.", "错误");
            //		return;
            //	}
            //	foreach (CShape st in CEditEnvironmentGlobal.childform.theglobal.g_ListAllShowCShape)
            //	{
            //		if ((myPropertyGrid1.SelectedObject is CShape) && st == (CShape)myPropertyGrid1.SelectedObject)
            //			continue;
            //		else if ((st is CControl) && (((CControl)st)._c == myPropertyGrid1.SelectedObject))
            //			continue;
            //		else if (st.ShapeName == NewValue)
            //		{
            //			System.Windows.Forms.MessageBox.Show("名称重复", "错误");
            //			myPropertyGrid1.Refresh();
            //			goto finish;
            //		}
            //	}
            //	foreach (String oneWord in CheckScript.Keystr)
            //	{
            //		if (NewValue == oneWord)
            //		{
            //			MessageBox.Show("名称中含有关键字将会导致错误。");
            //			goto finish;
            //		}
            //	}
            //	if (myPropertyGrid1.SelectedObject is CShape)
            //	{
            //		((CShape)myPropertyGrid1.SelectedObject).Name = NewValue;
            //		objView_Page.OnFresh(((CShape)myPropertyGrid1.SelectedObject).ShapeID.ToString());
            //	}
            //	else if (myPropertyGrid1.SelectedObject is IControlShape)
            //	{
            //		((IControlShape)myPropertyGrid1.SelectedObject).ID = NewValue;
            //	}
            //}
            //
            //userCommandControl21.theglobal.uc2.RefreshGraphics();
            //
            //try
            //{
            //	if (e.ChangedItem.Label == "页面名称")
            //	{
            //		string pageName = String.Empty;
            //		String NewValue = (String)e.ChangedItem.Value;
            //
            //		if (myPropertyGrid1.SelectedObject is CPageProperty)
            //		{
            //			((CPageProperty)myPropertyGrid1.SelectedObject).ShowName = (String)e.OldValue;
            //			pageName = ((CPageProperty)myPropertyGrid1.SelectedObject).PageName;
            //		}
            //		else if (myPropertyGrid1.SelectedObject is CWebPageProperty)
            //		{
            //			((CWebPageProperty)myPropertyGrid1.SelectedObject).ShowName = (String)e.OldValue;
            //			pageName = ((CWebPageProperty)myPropertyGrid1.SelectedObject).PageName;
            //		}
            //
            //		if (NewValue == "")
            //		{
            //			MessageBox.Show("名称不能为空,请重新输入", "错误");
            //			goto finish;
            //		}
            //
            //		foreach (DataFile df in CEditEnvironmentGlobal.dfs)
            //		{
            //			if (df.pageName == NewValue)
            //			{
            //				MessageBox.Show("名称重复,请重新输入", "错误");
            //				goto finish;
            //			}
            //		}
            //
            //		if (myPropertyGrid1.SelectedObject is CPageProperty)
            //		{
            //			((CPageProperty)myPropertyGrid1.SelectedObject).ShowName = NewValue;
            //		}
            //		else if (myPropertyGrid1.SelectedObject is CWebPageProperty)
            //		{
            //			((CWebPageProperty)myPropertyGrid1.SelectedObject).ShowName = NewValue;
            //		}
            //
            //		//修改页面关联节点的数据
            //		foreach (var node in treeView_工程导航.Nodes[0].Nodes[0].Nodes.Find(pageName, true))
            //		{
            //			node.Text = e.ChangedItem.Value.ToString();
            //			var page = node.Tag as HmiPage;
            //			if (page != null)
            //			{
            //				page.Text = e.ChangedItem.Value.ToString();
            //			}
            //		}
            //	}
            //	else if (e.ChangedItem.Label == "脚本名称")
            //	{
            //		String NewValue = (String)e.ChangedItem.Value;
            //
            //		if (myPropertyGrid1.SelectedObject is CPageProperty)
            //		{
            //			((CPageProperty)myPropertyGrid1.SelectedObject).PageName = (String)e.OldValue;
            //		}
            //		else if (myPropertyGrid1.SelectedObject is CWebPageProperty)
            //		{
            //			((CWebPageProperty)myPropertyGrid1.SelectedObject).PageName = (String)e.OldValue;
            //		}
            //
            //		List<string> temperr = CheckIOExists.PageInUse(CEditEnvironmentGlobal.childform.theglobal.df.name);
            //		if (temperr.Count != 0)
            //		{
            //			delPage f = new delPage(temperr);
            //			if (f.ShowDialog() != DialogResult.Yes)
            //				goto finish;
            //		}
            //
            //		if (NewValue == "")
            //		{
            //			MessageBox.Show("名称不能为空,请从新输入", "错误");
            //			goto finish;
            //		}
            //		foreach (DataFile df in CEditEnvironmentGlobal.dfs)
            //		{
            //			if (df.name == NewValue)
            //			{
            //				MessageBox.Show("名称重复,请从新输入", "错误");
            //				goto finish;
            //			}
            //		}
            //		System.Text.RegularExpressions.Regex regex = new System.Text.RegularExpressions.Regex(@"^[a-zA-Z_][a-zA-Z0-9_]*$");
            //		if (!regex.IsMatch(NewValue))
            //		{
            //			MessageBox.Show("变量名不合法,请从新输入", "错误");
            //			goto finish;
            //		}
            //
            //		if (myPropertyGrid1.SelectedObject is CPageProperty)
            //		{
            //			((CPageProperty)myPropertyGrid1.SelectedObject).PageName = NewValue;
            //		}
            //		else if (myPropertyGrid1.SelectedObject is CWebPageProperty)
            //		{
            //			((CWebPageProperty)myPropertyGrid1.SelectedObject).PageName = NewValue;
            //		}
            //		//当脚本名称改变时，应该使用新的名称添加到脏页面列表中
            //		CEditEnvironmentGlobal.dhp.dirtyPageAdd(NewValue);
            //
            //		//修改页面关联节点的数据
            //		foreach (var node in treeView_工程导航.Nodes[0].Nodes[0].Nodes.Find(e.OldValue.ToString(), true))
            //		{
            //			node.Name = e.ChangedItem.Value.ToString();
            //			var page = node.Tag as HmiPage;
            //			if (page != null)
            //			{
            //				page.Name = e.ChangedItem.Value.ToString();
            //			}
            //		}
            //	}
            //}
            //catch (Exception)
            //{
            //}
            //
            //try
            //{
            //	if (e.ChangedItem.Label == "组内索引号")
            //	{
            //		Int32 NewValue = (Int32)e.ChangedItem.Value;
            //		((CShape)myPropertyGrid1.SelectedObject).GroupIndex = (Int32)e.OldValue;
            //
            //		foreach (CShape st in CEditEnvironmentGlobal.childform.theglobal.g_ListAllShowCShape)
            //		{
            //			if (st.GroupName == ((CShape)myPropertyGrid1.SelectedObject).GroupName)
            //			{
            //				if (st.GroupIndex == NewValue)
            //				{
            //					goto finish;
            //				}
            //
            //			}
            //		}
            //
            //		((CShape)myPropertyGrid1.SelectedObject).GroupIndex = NewValue;
            //	}
            //	if (e.ChangedItem.Label == "组名称")
            //	{
            //		String NewValue = (String)e.ChangedItem.Value;
            //		((CShape)myPropertyGrid1.SelectedObject).GroupName = (String)e.OldValue;
            //		for (int i = 0; i < 1024; i++)
            //		{
            //			foreach (CShape st in CEditEnvironmentGlobal.childform.theglobal.g_ListAllShowCShape)
            //			{
            //				if (st.GroupName == NewValue)
            //				{
            //					if (st.GroupIndex == i)
            //					{
            //						goto goon;
            //					}
            //				}
            //			}
            //			((CShape)myPropertyGrid1.SelectedObject).GroupIndex = i;
            //			((CShape)myPropertyGrid1.SelectedObject).GroupName = NewValue;
            //			break;
            //		goon:
            //			;
            //			if (i == 1023)
            //				goto finish;
            //		}
            //	}
            //}
            //catch (System.Exception)
            //{
            //}
            //List<CShape> newshapelist = new List<CShape>();
            //List<CShape> oldshapelist = new List<CShape>();
            //if (myPropertyGrid1.SelectedObject is CShape)
            //{
            //	foreach (CShape st in userCommandControl21.theglobal.SelectedShapeList)
            //	{
            //
            //		foreach (CShape st2 in userCommandControl21.theglobal.OldShapes)
            //		{
            //			if (st.ShapeID == st2.ShapeID)
            //			{
            //				newshapelist.Add(st);
            //				oldshapelist.Add(st2);
            //			}
            //		}
            //		userCommandControl21.theglobal.ForUndo(newshapelist, oldshapelist);
            //	}
            //	userCommandControl21.theglobal.OldShapes.Clear();
            //}
            //
            //finish:
            //userCommandControl21.theglobal.OldShapes.Clear();
        }

        public System.Windows.Forms.PropertyGrid PropertyGrid { get { return _propertyGrid; } }
    }
}
