using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using USolution.Interface;

namespace USolution
{
    public partial class Solution
    {
        private Solution()
        {
            _id = Guid.NewGuid();
        }

        public void Save()
        {
        }

        public void Close()
        {
        }

        public void InitTreeNode(System.Windows.Forms.TreeNode node)
        {
            node.Text += $" ({_name})";
            node.Tag = this;
            node.ToolTipText = _path;
            for (var sev in _sevList)
            {

                node.Nodes.Add
}
        }
    }
}
