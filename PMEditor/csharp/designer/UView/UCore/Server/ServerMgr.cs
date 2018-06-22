using System;
using System.Collections.Generic;
using System.Xml.Serialization;
using UCore.Interface;

namespace UCore.Server
{
    public class ServerMgr
    {
        private List<IUServer> _sevList = new List<IUServer>();  // Server list

        internal ServerMgr()
        { }

        [XmlElement("ServerList")]
        public List<IUServer> ServerList { get { return _sevList; } }

        internal bool AddServer(Interface.IUServer sev)
        {
            //var sev
            return true;
        }
    }
}
