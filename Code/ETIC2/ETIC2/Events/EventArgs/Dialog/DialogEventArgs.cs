//-----------------------------------------------------------------------
// <copyright file="DialogEventArgs.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Events.EventArgs.Dialog
{
    using ETIC2.ViewModels;

    /// <summary>
    /// Event argument for the dialog window. The dialog window has a defined title. Is using from the user to add or delete an item. 
    /// </summary>
    /// <seealso cref="System.EventArgs" />
    public class DialogEventArgs : System.EventArgs
    {
        public DialogEventArgs(BasisViewModel basisViewModel, string windowTitle, int windowHeight, int windowWidth)
        {
            this.ViewModel = basisViewModel;
            this.WindowTitle = windowTitle;
            this.WindowHeight = windowHeight;
            this.WindowWidth = windowWidth;
        }

        public BasisViewModel ViewModel
        {
            get;
            set;
        }

        public string WindowTitle
        {
            get;
            set;
        }

        public int WindowHeight
        {
            get;
            set;
        }

        public int WindowWidth
        {
            get;
            set;
        }
    }
}
