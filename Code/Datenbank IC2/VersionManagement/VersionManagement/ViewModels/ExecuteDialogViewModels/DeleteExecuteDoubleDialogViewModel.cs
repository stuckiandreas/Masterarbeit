//-----------------------------------------------------------------------
// <copyright file="DeleteExecuteDoubleDialogViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.ViewModels.ExecuteDialogViewModels
{
    using VersionManagement.Events;
    using VersionManagement.Model;
    using VersionManagement.ViewModels.InputViewModels;

    public class DeleteExecuteDoubleDialogViewModel : DeleteExecuteDialogViewModel
    {
        /// <summary>
        /// The view model events
        /// </summary>
        private ViewModelEvents viewModelEvents;

        /// <summary>
        /// The version management model
        /// </summary>
        private VersionManagementModel versionManagementModel;

        /// <summary>
        /// The text input view model2
        /// </summary>
        private TextInputViewModel textInputViewModelDescription;

        public DeleteExecuteDoubleDialogViewModel(
            ViewModelEvents viewModelEvents,
            string buttonText,
            VersionManagementModel versionManagementModel,
            SelectionItemType selectionItemType,
            string selectedItem,
            string selectedItemDescription)
            : base(viewModelEvents, buttonText, versionManagementModel, selectionItemType, selectedItem)
        {
            this.versionManagementModel = versionManagementModel;
            this.viewModelEvents = viewModelEvents;

            this.textInputViewModelDescription = new TextInputViewModel(viewModelEvents, "Description: ", selectedItemDescription);
            this.textInputViewModelDescription.IsReadOnly = true;
        }

        public TextInputViewModel TextInputViewModelDescription
        {
            get
            {
                return this.textInputViewModelDescription;
            }

            set
            {
                this.textInputViewModelDescription = value;
                this.OnPropertyChanged("TextInputViewModelDescription");
            }
        }

        /// <summary>
        /// Delete an element in the selection item list
        /// </summary>
        public override void Execute()
        {
            if (this.versionManagementModel.SelectionItemDatabaseAccessManager.DeletePropertType(TextInputViewModelSelectedItem.TextInput, this.TextInputViewModelDescription.TextInput) == false)
                this.ResultDeleteOperation = "Not possible. First all Properties Items with this PropertyType and Description has to delete.";
        }
    }
}
