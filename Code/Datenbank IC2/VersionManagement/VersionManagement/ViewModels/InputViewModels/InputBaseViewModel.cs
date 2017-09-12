//-----------------------------------------------------------------------
// <copyright file="InputBaseViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.ViewModels.InputViewModels
{
    using VersionManagement.Events;

    /// <summary>
    /// Base class for all input view models. In this class the label is set. Tooltip for the user to help for special function. 
    /// </summary>
    /// <seealso cref="VersionManagement.ViewModels.BasisViewModel" />
    public abstract class InputBaseViewModel : BasisViewModel
    {
        /// <summary>
        /// True if view model is read only
        /// </summary>
        private bool isReadOnly;

        /// <summary>
        /// Label on the left side of the view model
        /// </summary>
        private string label;

        /// <summary>
        /// Tool tip for further information
        /// </summary>
        private string toolTip;

        public InputBaseViewModel(ViewModelEvents viewModelEvents, string label)
            : base(viewModelEvents)
        {
            this.Label = label;
        }

        public string ToolTip
        {
            get
            {
                return this.toolTip;
            }

            set
            {
                this.toolTip = value;
                this.OnPropertyChanged("ToolTip");
            }
        }

        public bool IsReadOnly
        {
            get
            {
                return this.isReadOnly;
            }

            set
            {
                this.isReadOnly = value;
                this.OnPropertyChanged("IsReadOnly");
            }
        }

        public string Label
        {
            get
            {
                return this.label;
            }

            private set
            {
                this.label = value;
                this.OnPropertyChanged("Label");
            }
        }
    }
}
