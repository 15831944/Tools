namespace LDesigner
{
    partial class PropertyCtrl
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

        #region 组件设计器生成的代码

        /// <summary> 
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this._cbObjectList = new System.Windows.Forms.ComboBox();
            this._propGrid = new System.Windows.Forms.PropertyGrid();
            this.SuspendLayout();
            // 
            // _cbObjectList
            // 
            this._cbObjectList.Dock = System.Windows.Forms.DockStyle.Top;
            this._cbObjectList.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this._cbObjectList.FormattingEnabled = true;
            this._cbObjectList.Location = new System.Drawing.Point(0, 0);
            this._cbObjectList.Name = "_cbObjectList";
            this._cbObjectList.Size = new System.Drawing.Size(149, 20);
            this._cbObjectList.Sorted = true;
            this._cbObjectList.TabIndex = 0;
            // 
            // _propGrid
            // 
            this._propGrid.Dock = System.Windows.Forms.DockStyle.Fill;
            this._propGrid.Location = new System.Drawing.Point(0, 20);
            this._propGrid.Name = "_propGrid";
            this._propGrid.Size = new System.Drawing.Size(149, 179);
            this._propGrid.TabIndex = 1;
            // 
            // PropertyCtrl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this._propGrid);
            this.Controls.Add(this._cbObjectList);
            this.Name = "PropertyCtrl";
            this.Size = new System.Drawing.Size(149, 199);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ComboBox _cbObjectList;
        private System.Windows.Forms.PropertyGrid _propGrid;
    }
}
