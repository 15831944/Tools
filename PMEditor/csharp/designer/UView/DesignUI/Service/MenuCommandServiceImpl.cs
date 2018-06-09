using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.ComponentModel.Design;
using System.ComponentModel;

namespace DesignUI.Service
{
    class MenuCommandServiceImpl : IMenuCommandService
    {
        //- this ServiceProvider is the DesignsurfaceUView instance 
        //- passed as paramter inside the ctor
        IServiceProvider _serviceProvider = null;
        MenuCommandService _menuCommandService = null;

        public MenuCommandServiceImpl(IServiceProvider serviceProvider)
        {
            this._serviceProvider = serviceProvider;
            _menuCommandService = new MenuCommandService(serviceProvider);
        }

        private void AddSubMenuItem(ContextMenuStrip contextMenu, CommandID id, string name, string shortCutkey = "", bool enable = true)
        {
            MenuCommand command = FindCommand(id);
            if (command != null)
            {
                var menuItem = new ToolStripMenuItem(name, null, new EventHandler(OnMenuClicked));
                if (!string.IsNullOrEmpty(shortCutkey))
                    menuItem.ShortcutKeyDisplayString = shortCutkey;
                menuItem.Text = name;
                menuItem.Tag = command;
                menuItem.Enabled = enable;
                contextMenu.Items.Add(menuItem);
            }
        }

        public void ShowContextMenu(CommandID menuID, int x, int y)
        {
            ContextMenuStrip contextMenu = new ContextMenuStrip();

            // Add the standard commands
            AddSubMenuItem(contextMenu, StandardCommands.Cut, UResource.Properties.Resource.EDIT_CUT + " (&X)", "Ctrl + X", IsEnableCut());
            AddSubMenuItem(contextMenu, StandardCommands.Copy, UResource.Properties.Resource.EDIT_COPY + " (&C)", "Ctrl + C");
            AddSubMenuItem(contextMenu, StandardCommands.Paste, UResource.Properties.Resource.EDIT_PASTE + " (&P)", "Ctrl + V");
            AddSubMenuItem(contextMenu, StandardCommands.Delete, UResource.Properties.Resource.EDIT_DELETE + " (&D)", "Delete");

            contextMenu.Items.Add(new ToolStripSeparator());
            AddSubMenuItem(contextMenu, StandardCommands.SelectAll, UResource.Properties.Resource.EDIT_SELECTALL + " (&S)", "Ctrl + A");
            AddSubMenuItem(contextMenu, StandardCommands.TabOrder, "Tab (&T)");

            contextMenu.Items.Add(new ToolStripSeparator());
            AddSubMenuItem(contextMenu, StandardCommands.BringToFront, UResource.Properties.Resource.EDIT_SETFRONT + " (&F)");
            AddSubMenuItem(contextMenu, StandardCommands.SendToBack, UResource.Properties.Resource.EDIT_SETBOTTOM + " (&B)");

            //- Show the contexteMenu
            DesignSurface surface = (DesignSurface)_serviceProvider;
            Control viewService = (Control)surface.View;
            if (viewService != null)
            {
                contextMenu.Show(viewService, viewService.PointToClient(new Point(x, y)));
            }
        }

        private bool IsEnableCut()
        {
            //DesignSurface surface = (DesignSurface)_serviceProvider;
            //surface.get
            return true;
        }

        //- Management of the selections of the contexteMenu
        private void OnMenuClicked(object sender, EventArgs e)
        {
            var menuItem = sender as ToolStripMenuItem;
            if (menuItem != null && menuItem.Tag is MenuCommand)
            {
                MenuCommand command = menuItem.Tag as MenuCommand;
                command.Invoke();
            }
        }

        public void AddCommand(MenuCommand command)
        {
            _menuCommandService.AddCommand(command);
        }

        public void AddVerb(DesignerVerb verb)
        {
            _menuCommandService.AddVerb(verb);
        }

        public MenuCommand FindCommand(CommandID commandID)
        {
            return _menuCommandService.FindCommand(commandID);
        }

        public bool GlobalInvoke(CommandID commandID)
        {
            return _menuCommandService.GlobalInvoke(commandID);
        }

        public void RemoveCommand(MenuCommand command)
        {
            _menuCommandService.RemoveCommand(command);
        }

        public void RemoveVerb(DesignerVerb verb)
        {
            _menuCommandService.RemoveVerb(verb);
        }

        public DesignerVerbCollection Verbs { get { return _menuCommandService.Verbs; } }
    }
}
