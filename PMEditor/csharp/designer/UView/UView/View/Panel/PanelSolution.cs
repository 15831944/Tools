
namespace UView.View.Panel
{
    internal class PanelSolution : PanelControl
    {
        private MainEditor _editor;
        private SolutionTree _tree = new SolutionTree();

        internal PanelSolution()
        { }

        internal override void InitializeComponent(MainEditor editor)
        {
            _editor = editor;

            // _tree
            this._tree.AllowDrop = true;
            this._tree.Dock = System.Windows.Forms.DockStyle.Fill;
            this._tree.HideSelection = false;
            this._tree.LineColor = System.Drawing.Color.Empty;
            this._tree.Location = new System.Drawing.Point(0, 0);
            this._tree.Name = "_tree";
            this._tree.Size = new System.Drawing.Size(228, 277);
            this._tree.TabIndex = 1;
            InitializeCtrl(this._tree, "Solution", $"Solution", DevExpress.XtraBars.Docking.DockingStyle.Left);
        }

        public SolutionTree SlnTree { get { return _tree; } }
    }
}
