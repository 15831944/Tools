using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UView.View.Toolbar
{
    class ToolbarStandard : ToolbarBase
    {
        protected override void InitializeToolbar()
        {
            this._itemGroup = new DevExpress.XtraBars.Bar()
            {
                BarName = "Standard",
                DockCol = 0,
                DockRow = 1,
                DockStyle = DevExpress.XtraBars.BarDockStyle.Top,
                FloatLocation = new System.Drawing.Point(204, 187),
                Text = "Standard"
            };
            this._itemGroup.OptionsBar.AllowQuickCustomization = false;
            base.InitializeToolbar();

            // File
            //_editor.BarMgr.Items.Add(this._barSubItem_File);
            //_editor.BarMgr.MainMenu.LinksPersistInfo.Add(new DevExpress.XtraBars.LinkPersistInfo(this._barSubItem_File));
            //
            // Button Menu
            InitToolButton("NewSln", "NewSln", UImageResource.Properties.Resource.Document);
            //InitToolButton("Quit (&X)", "Quit", true, new DevExpress.XtraBars.BarShortcut(System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.F4));
        }

        //protected override void ButtonItemClick(object sender, EventArgs e)
        //{
        //    
        //}
    }
}
