using System;
using System.Collections.Generic;
using System.Xml;
using System.Xml.Serialization;
using UCore.Interface;

namespace UCore.Server
{
    public class ServerMgr
    {
        private List<IUServer> _sevList = new List<IUServer>();  // Server list

        internal ServerMgr()
        { }

        public List<IUServer> ServerList { get { return _sevList; } }

        internal bool AddServer(Interface.IUServer sev)
        {
            //var sev
            return true;
        }

        internal void Save()
        {
            foreach (var sev in ServerList) sev.Save();
        }

        internal void ToXml(XmlDocument doc, XmlElement root)
        {
            foreach (var sev in ServerList)
            {
                var ele = doc.CreateElement("Server");
                ele.SetAttribute("name", sev.HeadInfo.Name);
                ele.SetAttribute("id", sev.HeadInfo.ID.ToString());
                ele.SetAttribute("path", sev.HeadInfo.Path);
            }
        }
    }
}
