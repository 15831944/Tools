using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Text;
using System.Xml.Serialization;
using UCore.Interface;

namespace UCore
{
    [XmlType("Solution")]
    public partial class Solution
    {
        private string _ext = "usl";
        private Server.ServerMgr _sevMgr = new Server.ServerMgr();

        public string PathName { get { return HeadInfo.Path + HeadInfo.Name + "." + _ext; } }

        public IUHeadInfo HeadInfo { get; set; }

        public Server.ServerMgr ServerMgr { get { return _sevMgr; } }
    }
}
