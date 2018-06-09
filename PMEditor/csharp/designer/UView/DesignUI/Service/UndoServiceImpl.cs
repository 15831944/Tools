using System;
using System.Collections.Generic;
using System.ComponentModel.Design;
using System.Diagnostics;

namespace DesignUI.Service
{
    public class UndoServiceImpl : UndoEngine
    {
        private string _name = "UndoServiceImpl";
        private bool _lock = false;
        private Stack<UndoUnit> _undoStack = new Stack<UndoUnit>();
        private Stack<UndoUnit> _redoStack = new Stack<UndoUnit>();

        public UndoServiceImpl(IServiceProvider provider) : base(provider) { }

        public bool EnableUndo { get { return _undoStack.Count > 0; } }

        public bool EnableRedo { get { return _redoStack.Count > 0; } }

        public void Undo()
        {
            if (_undoStack.Count > 0)
            {
                try
                {
                    UndoEngine.UndoUnit unit = _undoStack.Pop();
                    unit.Undo();
                    _redoStack.Push(unit);
                    //Log("::Undo - undo action performed: " + unit.Name);
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(_name + ex.Message);
                    //Log("::Undo() - Exception " + ex.Message + " (line:" + new StackFrame(true).GetFileLineNumber() + ")");
                }
            }
            else
            {
                //Log("::Undo - NO undo action to perform!");
            }
        }

        public void Redo()
        {
            if (_redoStack.Count > 0)
            {
                try
                {
                    UndoEngine.UndoUnit unit = _redoStack.Pop();
                    unit.Undo();
                    _undoStack.Push(unit);
                    //Log("::Redo - redo action performed: " + unit.Name);
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(_name + ex.Message);
                    //Log("::Redo() - Exception " + ex.Message + " (line:" + new StackFrame(true).GetFileLineNumber() + ")");
                }
            }
            else
            {
                //Log("::Redo - NO redo action to perform!");
            }
        }

        public bool LockUndoService { get { return _lock; } set { _lock = value; } }

        protected override void AddUndoUnit(UndoEngine.UndoUnit unit)
        {
            if (!_lock)
            {
                _undoStack.Push(unit);
                _redoStack.Clear();
            }
        }
    }
}

