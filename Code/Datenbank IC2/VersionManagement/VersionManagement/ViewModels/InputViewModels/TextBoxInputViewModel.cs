//-----------------------------------------------------------------------
// <copyright file="TextBoxInputViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.ViewModels.InputViewModels
{
    using System;
    using System.ComponentModel;
    using VersionManagement.Events;

    /// <summary>
    /// View model in which a label can defined and a textbox.
    /// </summary>
    /// <seealso cref="VersionManagement.ViewModels.InputViewModels.InputBaseViewModel" />
    public class TextBoxInputViewModel : InputBaseViewModel, IDataErrorInfo
    {
        /// <summary>
        /// Text of the text box
        /// </summary>
        private string textInput;

        public TextBoxInputViewModel(ViewModelEvents viewModelEvents, string label, string textInput)
            : base(viewModelEvents, label)
        {
            this.TextInput = textInput;
        }

        public string TextInput
        {
            get
            {
                return this.textInput;
            }

            set
            {
                this.textInput = value;
                this.OnPropertyChanged("TextInput");
            }
        }

        public string Error
        {
            get;
            set;
        }

        public string this[string columnName]
        {
            get
            {
                if (this.textInput != null && this.textInput.Length > 500)
                    this.Error = "The text input is to long (max 500 character)";
                else
                    this.Error = string.Empty;

                return this.Error;
            }
        }
    }
}
