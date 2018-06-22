
namespace UView.View.Panel
{
    internal class PanelOutput : PanelControl
    {
        private MainEditor _editor;
        private System.Windows.Forms.RichTextBox _output = new System.Windows.Forms.RichTextBox();

        internal PanelOutput()
        { }

        internal override void InitializeComponent(MainEditor editor)
        {
            _editor = editor;

            // _output
            this._output.BackColor = System.Drawing.SystemColors.Window;
            this._output.Dock = System.Windows.Forms.DockStyle.Fill;
            this._output.Location = new System.Drawing.Point(0, 0);
            this._output.Name = "_output";
            this._output.ReadOnly = true;
            this._output.Size = new System.Drawing.Size(799, 82);
            this._output.TabIndex = 3;
            this._output.Text = "";
            InitializeCtrl(this._output, "Output", DevExpress.XtraBars.Docking.DockingStyle.Bottom);
        }

        public void PrintOutput(string strOutput)
        {
            _output.Text = _output.Text + strOutput + "\r\n";
            DockPanel.Show();
        }

        public System.Windows.Forms.RichTextBox Output { get { return _output; } }
    }
}
