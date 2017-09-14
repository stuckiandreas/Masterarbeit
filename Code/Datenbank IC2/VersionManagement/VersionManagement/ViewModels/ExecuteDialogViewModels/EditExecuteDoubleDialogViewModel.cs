//-----------------------------------------------------------------------
// <copyright file="EditExecuteDoubleDialogViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.ViewModels.ExecuteDialogViewModels
{
    using InputViewModels;
    using VersionManagement.Events;
    using VersionManagement.Model;

    public class EditExecuteDoubleDialogViewModel : ExecuteDialogViewModel
    {
        private TextInputViewModel textInputViewModel1;

        private TextInputViewModel textInputViewModel2;

        private VersionManagementModel versionManagementModel;

        private string textInput1;

        private string textInput2;

        public EditExecuteDoubleDialogViewModel(
            ViewModelEvents viewModelEvents,
            string buttonText,
            VersionManagementModel versionManagementModel,
            string label1,
            string label2,
            string textInput1,
            string textInput2)
            : base(viewModelEvents, buttonText)
        {
            this.versionManagementModel = versionManagementModel;
            this.textInput1 = textInput1;
            this.textInput2 = textInput2;
            this.textInputViewModel1 = new TextInputViewModel(viewModelEvents, label1, textInput1);
            this.textInputViewModel2 = new TextInputViewModel(viewModelEvents, label2, textInput2);
        }

        public TextInputViewModel TextInputViewModel1
        {
            get
            {
                return this.textInputViewModel1;
            }

            set
            {
                this.textInputViewModel1 = value;
                this.OnPropertyChanged("TextInputViewModel1");
            }
        }

        public TextInputViewModel TextInputViewModel2
        {
            get
            {
                return this.textInputViewModel2;
            }

            set
            {
                this.textInputViewModel1 = value;
                this.OnPropertyChanged("TextInputViewModel2");
            }
        }

        /// <summary>
        /// Edit an element in the selection Item list
        /// </summary>
        public override void Execute()
        {
            this.versionManagementModel.SelectionItemDatabaseAccessManager.EditProperty(this.TextInputViewModel1.TextInput, this.textInput1, this.TextInputViewModel2.TextInput, this.textInput2);
            this.versionManagementModel.SoftwareVersionsDatabaseAccessManager.UpdatePropertyTypeInDatabase(this.TextInputViewModel1.TextInput, this.textInput1, this.TextInputViewModel2.TextInput, this.textInput2);
        }
    }  
}
