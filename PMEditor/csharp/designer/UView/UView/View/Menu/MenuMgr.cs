using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UView.View.Menu
{
    internal class MenuMgr
    {
        private DevExpress.XtraBars.BarSubItem _barSubItem_File = new DevExpress.XtraBars.BarSubItem();
        private DevExpress.XtraBars.BarButtonItem _barButtonItem_NewSolution = new DevExpress.XtraBars.BarButtonItem();
        private DevExpress.XtraBars.BarButtonItem _barButtonItem_Quit = new DevExpress.XtraBars.BarButtonItem();

        internal void InitMainMenu(DevExpress.XtraBars.Bar menuBar)
        {
            InitMenuFile();

            menuBar.LinksPersistInfo.AddRange(new DevExpress.XtraBars.LinkPersistInfo[] {
                new DevExpress.XtraBars.LinkPersistInfo(this._barSubItem_File)
                //new DevExpress.XtraBars.LinkPersistInfo(this.barSubItem_主菜单_编辑),
                //new DevExpress.XtraBars.LinkPersistInfo(this.barSubItem_主菜单_视图),
                //new DevExpress.XtraBars.LinkPersistInfo(this.barSubItem_主菜单_操作),
                //new DevExpress.XtraBars.LinkPersistInfo(this.barSubItem_主菜单_脚本),
                //new DevExpress.XtraBars.LinkPersistInfo(this.barSubItem_主菜单_工具),
                //new DevExpress.XtraBars.LinkPersistInfo(this.barSubItem_主菜单_窗口),
                //new DevExpress.XtraBars.LinkPersistInfo(this.barSubItem_主菜单_帮助),
                //new DevExpress.XtraBars.LinkPersistInfo(DevExpress.XtraBars.BarLinkUserDefines.PaintStyle, this.menubarCheckItem_全屏显示, DevExpress.XtraBars.BarItemPaintStyle.CaptionGlyph)
            });
        }

        private void InitMenuFile()
        {
            InitMenuButton();

            this._barSubItem_File.Caption = "File (&F)";
            this._barSubItem_File.Id = 2;
            this._barSubItem_File.Name = "_barSubItem_File";
            this._barSubItem_File.LinksPersistInfo.AddRange(new DevExpress.XtraBars.LinkPersistInfo[] {
            new DevExpress.XtraBars.LinkPersistInfo(this._barButtonItem_NewSolution, true),
            new DevExpress.XtraBars.LinkPersistInfo(this._barButtonItem_Quit, true)});
        }

        private void InitMenuButton()
        {
            this._barButtonItem_NewSolution.Caption = "New Solution (&N)";
            //this._barButtonItem_NewSolution.Glyph = global::HMIEditEnvironment.Properties.Resources.DocumentHS;   // image
            this._barButtonItem_NewSolution.Id = 3;
            //this._barButtonItem_NewSolution.ItemShortcut = barShortcut14;
            this._barButtonItem_NewSolution.Name = "_barButtonItem_NewSolution";
            //this._barButtonItem_NewSolution.ItemClick += new DevExpress.XtraBars.ItemClickEventHandler(this.ToolStripMenuItem_页面组_新建页面_Click);

            this._barButtonItem_Quit.Caption = "Quit (&X)";
            this._barButtonItem_Quit.Id = 4;
            this._barButtonItem_Quit.ItemShortcut = new DevExpress.XtraBars.BarShortcut(System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.F4);
            this._barButtonItem_Quit.Name = "_barButtonItem_Quit";
            //this._barButtonItem_Quit.ItemClick += new DevExpress.XtraBars.ItemClickEventHandler(this.barButtonItem_退出_ItemClick);
        }
    }
}
