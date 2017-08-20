using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.Runtime.InteropServices;
using System.Reflection;

using DevExpress.LookAndFeel;
using DevExpress.Skins;
using DevExpress.XtraBars;
using DevExpress.XtraNavBar;
using DevExpress.XtraBars.Docking;


namespace CraneView
{
	public partial class MainEditor : DevExpress.XtraEditors.XtraForm
	{
		private Project.ProjectMgr _projMgr = new Project.ProjectMgr();
		internal Project.ProjectMgr ProjMgr { get { return _projMgr; } }
		private MdiClient _mdiClient;
		private List<View.Panel.PanelControl> _ltPanel = new List<View.Panel.PanelControl>();
		internal List<View.Panel.PanelControl> LtPanel { get { return _ltPanel; } }

		//public View.Panel.PanelFind PanelFind1 { get { return _panelFind1; } }

		internal DevExpress.XtraBars.Docking.DockManager DockMgr { get { return _dockMgr; } }

		public MainEditor()
		{
			var splash = new SplashForm();
			splash.Show();
			splash.ShowInfo("欢迎使用CraneView产品");

			//Operation.bEditEnvironment = true;
			//CEditEnvironmentGlobal.mdiparent = this;
			ProjMgr.Editor = this;

			splash.ShowInfo("正在初始化功能控件");
			InitializeComponent();
			View.Panel.PanelControl.CreatePanels(this);

			foreach (var c in Controls)
			{
				if (c is MdiClient) {
					_mdiClient = c as MdiClient;
					_mdiClient.Paint += new PaintEventHandler(MdiClient_Paint);
					break;
				}
			}	

			//VarTableUITypeEditor.GetVar = new GetVarTable(CForDCCEControl.GetVarTableEvent);
			//VarTableImageManage.GetImage = new GetImageName(CForDCCEControl.GetImage);//图片资源
			//this.helpProvider1.HelpNamespace = AppDomain.CurrentDomain.BaseDirectory + "..\\..\\Doc\\DView使用说明.chm";

			splash.ShowInfo("正在初始化ToolBar");
			LoadBarLayout();

			//CEditEnvironmentGlobal.scriptUnitForm = new ScriptUnit();
			//cb = new ComboBox();
			//cb.Visible = false;
			//cb.DropDownStyle = ComboBoxStyle.DropDownList;
			//dataGridView1.Controls.Add(cb);

			//_slnTree.Nodes[0].Text = "工程(" + _projMgr.FirstProject().Name + ")";
			//_slnTree.ExpandAll();
			
			splash.ShowInfo("正在初始化菜单");
			//LoadShapeLibraryConfig();		// Shape configuration
			//
			//try
			//{
			//	splash.ShowInfo("正在初始化帮助信息");
			//	DataTable dt = new DataTable("HelpInfo");
			//	dt.Columns.Add("ClassName", typeof(string));
			//	dt.Columns.Add("FileName", typeof(string));
			//	dt.Columns.Add("Topic", typeof(string));
			//	dt.ReadXml(AppDomain.CurrentDomain.BaseDirectory + "HelpConfig.xml");
			//	foreach (DataRow dr in dt.Rows)
			//	{
			//		if (!CEditEnvironmentGlobal.helpInformationDict.ContainsKey(dr["ClassName"].ToString()))
			//			CEditEnvironmentGlobal.helpInformationDict.Add(dr["ClassName"].ToString(), new string[] { dr["FileName"].ToString(), dr["Topic"].ToString() });
			//		else
			//			CEditEnvironmentGlobal.helpInformationDict[dr["ClassName"].ToString()] = new string[] { dr["FileName"].ToString(), dr["Topic"].ToString() };
			//	}
			//}
			//catch (Exception)
			//{
			//}
			//
			//splash.ShowInfo("正在初始化图库");
			//DataRow[] drs = ShapeLibraryControlDataSet.Tables["Category"].Select("", "SN");
			//foreach (DataRow dr in drs)
			//{
			//	DevExpress.XtraNavBar.NavBarGroup nbg = new NavBarGroup(dr["Label"].ToString());
			//	nbg.Name = dr["Label"].ToString();
			//	navBarControl_图库.Groups.Add(nbg);
			//}
			//InitLibraryControl();
			//
			//splash.ShowInfo("正在初始化控件事件");
			//repositoryItemZoomTrackBar1.EditValueChanging += new DevExpress.XtraEditors.Controls.ChangingEventHandler(repositoryItemZoomTrackBar1_EditValueChanging);
			//repositoryItemSpinEdit1.EditValueChanging += new DevExpress.XtraEditors.Controls.ChangingEventHandler(repositoryItemSpinEdit1_EditValueChanging);
			//xtraScrollableControl_图库.MouseWheel += new MouseEventHandler(xtraScrollableControl1_MouseWheel);
			//this.MouseWheel += new MouseEventHandler(MDIParent1_MouseWheel);
			//userCommandControl21.OnButtonEnableChanged += new EventHandler(userCommandControl21_OnButtonEnableChanged);
			//cb.DropDownClosed += new EventHandler(cb_DropDownClosed);
			//
			//splash.ShowInfo("正在初始化界面样式");
			DevExpress.UserSkins.OfficeSkins.Register();
			//DevExpress.UserSkins.BonusSkins.Register();
			DevExpress.Skins.SkinManager.EnableFormSkinsIfNotVista();
			DevExpress.LookAndFeel.UserLookAndFeel.Default.SetWindowsXPStyle();
			//
			//_barMgr.ForceInitialize();
			//
			LoadSkin();
			//
			//if (clocktimer == null)
			//{
			//	AutoResetEvent autoEvent = new AutoResetEvent(false);
			//	clocktimer = new System.Threading.Timer(new TimerCallback(clocktext), autoEvent, 0, 1000);
			//}

			//_projMgr.SaveProjectHeadInfo();
			_projMgr.LoadProjectHeadInfo();
		}

