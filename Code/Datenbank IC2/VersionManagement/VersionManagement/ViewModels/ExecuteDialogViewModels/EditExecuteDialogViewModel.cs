//-----------------------------------------------------------------------
// <copyright file="EditExecuteDialogViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.ViewModels.ExecuteDialogViewModels
{
    using System;
    using VersionManagement.Events;
    using VersionManagement.Model;
    using VersionManagement.ViewModels.InputViewModels;

    public class EditExecuteDialogViewModel : ExecuteDialogViewModel
    {
        /// <summary>
        /// Events which are available in all view models.
        /// </summary>
        private ViewModelEvents viewModelEvents;

        /// <summary>
        /// References to the model
        /// </summary>
        private VersionManagementModel versionManagementModel;

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
            VersionManagementModel versionManagementModel,
            SelectionItemType selectionItemType,
            string selectedItem)
            : base(viewModelEvents, buttonText)
        {
            this.SelectedItem = selectedItem;
            this.textInputViewModelSelectedItem = new TextInputViewModel(viewModelEvents, "Item name: ", selectedItem);
            this.versionManagementModel = versionManagementModel;
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
                case SelectionItemType.Author:
                    this.versionManagementModel.SelectionItemDatabaseAccessManager.EditAuthor(this.textInputViewModelSelectedItem.TextInput, this.SelectedItem);
                    this.versionManagementModel.SoftwareVersionsDatabaseAccessManager.UpdateAuthorInDatabase(this.textInputViewModelSelectedItem.TextInput, this.SelectedItem);
                    this.SelectedItem = this.textInputViewModelSelectedItem.TextInput;
                    break;
                case SelectionItemType.Customer:
                    this.versionManagementModel.SelectionItemDatabaseAccessManager.EditCustomer(this.textInputViewModelSelectedItem.TextInput, this.SelectedItem);
                    this.versionManagementModel.SoftwareVersionsDatabaseAccessManager.UpdateCustomerInDatabase(this.textInputViewModelSelectedItem.TextInput, this.SelectedItem);
                    this.SelectedItem = this.textInputViewModelSelectedItem.TextInput;
                    break;
                case SelectionItemType.Softwares:
                    this.versionManagementModel.SelectionItemDatabaseAccessManager.EditSystem(this.textInputViewModelSelectedItem.TextInput, this.SelectedItem);
                    this.versionManagementModel.SoftwareVersionsDatabaseAccessManager.UpdateSystemInDatabase(this.textInputViewModelSelectedItem.TextInput, this.SelectedItem);
                    this.SelectedItem = this.textInputViewModelSelectedItem.TextInput;
                    break;
                case SelectionItemType.System:
                    this.versionManagementModel.SelectionItemDatabaseAccessManager.EditSystem(this.textInputViewModelSelectedItem.TextInput, this.SelectedItem);
                    this.versionManagementModel.SoftwareVersionsDatabaseAccessManager.UpdateSystemInDatabase(this.textInputViewModelSelectedItem.TextInput, this.SelectedItem);
                    this.SelectedItem = this.textInputViewModelSelectedItem.TextInput;
                    break;
                case SelectionItemType.DocumentType:
                    this.versionManagementModel.SelectionItemDatabaseAccessManager.EditDocumentType(this.textInputViewModelSelectedItem.TextInput, this.SelectedItem);
                    this.versionManagementModel.SoftwareVersionsDatabaseAccessManager.UpdateDocumentTypeInDatabase(this.textInputViewModelSelectedItem.TextInput, this.SelectedItem);
                    this.SelectedItem = this.textInputViewModelSelectedItem.TextInput;
                    break;
                default:
                    throw new NotImplementedException("Edit Execute Function, Unknown SelectionItemType: " + this.selectionItemType);
            }
        }
    }
}
