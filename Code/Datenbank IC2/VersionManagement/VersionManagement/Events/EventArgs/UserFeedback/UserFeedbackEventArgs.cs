//-----------------------------------------------------------------------
// <copyright file="UserFeedbackEventArgs.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.Events.EventArgs.UserFeedback
{
    using System.Windows;

    /// <summary>
    /// Basis class for all UserFeedback EventArgs
    /// These class is a ViewModel for MessageBoxes
    /// For the user to show an event, has he the option to define a caption, message, with a button and an image.
    /// </summary>
    public abstract class UserFeedbackEventArgs : System.EventArgs
    {
        /// <summary>
        /// The caption of the window which is shown by a user feedback
        /// </summary>
        private string caption;

        /// <summary>
        /// The message of the window which is shown by a user feedback
        /// </summary>
        private string message;

        /// <summary>
        /// The button of the window which is shown by a user feedback
        /// </summary>
        private MessageBoxButton button;

        /// <summary>
        /// The image of the window which is shown by a user feedback
        /// </summary>
        private MessageBoxImage image;

        /// <summary>
        /// The result of the user of the window which is shown by a user feedback
        /// </summary>
        private MessageBoxResult result = MessageBoxResult.None;

        public UserFeedbackEventArgs()
        {
        }

        public UserFeedbackEventArgs(string currentCaption, string currentMessage, MessageBoxButton currentButton, MessageBoxImage currentImage)
            : base()
        {
            this.caption = currentCaption;
            this.message = currentMessage;
            this.button = currentButton;
            this.image = currentImage;
        }

        public string Caption
        {
            get
            {
                return this.caption;
            }

            protected set
            {
                this.caption = value;
            }
        }

        public string Message
        {
            get
            {
                return this.message;
            }

            protected set
            {
                this.message = value;
            }
        }

        public MessageBoxButton Button
        {
            get
            {
                return this.button;
            }

            protected set
            {
                this.button = value;
            }
        }

        public MessageBoxImage Image
        {
            get
            {
                return this.image;
            }

            protected set
            {
                this.image = value;
            }
        }

        public MessageBoxResult Result
        {
            get
            {
                return this.result;
            }

            set
            {
                this.result = value;
            }
        }

        public override string ToString()
        {
            return base.ToString() + "; " +
                "Caption; " + this.caption + "; " +
                "Message; " + this.message;
        }
    }
}
