using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UView.View.Menu
{
    internal abstract class MenuGroupBase : IMenuGroup
    {
        protected MainEditor _editor;

        void IMenuGroup.InitMenu(MainEditor editor)
        {
            _editor = editor;
            InitMenuButton();
        }
        
        protected virtual void InitMenuButton() { }

        protected DevExpress.XtraBars.BarButtonItem InitMenuButton(DevExpress.XtraBars.BarSubItem parent, string caption, string name, bool bGroup = false, DevExpress.XtraBars.BarShortcut st = null)
        {
            DevExpress.XtraBars.BarButtonItem btItem = new DevExpress.XtraBars.BarButtonItem()
            {
                Caption = caption,
                Name = name,
                Id = MainEditor.GetItemID()
            };
            if (st != null) btItem.ItemShortcut = st;
            btItem.ItemClick += new DevExpress.XtraBars.ItemClickEventHandler(this.ButtonItemClick);

            _editor.BarMgr.Items.Add(btItem);
            parent.LinksPersistInfo.Add(new DevExpress.XtraBars.LinkPersistInfo(btItem, bGroup));
            return btItem;
        }

        protected virtual void ButtonItemClick(object sender, DevExpress.XtraBars.ItemClickEventArgs e) { var str = e.Item.Name; }
    }
}
