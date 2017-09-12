//-----------------------------------------------------------------------
// <copyright file="DatabaseItemViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2015 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.ViewModels
{
    using System;
    using System.Collections.Generic;
    using Model.Database;

    /// <summary>
    /// One item that can be displayed in the application.
    /// </summary>
    public class DatabaseItemViewModel
    {
        public DatabaseItemViewModel()
        {
            this.Properties = new List<Properties>();
            this.Softwares = new List<Softwares>();
            this.Documents = new List<Documents>();
        }

        public DatabaseItemViewModel(SoftwareVersions softwareItem)
        {
            this.ItemIdentification = softwareItem.Id;
            this.Software = softwareItem.Software;
            this.BaseSoftware = softwareItem.Base;
            this.System = softwareItem.System;
            this.Customer = softwareItem.Customer;
            this.Author = softwareItem.Author;
            this.Date = softwareItem.Date;
            this.PssNumber = softwareItem.PssNumber;
            this.Description = softwareItem.Description;
            this.PrivateInfosBugs = softwareItem.PrivateInfosBugs;
            this.Properties = new List<Properties>(softwareItem.Properties);
            this.Softwares = new List<Softwares>(softwareItem.Softwares);
            this.Documents = new List<Documents>(softwareItem.Documents);
        }

        public int ItemIdentification
        {
            get;
            set;
        }

        public string Software
        {
            get;
            set;
        }

        public string BaseSoftware
        {
            get;
            set;
        }

        public string System
        {
            get;
            set;
        }

        public string Customer
        {
            get;
            set;
        }

        public string Author
        {
            get;
            set;
        }

        public DateTime? Date
        {
            get;
            set;
        }

        public int? PssNumber
        {
            get;
            set;
        }

        public string Description
        {
            get;
            set;
        }

        public string PrivateInfosBugs
        {
            get;
            set;
        }

        public List<Properties> Properties
        {
            get;
            set;
        }

        /// <summary>
        /// Gets the properties grid description.
        /// </summary>
        /// <value>
        /// The properties grid description.
        /// </value>
        public string PropertiesGridDescription
        {
            get
            {
                string output  = string.Empty;

                if (this.Properties != null)
                {
                    foreach (var property in this.Properties)
                        output = output + property.PropertyType + "/ ";
                }

                return output;
            }
        }

        public List<Softwares> Softwares
        {
            get;
            set;
        }

        public string SoftwaresGridDescription
        {
            get
            {
                string output = string.Empty;
                if (this.Softwares != null)
                {
                    foreach (var software in this.Softwares)
                        output = output + software.System + ": " + software.Software + "/ ";
                }

                return output;
            }
        }

        public List<Documents> Documents
        {
            get;
            set;
        }

        /// <summary>
        /// Gets the documents grid description.
        /// </summary>
        /// <value>
        /// The documents grid description.
        /// </value>
        public string DocumentsGridDescription
        {
            get
            {
                string output = string.Empty;
                if (this.Documents != null)
                {
                    foreach (var document in this.Documents)
                        output = output + document.DocumentType + ": " + document.Document + "/ ";
                }

                return output;
            }
        }
    }
}
