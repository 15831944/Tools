using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Text;
using System.Xml.Serialization;
using UCore.Interface;

namespace UCore
{
	[XmlRoot("Solution")]
    public partial class Solution
    {
        private string _path;
        private string _name;
        private string _ext = "usl";
        private Guid _id = Guid.Empty;
        private Server.ServerMgr _sevMgr = new Server.ServerMgr();

        [XmlAttribute("Name")]
        public string Name { get { return _name; } set { _name = value; } }

        public string PathName { get { return _path + _name + "." + _ext; } }

        [XmlAttribute("ID")]
        public Guid ID { get { return _id; } set { _id = value; } }

        //[XmlAttribute("ServerMgr")]
        public Server.ServerMgr ServerMgr { get { return _sevMgr; } set { _sevMgr = value; } }

        public string ToXml()
        {
            var stringBuilder = new StringBuilder();
            using (var writeStream = new StringWriter(stringBuilder, CultureInfo.InvariantCulture))
            {
                var serializer = new XmlSerializer(typeof(Solution));
                serializer.Serialize(writeStream, this);
            }
            return stringBuilder.ToString();
        }

        public Solution FromXml(string xml)
        {
            Solution sln = null;
            if (!string.IsNullOrEmpty(xml))
            {
                try
                {
                    var stringBuilder = new StringBuilder();
                    using (var readStream = new StringReader(xml))
                    {
                        var serializer = new XmlSerializer(typeof(Solution));
                        sln = (Solution)serializer.Deserialize(readStream);
                    }
                }catch (Exception) { return null; }
            }
            return sln;
        }
    }
}
