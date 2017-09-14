//-----------------------------------------------------------------------
// <copyright file="DeleteExecuteDialogViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.ViewModels.ExecuteDialogViewModels
{
    using System;
    using Events;
    using InputViewModels;
    using Model;

    /// <summary>
    /// Dialog view model with the user can delete a selection item in the database. 
    /// The delete view model has a label and a text input. The text input shows the element which will be deleted.
    /// </summary>
    /// <seealso cref="VersionManagement.ViewModels.ExecuteDialogViewModels.ExecuteDialogViewModel" />
    public class DeleteExecuteDialogViewModel : ExecuteDialogViewModel
    {
        private ViewModelEvents viewModelEvents;

        private VersionManagementModel versionManagementModel;

        private TextInputViewModel textInputViewModelSelectedItem;

        private SelectionItemType selectionItemType;

        private string resultDeleteOperation;

        public DeleteExecuteDialogViewModel(
            ViewModelEvents viewModelEvents,
            string buttonText,
            VersionManagementModel versionManagementModel,
            SelectionItemType selectionItemType,
            string selectedItem)
            : base(viewModelEvents, buttonText)
        {
            this.textInputViewModelSelectedItem = new TextInputViewModel(viewModelEvents, "Item name: ", selectedItem);
            this.textInputViewModelSelectedItem.IsReadOnly = true;
            this.versionManagementModel = versionManagementModel;
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
                case SelectionItemType.Author:
                    this.versionManagementModel.SelectionItemDatabaseAccessManager.DeleteAuthor(this.textInputViewModelSelectedItem.TextInput);
                    break;
                case SelectionItemType.Customer:
                    this.versionManagementModel.SelectionItemDatabaseAccessManager.DeleteCustomer(this.textInputViewModelSelectedItem.TextInput);
                    break;
                case SelectionItemType.System:
                    if (this.versionManagementModel.SelectionItemDatabaseAccessManager.DeleteSystem(this.textInputViewModelSelectedItem.TextInput) == false)
                        this.resultDeleteOperation = "Not possible. First all Software Items with this System has to delete.";
                    break;
                case SelectionItemType.DocumentType:
                    this.versionManagementModel.SelectionItemDatabaseAccessManager.DeleteDocumentType(this.textInputViewModelSelectedItem.TextInput);
                    break;
                case SelectionItemType.Softwares:
                    if (this.versionManagementModel.SelectionItemDatabaseAccessManager.DeleteSystem(this.textInputViewModelSelectedItem.TextInput) == false)
                        this.resultDeleteOperation = "Not possible. First all Software Items with this System has to delete.";
                    break;
                case SelectionItemType.Properties:
                    if (this.versionManagementModel.SelectionItemDatabaseAccessManager.DeletePropertType(this.textInputViewModelSelectedItem.TextInput, this.textInputViewModelSelectedItem.ToolTip) == false)
                        this.resultDeleteOperation = "Not possible. First all Properties Items with this PropertyType and Description has to delete."; 
                    break;
                default:
                    throw new NotImplementedException("Delete Execute Function, Unknown SelectionItemType: " + this.selectionItemType);
            }
        }
    }
}
