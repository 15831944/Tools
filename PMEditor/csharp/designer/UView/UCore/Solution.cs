using System;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace UCore
{
    public partial class Solution
    {
        public Solution(Interface.IUHeadInfo hi)
        {
            HeadInfo = hi;
        }

        public void Save()
        {
            try
            {
                var xml = UTools.XmlHelper.XmlSerialize(this, Encoding.UTF8);
                UTools.FileHelper.CreatePath(HeadInfo.Path);
                UTools.FileHelper.WriteFileStr(PathName, xml);
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }

        public static Solution LoadSolution(string PathName)
        {
            try
            {
                var xml = UTools.FileHelper.ReadFileStr(PathName);
                Solution sln = UTools.XmlHelper.XmlDeserialize<Solution>(xml, Encoding.UTF8) as Solution;
                return sln;
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
            return null;
        }

        //public static Solution CreateSolution(string name, string path, string comment)
        //{
        //    Solution sln = new Solution()
        //    {
        //        _headInfo.ID = Guid.NewGuid(),
        //        _headInfo.Name = name,
        //        _headInfo.Path = path,
        //        _headInfo.Comment = comment,
        //        _headInfo.Time = DateTime.UtcNow
        //    };
        //    return sln;
        //}

        public void Close()
        {
        }

        public void InitTreeNode(TreeNode node)
        {
            node.Text = $"Solution '{HeadInfo.Name}' ({_sevMgr.ServerList.Count} servers)";
            node.Tag = this;
            node.ToolTipText = HeadInfo.Path;
            foreach (var sev in _sevMgr.ServerList)
            {
                var childNode = new TreeNode();
                node.Nodes.Add(childNode);

                sev.InitTreeNode(childNode);
            }
        }

        internal bool AddServer(Interface.IUServer sev)
        {
            return _sevMgr.AddServer(sev);
            //var proj = _projList.Find(x => x.ProjInfo == phi);
            //if (proj != null) return true;
            //_projList.Add(new Project(phi));
            //return true;
        }

        internal void CreateProj(string strName, string strPath, string strComment)
        {
            //var phi = new ProjectHeadInfo();
            //phi.OnCreate(strName, strPath, strComment);
            //_projHeadList.Add(phi);
            //
            //var proj = new Project(phi);
            //_projList.Add(proj);
        }

        internal bool IfServerExist(string strName)
        {
            return _sevMgr.ServerList.Find(x => (string.Compare(x.HeadInfo.Name, strName, true) == 0)) != null;
            //return _sevMgr.Find(x => (string.Compare(x.Name, strName, true) == 0)) != null;
        }
    }
}
