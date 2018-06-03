using System.ComponentModel.Design;
using System.Windows.Forms.Design;

namespace DesignUI.Service.OptionService
{
    internal class DesignerOptionServiceSnapLines : DesignerOptionService
    {
        public DesignerOptionServiceSnapLines() : base() { }

        protected override void PopulateOptionCollection(DesignerOptionCollection options)
        {
            if (null != options.Parent) return;

            DesignerOptions ops = new DesignerOptions();
            ops.UseSnapLines = true;
            ops.UseSmartTags = true;
            DesignerOptionCollection wfd = this.CreateOptionCollection(options, "WindowsFormsDesigner", null);
            this.CreateOptionCollection(wfd, "General", ops);
        }
    }
}
