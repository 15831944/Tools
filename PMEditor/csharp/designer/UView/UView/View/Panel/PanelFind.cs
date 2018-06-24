
namespace UView.View.Panel
{
    internal class PanelFind : PanelControl
    {
        private MainEditor _editor;
        private System.Windows.Forms.ListBox _listFind = new System.Windows.Forms.ListBox();

        internal PanelFind()
        { }

        internal override void InitializeComponent(MainEditor editor)
        {
            _editor = editor;

            // _listFind
            this._listFind.BackColor = System.Drawing.SystemColors.Window;
            this._listFind.Dock = System.Windows.Forms.DockStyle.Fill;
            this._listFind.Location = new System.Drawing.Point(0, 0);
            this._listFind.Name = "_listFind";
            this._listFind.Size = new System.Drawing.Size(799, 82);
            this._listFind.TabIndex = 4;
            this._listFind.Text = "";
            InitializeCtrl(this._listFind, "Find", $"Find", DevExpress.XtraBars.Docking.DockingStyle.Bottom);
        }

        public System.Windows.Forms.ListBox ListFind { get { return _listFind; } }
    }
}
