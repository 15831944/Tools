using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace UView.View
{
    public partial class SolutionTree : UTools.TreeCtrl
    {
        private UCore.SolutionMgr _slnMgr;
        public SolutionTree()
        {
            InitializeComponent();
        }

        internal void InitSolutionTree(UCore.SolutionMgr slnMgr)
        {
            _slnMgr = slnMgr;
            var sln = slnMgr.FirstSolution;

            // Projects
            //TreeNode root = AddNode(slnMgr, "Projects", null);
            TreeNode root = AddNode(sln, $"Solution ({sln.HeadInfo.Name})", null);
            foreach (var sev in sln.ServerMgr.ServerList)
            {
                //TreeNode projNode = AddNode(proj, proj.Name, root);
                //TreeNode displayMgrNode = AddNode(proj.DisplayMgr, "Displays", projNode);
                //displayMgrNode.ContextMenuStrip = _menuProj;

                foreach (var group in sev.Groups)
                {
                    if (group.Parent == null)
                    {
                        TreeNode groupNode = AddNode(group, group.HeadInfo.Name, root);
                        groupNode.ContextMenuStrip = _menuGroup;
                        InitGroupTree(group, groupNode);
                    }
                }
                //foreach (var display in proj.DisplayMgr.DisplayList)
                //{
                //    if (display.Group == null)
                //    {
                //        TreeNode dispNode = AddNode(display, display.Name, displayMgrNode);
                //        dispNode.ContextMenuStrip = _menuDisplay;
                //    }
                //    else
                //    {
                //        TreeNode dispNode = AddNode(display, display.Name, display.Group);
                //        dispNode.ContextMenuStrip = _menuDisplay;
                //    }
                //}
            }
            root.ExpandAll();
        }

        private void InitGroupTree(UCore.Interface.IUGroup group, TreeNode groupNode)
        {
            if (group.Groups.Count <= 0) return;
            foreach (var subGroup in group.Groups)
            {
                TreeNode subGroupNode = AddNode(subGroup, subGroup.HeadInfo.Name, groupNode);
                InitGroupTree(subGroup, subGroupNode);
            }
        }

        private void OnAddGroup(object sender, EventArgs e)
        {
            //CraneView.View.AddComponentForm form = new CraneView.View.AddComponentForm();
            //if (form.ShowDialog() == DialogResult.OK)
            //{
            //    string name = form.NewName;
            //    string err = "";
            //    var group = _projMgr.FirstProject.DisplayMgr.AddGroup(name, null, ref err);
            //    if (group == null)
            //    {
            //        MessageBox.Show("Add group failed!\r\n" + err, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            //    }
            //}
        }

        private void OnAddDisplay(object sender, EventArgs e)
        {
            //CraneView.View.AddComponentForm form = new CraneView.View.AddComponentForm();
            //if (form.ShowDialog() == DialogResult.OK)
            //{
            //    string name = form.NewName;
            //    string err = "";
            //    var display = _projMgr.FirstProject.DisplayMgr.AddDisplay(name, null, ref err);
            //    if (display == null)
            //    {
            //        MessageBox.Show("Add display failed!\r\n" + err, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            //    }
            //}
        }
        private void OnAddSubGroup(object sender, EventArgs e) { }
        private void OnRenameGroup(object sender, EventArgs e) { }
        private void OnDeleteGroup(object sender, EventArgs e) { }
        private void OnAddSubDisplay(object sender, EventArgs e) { }
        private void OnRenameDisplay(object sender, EventArgs e) { }
        private void OnDeleteDisplay(object sender, EventArgs e) { }
    }
}
