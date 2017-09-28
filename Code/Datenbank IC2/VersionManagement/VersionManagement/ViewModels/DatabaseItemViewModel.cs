//-----------------------------------------------------------------------
// <copyright file="DatabaseItemViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
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
                int iteration = 0;
                string output  = string.Empty;

                if (this.Properties != null)
                {
                    foreach (var property in this.Properties)
                    {
                        if (iteration == 0) output = property.PropertyType;
                        else output = output + " / " + property.PropertyType;
                        iteration++;
                    }
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
                int iteration = 0;
                string output = string.Empty;
                if (this.Softwares != null)
                {
                    foreach (var software in this.Softwares)
                    {
                        if (iteration == 0) output = software.System + ": " + software.Software;
                        else output = output + " / " + software.System + ": " + software.Software;
                        iteration++;
                    }
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
                int iteration = 0;
                string output = string.Empty;
                if (this.Documents != null)
                {
                    foreach (var document in this.Documents)
                    {
                        if (iteration == 0) output = document.DocumentType + ": " + document.Document;
                        else output = output + " / " + document.DocumentType + ": " + document.Document;
                        iteration++;
                    }
                }

                return output;
            }
        }
    }
}
