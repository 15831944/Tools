using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UView.View.Menu
{
    internal class MenuGroupFile : MenuGroupBase
    {
        protected override void InitMenuButton()
        {
            // File
            _itemGroup = new DevExpress.XtraBars.BarSubItem()
            {
                Caption = "File (&F)",
                Name = "File",
                Id = MainEditor.GetItemID()
            };
            _editor.BarMgr.Items.Add(this._itemGroup);
            _editor.BarMgr.MainMenu.LinksPersistInfo.Add(new DevExpress.XtraBars.LinkPersistInfo(this._itemGroup));

            // Button Menu
            InitMenuButton("New Solution (&N)", "NewSolution");
            InitMenuButton("Quit (&X)", "Quit", true, new DevExpress.XtraBars.BarShortcut(System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.F4));
        }

        protected override void ButtonItemClick(object sender, DevExpress.XtraBars.ItemClickEventArgs e)
        {
            var str = e.Item.Name;
        }
    }
}
