using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace UCore.Interface
{
    public interface IUServer
    {
        IUHeadInfo HeadInfo { get; set; }

        List<IUGroup> Groups { get; }

        List<IUChild> Children { get; }

        void InitTreeNode(TreeNode node);

        void Save();
    }
}
