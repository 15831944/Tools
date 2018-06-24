using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Text;
using System.Xml;
using System.Xml.Serialization;
using UCore.Interface;

namespace UCore
{
    [XmlType("Solution")]
    public partial class Solution
    {
        private static string _ext = "usl";
        private Server.ServerMgr _sevMgr = new Server.ServerMgr();

        public string PathName { get { return HeadInfo.Path + HeadInfo.Name + "." + _ext; } }

        public IUHeadInfo HeadInfo { get; set; }

        public Server.ServerMgr ServerMgr { get { return _sevMgr; } }

        public string ToXml()
        {
            XmlDocument doc = new XmlDocument();
            XmlDeclaration dec = doc.CreateXmlDeclaration("1.0", "UTF-8", null);
            doc.AppendChild(dec);

            XmlElement root = doc.CreateElement("Solution");
            doc.AppendChild(root);

            root.SetAttribute("type", typeof(Solution).ToString());
            root.SetAttribute("name", HeadInfo.Name);
            root.SetAttribute("id", HeadInfo.ID.ToString());
            root.SetAttribute("comment", HeadInfo.Comment);
            root.SetAttribute("time", HeadInfo.Time.ToString());

            _sevMgr.ToXml(doc, root);

            return doc.InnerXml;
        }
    }
}
