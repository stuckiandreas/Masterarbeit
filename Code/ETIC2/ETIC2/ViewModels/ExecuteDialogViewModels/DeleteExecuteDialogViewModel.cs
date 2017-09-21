//-----------------------------------------------------------------------
// <copyright file="DeleteExecuteDialogViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels.ExecuteDialogViewModels
{
    using System;
    using Events;
    using InputViewModels;
    using Model.Application;

    /// <summary>
    /// Dialog view model with the user can delete a selection item in the database. 
    /// The delete view model has a label and a text input. The text input shows the element which will be deleted.
    /// </summary>
    /// <seealso cref="ETIC2.ViewModels.ExecuteDialogViewModels.ExecuteDialogViewModel" />
    public class DeleteExecuteDialogViewModel : ExecuteDialogViewModel
    {
        private ViewModelEvents viewModelEvents;

        private ETIC2Model etic2Model;

        private TextInputViewModel textInputViewModelSelectedItem;

        private SelectionItemType selectionItemType;

        private string resultDeleteOperation;

        public DeleteExecuteDialogViewModel(
            ViewModelEvents viewModelEvents,
            string buttonText,
            ETIC2Model etic2Model,
            SelectionItemType selectionItemType,
            string selectedItem)
            : base(viewModelEvents, buttonText)
        {
            this.textInputViewModelSelectedItem = new TextInputViewModel(viewModelEvents, "Item name: ", selectedItem);
            this.textInputViewModelSelectedItem.IsReadOnly = true;
            this.etic2Model = etic2Model;
            this.selectionItemType = selectionItemType;
            this.viewModelEvents = viewModelEvents;
            this.resultDeleteOperation = string.Empty;
        }

        public TextInputViewModel TextInputViewModelSelectedItem
        {
            get
            {
                return this.textInputViewModelSelectedItem;
            }

            set
            {
                this.textInputViewModelSelectedItem = value;
                this.OnPropertyChanged("TextInputViewModelSelectedItem");
            }
        }

        public string ResultDeleteOperation
        {
            get
            {
                return this.resultDeleteOperation;
            }

            set
            {
                this.resultDeleteOperation = value;
            }
        }

        /// <summary>
        /// Delete an element in the selection item list
        /// </summary>
        /// <exception cref="System.NotImplementedException">Unknown SelectionItemType;  + selectionItemType</exception>
        public override void Execute()
        {
            switch (this.selectionItemType)
            {
                case SelectionItemType.FailureType:
                    this.etic2Model.BuglistSelectedItem.DeleteFailureType(this.textInputViewModelSelectedItem.TextInput);
                    break;
                case SelectionItemType.StatusType:
                    this.etic2Model.BuglistSelectedItem.DeleteStatusType(this.textInputViewModelSelectedItem.TextInput);
                    break;
                case SelectionItemType.HardwareIdentificationLevel1:
                    this.etic2Model.BuglistSelectedItem.DeleteHardwareIdentificationLevel1(this.textInputViewModelSelectedItem.TextInput);
                    break;
                case SelectionItemType.HardwareIdentificationLevel2:
                    this.etic2Model.BuglistSelectedItem.DeleteHardwareIdentificationLevel2(this.textInputViewModelSelectedItem.TextInput);
                    break;
                case SelectionItemType.Priority:
                    this.etic2Model.BuglistSelectedItem.DeletePriority(this.textInputViewModelSelectedItem.TextInput);
                    break;
                default:
                    throw new NotImplementedException("Delete Execute Function, Unknown SelectionItemType: " + this.selectionItemType);
            }
        }
    }
}
