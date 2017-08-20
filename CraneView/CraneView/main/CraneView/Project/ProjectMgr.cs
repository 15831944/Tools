using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.Xml.Serialization;
using CraneTool;
using System.Windows.Forms;

namespace CraneView.Project
{
	internal class ProjectMgr
	{
		internal List<ProjectHeadInfo> _projHeadList = new List<ProjectHeadInfo>();
		internal List<Project> _projList = new List<Project>();

		internal MainEditor Editor { get; set; }

		internal List<ProjectHeadInfo> ProjHeadList { get { return _projHeadList; } }

		internal ProjectMgr()
		{ }

		internal void SaveProjectHeadInfo()
		{
			try
			{
				string xml = XmlHelper.XmlSerialize(_projHeadList, Encoding.UTF8);
				byte[] data;
				EncryptHelper.EncryptResult res = EncryptHelper.Encrypt(xml, "ProjectHeadInfo", out data);

				FileStream fs = new FileStream("ProjHead.pjh", FileMode.Create);
				fs.Write(data, 0, data.Length);
				fs.Close();
			}
			catch (Exception e)
			{
				Console.WriteLine(e.Message);
			}
		}

		internal void LoadProjectHeadInfo()
		{
			try
			{
				FileStream fs = new FileStream("ProjHead.pjh", FileMode.Open, FileAccess.Read);
				byte[] data = new byte[(int)fs.Length];
				fs.Read(data, 0, (int)fs.Length);
				fs.Close();

				string xmlStr = "";
				EncryptHelper.EncryptResult res = EncryptHelper.Decrypt(data, "ProjectHeadInfo", ref xmlStr);
				if (res!= EncryptHelper.EncryptResult.Good)
				{
					return;
				}

				_projHeadList.Clear();
				_projHeadList = XmlHelper.XmlDeserialize<List<ProjectHeadInfo>>(xmlStr, Encoding.UTF8);
			}
			catch (Exception e)
			{
				Console.WriteLine(e.Message);
			}
		}

		internal bool LoadProj(ProjectHeadInfo phi)
		{
			var proj = _projList.Find(x => x.ProjInfo == phi);
			if (proj != null) return true;
			_projList .Add(new Project(phi));
			return true;
		}

		internal void CreateProj(string strName, string strPath, string strComment)
		{
			var phi = new ProjectHeadInfo();
			phi.OnCreate(strName, strPath, strComment);
			_projHeadList.Add(phi);

			var proj = new Project(phi);
			_projList.Add(proj);
		}

		internal bool IfProjExist(string strName)
		{
			return _projList.Find(x => (string.Compare(x.Name, strName, true) == 0)) != null;
		}

		internal Project FirstProject()
		{
			if (_projList.Count == 0) return null;
			return _projList[0];
		}

		internal TreeNode InitProjTree(CraneTool.TreeCtrl treeCtrl)
		{
			TreeNode node = treeCtrl.AddNode(this, "Projects", null);
			foreach(var proj in _projList)
			{
				proj.InitProjTree(treeCtrl, node);
			}
			node.ExpandAll();
			return node;
		}
	}
}
