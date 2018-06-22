using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UCore
{
    public class SolutionMgr
    {
        internal List<HeadInfo> _solutionHeadList = new List<HeadInfo>();
        internal List<Solution> _solutionList = new List<Solution>(); // only support first onenow

        public SolutionMgr()
        { }

        public void SaveSolutionHeadInfo()
        {
            try
            {
                string xml = UTools.XmlHelper.XmlSerialize(_solutionHeadList, Encoding.UTF8);
                UTools.FileHelper.WriteFileStr("ServerHeak.slh", xml);
                //byte[] data;
                //EncryptHelper.EncryptResult res = EncryptHelper.Encrypt(xml, "ProjectHeadInfo", out data);
                //
                //FileStream fs = new FileStream("ProjHead.pjh", FileMode.Create);
                //fs.Write(data, 0, data.Length);
                //fs.Close();
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }

        public void LoadSolutionHeadInfo()
        {
            try
            {
                //FileStream fs = new FileStream("ProjHead.pjh", FileMode.Open, FileAccess.Read);
                //byte[] data = new byte[(int)fs.Length];
                //fs.Read(data, 0, (int)fs.Length);
                //fs.Close();
                string xmlStr = UTools.FileHelper.ReadFileStr("ServerHead.slh");

                //EncryptHelper.EncryptResult res = EncryptHelper.Decrypt(data, "ProjectHeadInfo", ref xmlStr);
                //if (res != EncryptHelper.EncryptResult.Good)
                //{
                //    return;
                //}

                _solutionHeadList.Clear();
                _solutionHeadList = UTools.XmlHelper.XmlDeserialize<List<HeadInfo>>(xmlStr, Encoding.UTF8);
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }

        public bool LoadSolution(HeadInfo hi)
        {
            var sln = _solutionList.Find(x => x.HeadInfo == hi);
            if (sln != null) return true;
            _solutionList.Add(new Solution(hi));
            return true;
        }

        public void CreateSolution(string strName, string strPath, string strComment)
        {
            var hi = new HeadInfo() {
                Name = strName,
                Path = strPath,
                Comment = strComment
            };
            _solutionHeadList.Add(hi);

            var sln = new Solution(hi);
            _solutionList.Add(sln);
        }

        public bool IfSolutionExist(string strName)
        {
            return _solutionList.Find(x => (string.Compare(x.HeadInfo.Name, strName, true) == 0)) != null;
        }

        public Solution FirstSolution { get { return _solutionList.Count > 0 ? _solutionList[0] : null; } }

        //public TreeNode InitProjTree(View.ProjectTree treeCtrl)
        //{
        //	TreeNode node = treeCtrl.AddNode(this, "Projects", null);
        //	foreach(var proj in _solutionList)
        //	{
        //		proj.InitProjTree(treeCtrl, node);
        //	}
        //	node.ExpandAll();
        //	return node;
        //}

        //public MainEditor Editor { get; set; }

        public List<HeadInfo> SolutionHeadList { get { return _solutionHeadList; } }

        public List<Solution> Solutions { get { return _solutionList; } }
    }

}
