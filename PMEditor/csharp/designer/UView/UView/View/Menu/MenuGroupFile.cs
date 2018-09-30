using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UView.View.Menu
{
    internal class MenuGroupFile : MenuGroupBase
    {
        private DevExpress.XtraBars.BarSubItem _barSubItem_File;

        protected override void InitMenuButton()
        {
            // File
            _barSubItem_File = new DevExpress.XtraBars.BarSubItem()
            {
                Caption = "File (&F)",
                Name = "File",
                Id = MainEditor.GetItemID()
            };
            _editor.BarMgr.Items.Add(this._barSubItem_File);
            _editor.BarMgr.MainMenu.LinksPersistInfo.Add(new DevExpress.XtraBars.LinkPersistInfo(this._barSubItem_File));

            // Button Menu
            InitMenuButton(_barSubItem_File, "New Solution (&N)", "NewSolution");
            InitMenuButton(_barSubItem_File, "Quit (&X)", "Quit", true, new DevExpress.XtraBars.BarShortcut(System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.F4));
        }

        protected override void ButtonItemClick(object sender, DevExpress.XtraBars.ItemClickEventArgs e)
        {
            var str = e.Item.Name;
        }
    }
}
