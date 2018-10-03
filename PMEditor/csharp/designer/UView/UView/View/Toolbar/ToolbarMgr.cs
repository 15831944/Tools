using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UView.View.Toolbar
{
    class ToolbarMgr
    {
        private MainEditor _editor;
        private List<IToolbar> _toolbarList = new List<IToolbar>();

        internal void InitToolbar(MainEditor editor)
        {
            _editor = editor;
            _toolbarList.Add(new ToolbarStandard());

            foreach (var bar in _toolbarList)
                bar.InitToolbar(editor);
        }
    }
}
