using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Xml.Serialization;
using UCore.Interface;

namespace UCore.Server
{
    public abstract class UServerBase : IUServer
    {
        private List<IUGroup> _groups = new List<IUGroup>();

        public virtual void InitTreeNode(TreeNode node)
        {
            node.Text = HeadInfo.Name;
            node.Tag = this;
            foreach (var group in _groups)
            {
                var childNode = new TreeNode();
                node.Nodes.Add(childNode);

                group.InitTreeNode(childNode);
            };
        }

        [XmlElement("Groups")]
        public List<IUGroup> Groups { get { return _groups; } }

        public IUHeadInfo HeadInfo { get; set; }
    }
}
