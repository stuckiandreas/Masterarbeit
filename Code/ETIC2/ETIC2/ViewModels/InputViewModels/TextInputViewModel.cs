//-----------------------------------------------------------------------
// <copyright file="TextInputViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels.InputViewModels
{
    using System;
    using System.ComponentModel;
    using Events;

    /// <summary>
    /// View model in which a label can defined and a text block.
    /// </summary>
    /// <seealso cref="ETIC2.ViewModels.InputViewModels.InputBaseViewModel" />
    public class TextInputViewModel : InputBaseViewModel, IDataErrorInfo
    {
        /// <summary>
        /// Text of the text box
        /// </summary>
        private string textInput;

        public TextInputViewModel(ViewModelEvents viewModelEvents, string label, string textInput)
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

        public string Error { get; set; }

        public string this[string columnName]
        {
            get
            {
                if (this.textInput != null && this.textInput.Length > 100)
                    this.Error = "The text input is to long (max 100 character)";
                else
                    this.Error = string.Empty;

                return this.Error;
            }
        }
    }
}
