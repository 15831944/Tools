using System;
using System.Windows.Forms;

namespace UCore
{
    public partial class Solution
    {
        private Solution()
        {
            _id = Guid.NewGuid();
        }

        public void Save()
        {
            IO.FileXmlIO.Save2File(PathName, ToXml());
        }

        public void Close()
        {
        }

        public void InitTreeNode(TreeNode node)
        {
            node.Text = $"Solution '{_name}' ({_sevMgr.ServerList.Count} servers)";
            node.Tag = this;
            node.ToolTipText = _path;
            foreach (var sev in _sevMgr.ServerList)
            {
                var childNode = new TreeNode();
                node.Nodes.Add(childNode);

                sev.InitTreeNode(childNode);
            }
        }
    }
}
