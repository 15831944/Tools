﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;

namespace UCore
{
    [XmlType("HeadInformation")]
    public class HeadInfo : Interface.IUHeadInfo
    {
        [XmlAttribute("id")]
        public Guid ID { get; set; }
        [XmlAttribute("name")]
        public string Name { get; set; }
        [XmlAttribute("path")]
        public string Path { get; set; }
        [XmlAttribute("comment")]
        public string Comment { get; set; }
        [XmlAttribute("time")]
        public DateTime Time { get; set; }

        internal HeadInfo() : this("", "", "") {}

        internal HeadInfo(string name, string path, string comment)
        {
            Name = name;
            Path = path;
            Comment = comment;
            Time = DateTime.UtcNow;
            ID = Guid.NewGuid();
        }
    }
}
