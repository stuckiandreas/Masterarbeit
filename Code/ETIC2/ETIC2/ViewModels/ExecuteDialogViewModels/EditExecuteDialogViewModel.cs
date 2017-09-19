//-----------------------------------------------------------------------
// <copyright file="EditExecuteDialogViewModel.cs" company="VAT Vakuumventile AG">
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
    /// Edit a buglist selected item
    /// </summary>
    public class EditExecuteDialogViewModel : ExecuteDialogViewModel
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
        /// Type of the combo box element.
        /// </summary>
        private SelectionItemType selectionItemType;

        /// <summary>
        /// The text input view model selected item
        /// </summary>
        private TextInputViewModel textInputViewModelSelectedItem;

        /// <summary>
        /// The selected item
        /// </summary>
        public string SelectedItem;

        public EditExecuteDialogViewModel(
            ViewModelEvents viewModelEvents,
            string buttonText,
            ETIC2Model etic2Model,
            SelectionItemType selectionItemType,
            string selectedItem)
            : base(viewModelEvents, buttonText)
        {
            this.SelectedItem = selectedItem;
            this.textInputViewModelSelectedItem = new TextInputViewModel(viewModelEvents, "Item name: ", selectedItem);
            this.etic2Model = etic2Model;
            this.selectionItemType = selectionItemType;
            this.viewModelEvents = viewModelEvents;
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

        /// <summary>
        /// Edit an element in the selection Item list
        /// </summary>
        public override void Execute()
        {
            switch (this.selectionItemType)
            {
                //Attention save the edit selection item as well in the SoftwareVersionDatabase
                //Set the selectionItem with the edit selectionItem value, Remark: the selectem Item set after the database Commands
                case SelectionItemType.FailureType:
                    this.etic2Model.BuglistSelectedItem.EditFailureType(this.textInputViewModelSelectedItem.TextInput, this.SelectedItem);
                    this.SelectedItem = this.textInputViewModelSelectedItem.TextInput;
                    break;
                case SelectionItemType.StatusType:
                    this.etic2Model.BuglistSelectedItem.EditStatusType(this.textInputViewModelSelectedItem.TextInput, this.SelectedItem);
                    this.SelectedItem = this.textInputViewModelSelectedItem.TextInput;
                    break;
                case SelectionItemType.HardwareIdentificationLevel1:
                    this.etic2Model.BuglistSelectedItem.EditHardwareIdentificationLevelel1(this.textInputViewModelSelectedItem.TextInput, this.SelectedItem);
                    this.SelectedItem = this.textInputViewModelSelectedItem.TextInput;
                    break;
                case SelectionItemType.HardwareIdentificationLevel2:
                    this.etic2Model.BuglistSelectedItem.EditHardwareIdentificationLevelel2(this.textInputViewModelSelectedItem.TextInput, this.SelectedItem);
                    this.SelectedItem = this.textInputViewModelSelectedItem.TextInput;
                    break;
                case SelectionItemType.Priority:
                    this.etic2Model.BuglistSelectedItem.EditPriority(this.textInputViewModelSelectedItem.TextInput, this.SelectedItem);
                    this.SelectedItem = this.textInputViewModelSelectedItem.TextInput;
                    break;
                default:
                    throw new NotImplementedException("Edit Execute Function, Unknown SelectionItemType: " + this.selectionItemType);
            }
        }
    }
}
