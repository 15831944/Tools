using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml.Serialization;
using UCore.Interface;

namespace UCore.Server
{
    public abstract class UChildBase : IUChild
    {
        private IUGroup _parent;
        public IUGroup Parent { get { return _parent; } set { _parent = value; } }

        public IUHeadInfo HeadInfo { get; set; }

        public virtual void InitTreeNode(TreeNode node)
        {
            node.Text = HeadInfo.Name;
            node.Tag = this;
        }

        public virtual void Save()
        {
            throw new NotImplementedException();
        }
    }
}
