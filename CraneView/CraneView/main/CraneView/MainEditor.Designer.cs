namespace CraneView
{
	partial class MainEditor
	{
		/// <summary>
		/// 必需的设计器变量。
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		/// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows 窗体设计器生成的代码

		/// <summary>
		/// 设计器支持所需的方法 - 不要
		/// 使用代码编辑器修改此方法的内容。
		/// </summary>
		private void InitializeComponent()
		{
            this.components = new System.ComponentModel.Container();
            this._barMgr = new DevExpress.XtraBars.BarManager(this.components);
            this._menuBar = new DevExpress.XtraBars.Bar();
            this._statusBar = new DevExpress.XtraBars.Bar();
            this.barDockControlTop = new DevExpress.XtraBars.BarDockControl();
            this.barDockControlBottom = new DevExpress.XtraBars.BarDockControl();
            this.barDockControlLeft = new DevExpress.XtraBars.BarDockControl();
            this.barDockControlRight = new DevExpress.XtraBars.BarDockControl();
            this._dockMgr = new DevExpress.XtraBars.Docking.DockManager(this.components);
            ((System.ComponentModel.ISupportInitialize)(this._barMgr)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this._dockMgr)).BeginInit();
            this.SuspendLayout();
            // 
            // _barMgr
            // 
            this._barMgr.AllowQuickCustomization = false;
            this._barMgr.Bars.AddRange(new DevExpress.XtraBars.Bar[] {
            this._menuBar,
            this._statusBar});
            this._barMgr.DockControls.Add(this.barDockControlTop);
            this._barMgr.DockControls.Add(this.barDockControlBottom);
            this._barMgr.DockControls.Add(this.barDockControlLeft);
            this._barMgr.DockControls.Add(this.barDockControlRight);
            this._barMgr.Form = this;
            this._barMgr.MainMenu = this._menuBar;
            this._barMgr.MaxItemId = 272;
            this._barMgr.StatusBar = this._statusBar;
            // 
            // _menuBar
            // 
            this._menuBar.BarName = "MenuBar";
            this._menuBar.DockCol = 0;
            this._menuBar.DockRow = 0;
            this._menuBar.DockStyle = DevExpress.XtraBars.BarDockStyle.Top;
            this._menuBar.OptionsBar.AllowQuickCustomization = false;
            this._menuBar.OptionsBar.DrawDragBorder = false;
            this._menuBar.OptionsBar.MultiLine = true;
            this._menuBar.OptionsBar.UseWholeRow = true;
            this._menuBar.Text = "MenuBar";
            // 
            // _statusBar
            // 
            this._statusBar.BarName = "StatusBar";
            this._statusBar.CanDockStyle = DevExpress.XtraBars.BarCanDockStyle.Bottom;
            this._statusBar.DockCol = 0;
            this._statusBar.DockRow = 0;
            this._statusBar.DockStyle = DevExpress.XtraBars.BarDockStyle.Bottom;
            this._statusBar.OptionsBar.AllowQuickCustomization = false;
            this._statusBar.OptionsBar.DisableClose = true;
            this._statusBar.OptionsBar.DisableCustomization = true;
            this._statusBar.OptionsBar.DrawDragBorder = false;
            this._statusBar.OptionsBar.UseWholeRow = true;
            this._statusBar.Text = "StatusBar";
            // 
            // barDockControlTop
            // 
            this.barDockControlTop.Dock = System.Windows.Forms.DockStyle.Top;
            this.barDockControlTop.Location = new System.Drawing.Point(0, 0);
            this.barDockControlTop.Size = new System.Drawing.Size(1008, 0);
            // 
            // barDockControlBottom
            // 
            this.barDockControlBottom.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.barDockControlBottom.Location = new System.Drawing.Point(0, 834);
            this.barDockControlBottom.Size = new System.Drawing.Size(1008, 0);
            // 
            // barDockControlLeft
            // 
            this.barDockControlLeft.Dock = System.Windows.Forms.DockStyle.Left;
            this.barDockControlLeft.Location = new System.Drawing.Point(0, 0);
            this.barDockControlLeft.Size = new System.Drawing.Size(0, 834);
            // 
            // barDockControlRight
            // 
            this.barDockControlRight.Dock = System.Windows.Forms.DockStyle.Right;
            this.barDockControlRight.Location = new System.Drawing.Point(1008, 0);
            this.barDockControlRight.Size = new System.Drawing.Size(0, 834);
            // 
            // _dockMgr
            // 
            this._dockMgr.Form = this;
            this._dockMgr.TopZIndexControls.AddRange(new string[] {
            "DevExpress.XtraBars.BarDockControl",
            "DevExpress.XtraBars.StandaloneBarDockControl",
            "System.Windows.Forms.StatusBar",
            "DevExpress.XtraBars.Ribbon.RibbonStatusBar",
            "DevExpress.XtraBars.Ribbon.RibbonControl"});
            // 
            // MainEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1008, 834);
            this.Controls.Add(this.barDockControlLeft);
            this.Controls.Add(this.barDockControlRight);
            this.Controls.Add(this.barDockControlBottom);
            this.Controls.Add(this.barDockControlTop);
            this.DoubleBuffered = true;
            this.IsMdiContainer = true;
            this.LookAndFeel.SkinName = "Black";
            this.Name = "MainEditor";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "CraneView";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.MainEditor_FormClosed);
            this.Load += new System.EventHandler(this.MainEditor_Load);
            ((System.ComponentModel.ISupportInitialize)(this._barMgr)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this._dockMgr)).EndInit();
            this.ResumeLayout(false);

		}

		#endregion

		private DevExpress.XtraBars.BarManager _barMgr;
		private DevExpress.XtraBars.Bar _menuBar;
		private DevExpress.XtraBars.Bar _statusBar;
		private DevExpress.XtraBars.BarDockControl barDockControlTop;
		private DevExpress.XtraBars.BarDockControl barDockControlBottom;
		private DevExpress.XtraBars.BarDockControl barDockControlLeft;
		private DevExpress.XtraBars.BarDockControl barDockControlRight;
		private DevExpress.XtraBars.Docking.DockManager _dockMgr;
	}
}

