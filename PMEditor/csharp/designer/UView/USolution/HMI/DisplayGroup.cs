using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;
using USolution.Interface;

namespace USolution.HMI
{
    internal class DisplayGroup : Interface.IUGroup
    {
        private string _name;
        private Guid _id;
        private IUGroup _parent;
        private List<IUGroup> _groups = new List<IUGroup>();
        private List<IUChild> _displays = new List<IUChild>();

        public DisplayGroup(string name)
        {
            _name = name;
            _id = Guid.NewGuid();
        }

        public static IUGroup LoadGroup()
        {
            DisplayGroup group;
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

        [XmlElement("Name")]
        public string Name { get { return _name; } }
        [XmlElement("ID")]
        public Guid ID { get { return _id; } }
        [XmlElement("Parent")]
        public IUGroup Parent { get => _parent; set { if (_parent != value) _parent?.RemoveGroup(this as Interface.IUGroup); _parent = value; } }
        [XmlElement("Groups")]
        public List<IUGroup> Groups { get => _groups; }
        [XmlElement("Children")]
        public List<IUChild> Children { get => _displays; }
    }
}
