namespace UView
{
    partial class CreateSolutionDialog
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.lbName = new System.Windows.Forms.Label();
            this.btBrowsePath = new System.Windows.Forms.Button();
            this.txtName = new System.Windows.Forms.TextBox();
            this.lbPath = new System.Windows.Forms.Label();
            this.txtPath = new System.Windows.Forms.TextBox();
            this.btOK = new System.Windows.Forms.Button();
            this.btCancel = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // lbName
            // 
            this.lbName.AutoSize = true;
            this.lbName.Location = new System.Drawing.Point(12, 9);
            this.lbName.Name = "lbName";
            this.lbName.Size = new System.Drawing.Size(29, 12);
            this.lbName.TabIndex = 0;
            this.lbName.Text = "Name";
            // 
            // btBrowsePath
            // 
            this.btBrowsePath.Location = new System.Drawing.Point(478, 33);
            this.btBrowsePath.Name = "btBrowsePath";
            this.btBrowsePath.Size = new System.Drawing.Size(41, 21);
            this.btBrowsePath.TabIndex = 2;
            this.btBrowsePath.Text = "...";
            this.btBrowsePath.UseVisualStyleBackColor = true;
            this.btBrowsePath.Click += new System.EventHandler(this.btBrowsePath_Click);
            // 
            // txtName
            // 
            this.txtName.Location = new System.Drawing.Point(57, 6);
            this.txtName.Name = "txtName";
            this.txtName.Size = new System.Drawing.Size(415, 21);
            this.txtName.TabIndex = 0;
            // 
            // lbPath
            // 
            this.lbPath.AutoSize = true;
            this.lbPath.Location = new System.Drawing.Point(12, 36);
            this.lbPath.Name = "lbPath";
            this.lbPath.Size = new System.Drawing.Size(29, 12);
            this.lbPath.TabIndex = 0;
            this.lbPath.Text = "Path";
            // 
            // txtPath
            // 
            this.txtPath.Location = new System.Drawing.Point(57, 33);
            this.txtPath.Name = "txtPath";
            this.txtPath.Size = new System.Drawing.Size(415, 21);
            this.txtPath.TabIndex = 1;
            // 
            // btOK
            // 
            this.btOK.Location = new System.Drawing.Point(373, 64);
            this.btOK.Name = "btOK";
            this.btOK.Size = new System.Drawing.Size(70, 21);
            this.btOK.TabIndex = 3;
            this.btOK.Text = "OK";
            this.btOK.UseVisualStyleBackColor = true;
            this.btOK.Click += new System.EventHandler(this.btOK_Click);
            // 
            // btCancel
            // 
            this.btCancel.Location = new System.Drawing.Point(449, 64);
            this.btCancel.Name = "btCancel";
            this.btCancel.Size = new System.Drawing.Size(70, 21);
            this.btCancel.TabIndex = 4;
            this.btCancel.Text = "Cancel";
            this.btCancel.UseVisualStyleBackColor = true;
            this.btCancel.Click += new System.EventHandler(this.btCancel_Click);
            // 
            // CreateSolutionDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(526, 97);
            this.Controls.Add(this.txtPath);
            this.Controls.Add(this.txtName);
            this.Controls.Add(this.lbPath);
            this.Controls.Add(this.btCancel);
            this.Controls.Add(this.btOK);
            this.Controls.Add(this.btBrowsePath);
            this.Controls.Add(this.lbName);
            this.Name = "CreateSolutionDialog";
            this.Text = "CreateSolution";
            this.Load += new System.EventHandler(this.CreateSolutionDialog_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lbName;
        private System.Windows.Forms.Button btBrowsePath;
        private System.Windows.Forms.TextBox txtName;
        private System.Windows.Forms.Label lbPath;
        private System.Windows.Forms.TextBox txtPath;
        private System.Windows.Forms.Button btOK;
        private System.Windows.Forms.Button btCancel;
    }
}