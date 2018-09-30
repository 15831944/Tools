using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UView.View.Menu
{
    internal class MenuMgr
    {
        private MainEditor _editor;
        private List<IMenuGroup> _menuGroupList = new List<IMenuGroup>();

        internal void InitMainMenu(MainEditor editor)//DevExpress.XtraBars.BarManager barMgr, DevExpress.XtraBars.Bar menuBar)
        {
            _editor = editor;
            _menuGroupList.Add(new MenuGroupFile());

            foreach (var group in _menuGroupList)
                group.InitMenu(editor);
        }
    }
}
