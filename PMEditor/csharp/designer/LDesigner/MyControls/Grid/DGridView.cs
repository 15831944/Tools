using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MyControls.Grid
{
    public partial class DGridView : DataGridView, IComponent
    {
        private ISite _site = null;

        public DGridView()
        {
            _site = new MySite(this);
        }

        //public event EventHandler Disposed;

        [Browsable(false)]
        public ISite Site {
            get { return _site; }
            set { _site = value; }
        }

        //public string Name { get; set; }

        //public void Dispose() { }

    }
}
