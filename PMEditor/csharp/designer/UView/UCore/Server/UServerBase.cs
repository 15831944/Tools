using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Xml.Serialization;
using UCore.Interface;

namespace UCore.Server
{
    public abstract class UServerBase : IUServer
    {
        private Guid _id = Guid.NewGuid();
        private string _name;
        private List<IUGroup> _groups = new List<IUGroup>();

        public virtual void InitTreeNode(TreeNode node)
        {
            node.Text = _name;
            node.Tag = this;
            foreach (var group in _groups)
            {
                var childNode = new TreeNode();
                node.Nodes.Add(childNode);

                group.InitTreeNode(childNode);
            };
        }

        [XmlElement("Name")]
        public string Name { get { return _name; } }
        [XmlElement("ID")]
        public Guid ID { get { return _id; } }
        [XmlElement("Groups")]
        public List<IUGroup> Groups { get { return _groups; } }
    }
}
