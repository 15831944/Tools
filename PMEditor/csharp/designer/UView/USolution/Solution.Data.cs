using System;
using System.Collections.Generic;
using System.Xml.Serialization;
using USolution.Interface;

namespace USolution
{
	[XmlRoot("Solution")]
    public partial class Solution
    {
        private string _path;
        private string _name;
        private Guid _id = Guid.Empty;
        private List<IUServer> _sevList = new List<Interface.IUServer>();  // Server list

		[XmlElement("Name")]
        public string Name { get { return _name; } }

        [XmlElement("ID")]
        public Guid ID { get { return _id; } }

        [XmlElement("Server")]
        public List<IUServer> Servers { get { return _sevList; } }
    }
}
