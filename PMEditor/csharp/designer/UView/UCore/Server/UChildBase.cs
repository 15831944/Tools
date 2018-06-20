using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml.Serialization;
using UCore.Interface;

namespace UCore.Server
{
    public abstract class UChildBase : IUChild
    {
        private string _name = "";
        private Guid _id = Guid.NewGuid();
        private IUGroup _parent;

        [XmlElement("Name")]
        public string Name { get { return _name; } }
        [XmlElement("ID")]
        public Guid ID { get { return _id; } }
        [XmlElement("Parent")]
        public IUGroup Parent { get { return _parent; } set { _parent = value; } }

        public virtual void InitTreeNode(TreeNode node)
        {
            node.Text = _name;
            node.Tag = this;
        }
    }
}
