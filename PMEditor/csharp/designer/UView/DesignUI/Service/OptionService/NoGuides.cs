using System.ComponentModel.Design;
using System.Windows.Forms.Design;

namespace DesignUI.Service.OptionService
{
    internal class DesignerOptionServiceNoGuides : DesignerOptionService
    {
        public DesignerOptionServiceNoGuides() : base() { }

        protected override void PopulateOptionCollection(DesignerOptionCollection options)
        {
            if (null != options.Parent) return;

            DesignerOptions ops = new DesignerOptions();
            ops.GridSize = new System.Drawing.Size(8, 8);
            ops.SnapToGrid = false;
            ops.ShowGrid = false;
            ops.UseSnapLines = false;
            ops.UseSmartTags = true;
            DesignerOptionCollection wfd = this.CreateOptionCollection(options, "WindowsFormsDesigner", null);
            this.CreateOptionCollection(wfd, "General", ops);
        }
    }

}
