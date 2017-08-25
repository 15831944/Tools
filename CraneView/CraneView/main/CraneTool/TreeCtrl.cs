using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CraneTool
{
	public class TreeCtrl : TreeView
	{
		private Dictionary<object, TreeNode> _dictObj = new Dictionary<object, TreeNode>();

		public TreeNode AddNode(object o, string text, TreeNode root)
		{
			if (_dictObj.ContainsKey(o))
				return _dictObj[o];

			TreeNode node = new TreeNode(text);
			node.Tag = o;
			_dictObj.Add(o, node);

			if (root != null)
				root.Nodes.Add(node);
			else
				Nodes.Add(node);
			return node;
		}

		public TreeNode AddNode(object o, string text, object parent)
		{
			if (_dictObj.ContainsKey(o))
				return _dictObj[o];
			if (!_dictObj.ContainsKey(parent))
				return null;

			TreeNode node = new TreeNode(text);
			node.Tag = o;
			_dictObj.Add(o, node);

			_dictObj[parent].Nodes.Add(node);
			return node;
		}

		public TreeNode FindNode(object o)
		{
			if (_dictObj.ContainsKey(o))
				return _dictObj[o];
			return null;
		}

		public void RemoveNode(object o)
		{
			if (_dictObj.ContainsKey(o))
			{
				_dictObj.Remove(o);
			}
		}
	}
}
