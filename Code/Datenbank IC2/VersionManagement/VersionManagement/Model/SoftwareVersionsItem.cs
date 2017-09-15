//-----------------------------------------------------------------------
// <copyright file="SoftwareVersionsItem.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.Model
{
    using System.Collections.Generic;
    using System.Linq;
    using VersionManagement.Model.Database;

    /// <summary>
    /// Get, set or delete the software item in the database.
    /// </summary>
    public class SoftwareVersionsItem : DatabaseAccessManager
    {
        public SoftwareVersionsItem()
        {
        }

        /// <summary>
        /// Get one specific software item from the database. If no item has been found NULL will be returned.
        /// </summary>
        /// <param name="software">The software.</param>
        /// <returns>software item</returns>
        public SoftwareVersions GetItemFromDatabase(string software)
        {
            return databaseContext.SoftwareVersions.Where(x => x.Software == software).FirstOrDefault();
        }

        /// <summary>
        /// Get all software item from the database.
        /// </summary>
        /// <returns>list of software</returns>
        public List<SoftwareVersions> GetSoftwareVersions()
        {
            return databaseContext.SoftwareVersions.ToList();
        }

        /// <summary>
        /// Add a SoftwareVersion to the database.
        /// </summary>
        /// <param name="softwareVersion">single software</param>
        public void AddSoftwareVersion(SoftwareVersions softwareVersion)
        {
            databaseContext.SoftwareVersions.Add(softwareVersion);
            databaseContext.SaveChanges();
        }

        /// <summary>
        /// Delete a SoftwareVersion from the database
        /// </summary>
        /// <param name="software">The software.</param>
        public void DeleteSoftwareVersion(string software)
        {
            SoftwareVersions softwareVersion = databaseContext.SoftwareVersions.Where(x => x.Software == software).FirstOrDefault();

            if (softwareVersion != null)
            {
                //Remove sub tables first with foreign keys
                databaseContext.Properties.RemoveRange(softwareVersion.Properties);
                databaseContext.Documents.RemoveRange(softwareVersion.Documents);
                databaseContext.Softwares.RemoveRange(softwareVersion.Softwares);

                //After subtables remove main table
                databaseContext.SoftwareVersions.Remove(softwareVersion);
                databaseContext.SaveChanges();
            }
        }

        /// <summary>
        /// Updates the item in database.
        /// </summary>
        /// <param name="softwareVersion">The software version.</param>
        public void UpdateItemInDatabase(SoftwareVersions softwareVersion)
        {
            SoftwareVersions softwareVersionToUpdate = databaseContext.SoftwareVersions.Where(x => x.Id == softwareVersion.Id).FirstOrDefault();

            if (softwareVersionToUpdate != null)
            {
                softwareVersionToUpdate.Software = softwareVersion.Software;
                softwareVersionToUpdate.Author = softwareVersion.Author;
                softwareVersionToUpdate.Base = softwareVersion.Base;
                softwareVersionToUpdate.Customer = softwareVersion.Customer;
                softwareVersionToUpdate.Date = softwareVersion.Date;
                softwareVersionToUpdate.PrivateInfosBugs = softwareVersion.PrivateInfosBugs;
                softwareVersionToUpdate.Description = softwareVersion.Description;
                softwareVersionToUpdate.PssNumber = softwareVersion.PssNumber;
                softwareVersionToUpdate.System = softwareVersion.System;
                softwareVersionToUpdate.Properties = softwareVersion.Properties;
                softwareVersionToUpdate.Documents = softwareVersion.Documents;
                softwareVersionToUpdate.Softwares = softwareVersion.Softwares;
            }

            databaseContext.SaveChanges();
        }

        /// <summary>
        /// Updates the author in database.
        /// </summary>
        /// <param name="author">The author.</param>
        /// <param name="oldAuthor">The old author.</param>
        public void UpdateAuthorInDatabase(string author, string oldAuthor)
        {
            List<SoftwareVersions> softwareVersionListToUpdate = databaseContext.SoftwareVersions.Where(x => x.Author == oldAuthor).ToList();

            foreach (var softwareVersion in softwareVersionListToUpdate)
                softwareVersion.Author = author;

            databaseContext.SaveChanges();
        }

        /// <summary>
        /// Updates the customer in database.
        /// </summary>
        /// <param name="customer">The customer.</param>
        /// <param name="oldCustomer">The old customer.</param>
        public void UpdateCustomerInDatabase(string customer, string oldCustomer)
        {
            List<SoftwareVersions> softwareVersionListToUpdate = databaseContext.SoftwareVersions.Where(x => x.Customer == oldCustomer).ToList();

            foreach (var softwareVersion in softwareVersionListToUpdate)
                softwareVersion.Customer = customer;

            databaseContext.SaveChanges();
        }

        /// <summary>
        /// Updates the system in database.
        /// </summary>
        /// <param name="system">The system.</param>
        /// <param name="oldSystem">The old system.</param>
        public void UpdateSystemInDatabase(string system, string oldSystem)
        {
            List<SoftwareVersions> softwareVersionListToUpdate = databaseContext.SoftwareVersions.Where(x => x.System == oldSystem).ToList();

            foreach (var softwareVersion in softwareVersionListToUpdate)
                softwareVersion.System = system;

            databaseContext.SaveChanges();
        }

        /// <summary>
        /// Updates the document type in database.
        /// </summary>
        /// <param name="documentType">Type of the document.</param>
        /// <param name="oldDocumentType">Old type of the document.</param>
        public void UpdateDocumentTypeInDatabase(string documentType, string oldDocumentType)
        {
            List<Documents> documentListToUpdate = databaseContext.Documents.Where(x => x.DocumentType == oldDocumentType).ToList();

            foreach (var document in documentListToUpdate)
                document.DocumentType = documentType;

            databaseContext.SaveChanges();
        }

        public void UpdatePropertyTypeInDatabase(string propertyType, string oldPropertyType, string propertyDescription, string oldPropertyDescription)
        {
            List<Properties> propertyListToUpdate = databaseContext.Properties.Where(x => x.PropertyType == oldPropertyType).Where(x => x.Description == oldPropertyDescription).ToList();

            foreach (var property in propertyListToUpdate)
            {
                property.PropertyType = propertyType;
                property.Description = propertyDescription;
            }

            databaseContext.SaveChanges();
        }
    }
}
