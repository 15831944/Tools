using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Xml;
using System.Xml.Serialization;
using UCore.Interface;

namespace UCore.Server
{
    public abstract class UServerBase : IUServer
    {
        private List<IUGroup> _groups = new List<IUGroup>();
        private List<IUChild> _children = new List<IUChild>();
        private string _ext = "sev";

        public List<IUGroup> Groups { get { return _groups; } }

        public List<IUChild> Children { get { return _children; } }

        public IUHeadInfo HeadInfo { get; set; }

        public virtual void InitTreeNode(TreeNode node)
        {
            node.Text = HeadInfo.Name;
            node.Tag = this;
            foreach (var group in _groups)
            {
                var childNode = new TreeNode();
                node.Nodes.Add(childNode);

                group.InitTreeNode(childNode);
            };
        }

        public virtual void Save()
        {
            var xml = ToXml();
            UTools.FileHelper.WriteFileStr(HeadInfo.Path + HeadInfo.Name + "." + _ext, xml);

            foreach (var child in _children)
                child.Save();
        }

        internal string ToXml()
        {
            XmlDocument doc = new XmlDocument();
            XmlDeclaration dec = doc.CreateXmlDeclaration("1.0", "UTF-8", null);
            doc.AppendChild(dec);

            XmlElement root = doc.CreateElement("Server");
            doc.AppendChild(root);

            root.SetAttribute("type", typeof(UServerBase).ToString());
            root.SetAttribute("name", HeadInfo.Name);
            root.SetAttribute("id", HeadInfo.ID.ToString());
            root.SetAttribute("comment", HeadInfo.Comment);
            root.SetAttribute("time", HeadInfo.Time.ToString());

            return doc.InnerXml;
        }
    }
}
