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
    public abstract class UGroupBase : IUGroup
    {
        private string _name;
        private Guid _id;
        private IUGroup _parent;
        private List<IUGroup> _groups = new List<IUGroup>();
        private List<IUChild> _children = new List<IUChild>();

        public UGroupBase(string name)
        {
            _name = name;
            _id = Guid.NewGuid();
        }

        public static IUGroup LoadGroup()
        {
            UGroupBase group;
            return null;
        }

        public bool AddChildGroup(Interface.IUGroup group)
        {
            if (IsNameExist(group.Name)) return false;
            group.Parent = this;
            _groups.Add(group);
            return true;
        }

        public void RemoveGroup(Interface.IUGroup group)
        {
            _groups.Remove(group);
        }

        public bool IsNameExist(string name)
        {
            foreach (var g in _groups)
                if (g.Name.Equals(name, StringComparison.OrdinalIgnoreCase))
                    return true;
            return false;
        }

        public virtual void InitTreeNode(TreeNode node)
        {
            node.Text = _name;
            node.Tag = this;

            var listG = _groups;
            listG.Sort((l, r) => l.Name.CompareTo(r.Name));
            foreach (var group in listG)
            {
                var childNode = new TreeNode();
                node.Nodes.Add(childNode);

                group.InitTreeNode(childNode);
            }

            var listC = _children;
            listC.Sort((l, r) => l.Name.CompareTo(r.Name));
            foreach (var child in listC)
            {
                var childNode = new TreeNode();
                node.Nodes.Add(childNode);

                child.InitTreeNode(childNode);
            }
        }

        [XmlElement("Name")]
        public string Name { get { return _name; } }
        [XmlElement("ID")]
        public Guid ID { get { return _id; } }
        [XmlElement("Parent")]
        public IUGroup Parent { get { return _parent; } set { if (_parent != value) _parent?.RemoveGroup(this as Interface.IUGroup); _parent = value; } }
        [XmlElement("Groups")]
        public List<IUGroup> Groups { get { return _groups; } }
        [XmlElement("Children")]
        public List<IUChild> Children { get { return _children; } }
    }
}
