//-----------------------------------------------------------------------
// <copyright file="DialogWindowViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2
{
    using ETIC2.ViewModels;
    using Events;

    /// <summary>
    /// View model for the dialog view. It used for user inputs.
    /// Further settings window and about information are display with the dialog window. 
    /// The title, height and width are individual definable.
    /// </summary>
    public class DialogWindowViewModel : BasisViewModel
    {
        private BasisViewModel viewModel;

        private string windowTitle;

        private int windowHeight;

        private int windowWidth;

        public DialogWindowViewModel(ViewModelEvents viewModelEvents, BasisViewModel viewModel, string windowTitle, int windowHeight, int windowWidth)
            : base(viewModelEvents)
        {
            this.viewModel = viewModel;
            this.windowTitle = windowTitle;
            this.windowHeight = windowHeight;
            this.windowWidth = windowWidth;
        }

        public BasisViewModel ViewModel
        {
            get
            {
                return this.viewModel;
            }

            set
            {
                this.viewModel = value;
                this.OnPropertyChanged("ViewModel");
            }
        }

        public string WindowTitle
        {
            get
            {
                return this.windowTitle;
            }

            set
            {
                this.windowTitle = value;
                this.OnPropertyChanged("WindowTitle");
            }
        }

        public int WindowHeight
        {
            get
            {
                return this.windowHeight;
            }

            set
            {
                this.windowHeight = value;
                this.OnPropertyChanged("WindowHeight");
            }
        }

        public int WindowWidth
        {
            get
            {
                return this.windowWidth;
            }

            set
            {
                this.windowWidth = value;
                this.OnPropertyChanged("WindowWidth");
            }
        }
    }
}
