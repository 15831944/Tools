using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace UCore.Interface
{
    public interface IUServer
    {
        string Name { get; }

        Guid ID { get; }

        List<IUGroup> Groups { get; }

        void InitTreeNode(TreeNode node);
    }
}
