using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing.Design;

namespace MyControls.Grid
{
    public partial class DGridView : DataGridView, IComponent
    {
        private ISite _site = null;

        public DGridView()
        {
            _site = new DGridViewSite(this);
            //_site = new DataGridViewComponentPropertyGridSite(this, this);
        }

        //public event EventHandler Disposed;

        [Browsable(false)]
        public ISite Site {
            get { return _site; }
            set { _site = value; }
        }

        //
        // Summary:
        //     Gets a collection that contains all the columns in the control.
        //
        // Returns:
        //     The System.Windows.Forms.DataGridViewColumnCollection that contains all the columns
        //     in the System.Windows.Forms.DataGridView control.
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Content)]
        [Editor(typeof(DataGridViewColumnCollectionEditor), typeof(UITypeEditor))]
        [MergableProperty(false)]
        public DataGridViewColumnCollection Columns { get; }
    }
}
