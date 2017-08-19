using System;
using System.Collections.Generic;
using System.Text;

namespace CraneView.View.Panel
{
	internal class PanelOutput : PanelControl
	{
		private MainEditor _editor;
		private System.Windows.Forms.RichTextBox _output = new System.Windows.Forms.RichTextBox();

		internal PanelOutput()
		{ }

		internal void InitializeComponent(MainEditor editor)
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
	}
}
