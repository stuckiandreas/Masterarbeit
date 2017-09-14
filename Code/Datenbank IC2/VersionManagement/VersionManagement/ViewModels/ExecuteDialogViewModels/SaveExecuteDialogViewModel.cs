//-----------------------------------------------------------------------
// <copyright file="SaveExecuteDialogViewModel.cs" company="VAT Vakuumventile AG">
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
    /// Dialog view model with the user can save a selection item in the database.
    /// The save view model has a label and a text input.
    /// </summary>
    /// <seealso cref="VersionManagement.ViewModels.ExecuteDialogViewModels.ExecuteDialogViewModel" />
    public class SaveExecuteDialogViewModel : ExecuteDialogViewModel
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
            VersionManagementModel versionManagementModel,
            SelectionItemType selectionItemType)
            : base(viewModelEvents, buttonText)
        {
            this.textInputViewModel = new TextInputViewModel(viewModelEvents, "Entry: ", string.Empty);
            this.versionManagementModel = versionManagementModel;
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
                case SelectionItemType.Author:
                    this.versionManagementModel.SelectionItemDatabaseAccessManager.AddAuthor(this.textInputViewModel.TextInput);
                    break;
                case SelectionItemType.Customer:
                    this.versionManagementModel.SelectionItemDatabaseAccessManager.AddCustomer(this.textInputViewModel.TextInput);
                    break;
                case SelectionItemType.Softwares:
                    this.versionManagementModel.SelectionItemDatabaseAccessManager.AddSystem(this.textInputViewModel.TextInput);
                    break;
                case SelectionItemType.System:
                    this.versionManagementModel.SelectionItemDatabaseAccessManager.AddSystem(this.textInputViewModel.TextInput);
                    break;
                case SelectionItemType.DocumentType:
                    this.versionManagementModel.SelectionItemDatabaseAccessManager.AddDocumentType(this.textInputViewModel.TextInput);
                    break;
                default:
                    throw new NotImplementedException("Save Execute Function, Unknown SelectionItemType: " + this.selectionItemType);
            }                     
        }
    }
}
