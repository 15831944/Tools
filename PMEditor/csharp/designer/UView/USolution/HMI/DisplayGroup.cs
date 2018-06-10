using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace USolution.HMI
{
    internal class DisplayGroup : Interface.IGroup
    {
        private string _name;
        private Guid _id;
        private Interface.IGroup _parent;
        private List<Interface.IGroup> _children = new List<Interface.IGroup>();
        private List<Display> _displays = new List<Display>();

        public DisplayGroup(string name)
        {
            _name = name;
            _id = Guid.NewGuid();
        }

        public static Interface.IGroup LoadGroup()
        {
            DisplayGroup group;
            return null;
        }

        public bool AddChildGroup(Interface.IGroup group)
        {
            if (IsNameExist(group.Name)) return false;
            group.Parent = this;
            _children.Add(group);
            return true;
        }

        public void RemoveGroup(Interface.IGroup group)
        {
            _children.Remove(group);
        }

        public bool IsNameExist(string name)
        {
            foreach (var g in _children)
                if (g.Name.Equals(name, StringComparison.OrdinalIgnoreCase))
                    return true;
            return false;
        }

        public string Name { get { return _name; } }
        public Guid ID { get { return _id; } }
        public Interface.IGroup Parent { get { return _parent; } set { if (_parent != value) _parent?.RemoveGroup(this as Interface.IGroup); _parent = value; } }
    }
}
