using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;

namespace UView.View.Toolbar
{
    class ToolbarBase : IToolbar
    {
        protected MainEditor _editor;
        protected DevExpress.XtraBars.Bar _itemGroup;
        protected List<DevExpress.XtraBars.BarButtonItem> _itemList = new List<DevExpress.XtraBars.BarButtonItem>();

        void IToolbar.InitToolbar(MainEditor editor)
        {
            _editor = editor;
            InitializeToolbar();
        }

        protected virtual void InitializeToolbar()
        {
            if (_itemGroup != null)
                _editor.BarMgr.Bars.Add(_itemGroup);
        }

        protected DevExpress.XtraBars.BarButtonItem InitToolButton(string caption, string name, Image image, bool bGroup = false, DevExpress.XtraBars.BarShortcut st = null)
        {
            DevExpress.XtraBars.BarButtonItem item = new DevExpress.XtraBars.BarButtonItem()
            {
                Name = name,
                Caption = caption,
                //Hint = "新建",  // exception because font is wrong, i don't know why yet
                Id = MainEditor.GetItemID()
            };
            if (image != null) item.Glyph = image;
            if (st != null) item.ItemShortcut = st;
            item.ItemClick += new DevExpress.XtraBars.ItemClickEventHandler(this.ButtonItemClick);

            _editor.BarMgr.Items.Add(item);
            _itemGroup.LinksPersistInfo.Add(new DevExpress.XtraBars.LinkPersistInfo(item, bGroup));
            _itemList.Add(item);
            return item;
        }

        private void ButtonItemClick(object sender, EventArgs e)
        {
            string var = e.ToString();
        }
    }
}