		private void MainEditor_Load(object sender, EventArgs e)
		{
			View.ProjectForm pf = new View.ProjectForm(this);
			if (pf.ShowDialog() != System.Windows.Forms.DialogResult.OK)
			{
				this.Close();
			}
		}


		private string DockConfigurationFileName()
		{
			string fileName = "DockConfiguration.cfg";
			//switch (ProjectType)
			//{
			//	case 0:
			//		fileName = "DockConfig_Native.xml";
			//		break;
			///	case 1:
			//		fileName = "DockConfig_HTML5.xml";
			//		break;
			//}
			return fileName;
		}

		/// <summary>
		/// Reset toolbar from configuration
		/// </summary>
		private void LoadBarLayout()
		{
			var fileName = DockConfigurationFileName();
			if (File.Exists(AppDomain.CurrentDomain.BaseDirectory + fileName))
				_barMgr.RestoreLayoutFromXml(AppDomain.CurrentDomain.BaseDirectory + fileName);
		}

		private void SaveBarLayout()
		{
			var fileName = DockConfigurationFileName();
			_barMgr.SaveLayoutToXml(AppDomain.CurrentDomain.BaseDirectory + fileName);
		}

		private void LoadSkin()
		{
			//string defaultSkin = "Office 2010 Black";
			string defaultSkin = "Black";
			//try
			//{
			//	if (File.Exists(AppDomain.CurrentDomain.BaseDirectory + "ThemeConfig.txt"))
			//		defaultSkin = File.ReadAllText(AppDomain.CurrentDomain.BaseDirectory + "ThemeConfig.txt");
			//}
			//catch (System.Exception)
			//{
			//}

			foreach (SkinContainer skin in SkinManager.Default.Skins)
			{
				//BarCheckItem item = new BarCheckItem(barManager1, false);
				//item.Caption = cnt.SkinName;
				//menubarSubItem_主题.AddItem(item);
				//item.ItemClick += new ItemClickEventHandler(SkinItemClick);
				if (defaultSkin == skin.SkinName)
				{
					//menubarCheckItem_默认主题.Checked = false;
					//item.Checked = true;
					UserLookAndFeel.Default.SetSkinStyle(skin.SkinName);
				}
			}
		}
		private void MdiClient_Paint(object sender, PaintEventArgs e)
		{ }

	}
}
