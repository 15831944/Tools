using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;
using USolution.Interface;

namespace USolution.HMI
{
    internal class Display : Interface.IUChild
    {
        private string _name;
        private Guid _id;
        private IUGroup _parent;

        [XmlElement("Name")]
        public string Name { get { return _name; } }
        [XmlElement("ID")]
        public Guid ID { get { return _id; } }
        [XmlElement("Parent")]
        public IUGroup Parent { get => _parent; set => _parent = value; }
    }
}
