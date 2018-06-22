using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace UCore.Interface
{
    public interface IUGroup
    {
        IUHeadInfo HeadInfo { get; set; }

        IUGroup Parent { get; set; }

        List<IUGroup> Groups { get; }

        List<IUChild> Children { get; }

        bool AddChildGroup(IUGroup group);

        void RemoveGroup(IUGroup group);

        bool IsNameExist(string name);

        void InitTreeNode(TreeNode node);
    }
}
