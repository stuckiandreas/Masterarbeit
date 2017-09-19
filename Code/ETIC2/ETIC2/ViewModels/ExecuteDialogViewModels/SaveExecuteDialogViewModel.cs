//-----------------------------------------------------------------------
// <copyright file="SaveExecuteDialogViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels.ExecuteDialogViewModels
{
    using System;
    using ETIC2.Events;
    using InputViewModels;
    using Model.Application;
    
    /// <summary>
    /// Dialog view model with the user can save a selection item in the database.
    /// The save view model has a label and a text input.
    /// </summary>
    public class SaveExecuteDialogViewModel : ExecuteDialogViewModel
    {
        /// <summary>
        /// Events which are available in all view models.
        /// </summary>
        private ViewModelEvents viewModelEvents;

        /// <summary>
        /// References to the model
        /// </summary>
        private ETIC2Model etic2Model;

        /// <summary>
        /// Text input view model of the item that will be saved
        /// </summary>
        private TextInputViewModel textInputViewModel;

        /// <summary>
        /// Type of the combo box element.
        /// </summary>
        private SelectionItemType selectionItemType;

        public SaveExecuteDialogViewModel(
            ViewModelEvents viewModelEvents,
            string buttonText,
            ETIC2Model etic2Model,
            SelectionItemType selectionItemType)
            : base(viewModelEvents, buttonText)
        {
            this.textInputViewModel = new TextInputViewModel(viewModelEvents, "Entry: ", string.Empty);
            this.etic2Model = etic2Model;
            this.selectionItemType = selectionItemType;
            this.viewModelEvents = viewModelEvents;
        }

        public TextInputViewModel TextInputViewModel
        {
            get
            {
                return this.textInputViewModel;
            }

            set
            {
                this.textInputViewModel = value;
                this.OnPropertyChanged("TextInputViewModel");
            }
        }

        /// <summary>
        /// Add an element in the selection item list
        /// </summary>
        /// <exception cref="System.NotImplementedException">Unknown SelectionItemType;  + selectionItemType</exception>
        public override void Execute()
        {
            switch (this.selectionItemType)
            {
                case SelectionItemType.FailureType:
                    this.etic2Model.BuglistSelectedItem.AddFailureType(this.textInputViewModel.TextInput);
                    break;
                case SelectionItemType.StatusType:
                    this.etic2Model.BuglistSelectedItem.AddStatusType(this.textInputViewModel.TextInput);
                    break;
                case SelectionItemType.HardwareIdentificationLevel1:
                    this.etic2Model.BuglistSelectedItem.AddHardwareIdentificationLevel1(this.textInputViewModel.TextInput);
                    break;
                case SelectionItemType.HardwareIdentificationLevel2:
                    this.etic2Model.BuglistSelectedItem.AddHardwareIdentificationLevel2(this.textInputViewModel.TextInput);
                    break;
                case SelectionItemType.Priority:
                    this.etic2Model.BuglistSelectedItem.AddPriority(this.textInputViewModel.TextInput);
                    break;
                default:
                    throw new NotImplementedException("Save Execute Function, Unknown SelectionItemType: " + this.selectionItemType);
            }
        }
    }
}
