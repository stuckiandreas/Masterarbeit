//-----------------------------------------------------------------------
// <copyright file="SelectionItemDatabaseAccessManager.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.Model
{
    using System.Collections.Generic;
    using System.Linq;
    using VersionManagement.Model.Database;

    /// <summary>
    /// Get, add or delete selection item in the database
    /// </summary>
    public class SelectionItemDatabaseAccessManager : DatabaseAccessManager
    {
        public SelectionItemDatabaseAccessManager()
        {
        }

        public List<Authors> GetAuthors()
        {
            return databaseContext.Authors.ToList();
        }

        public List<Customers> GetCustomers()
        {
            return databaseContext.Customers.ToList();
        }

        public List<Systems> GetSystems()
        {
            return databaseContext.Systems.ToList();
        }

        public List<BaseSoftwares> GetBaseSoftwares()
        {
            return databaseContext.BaseSoftwares.ToList();
        }

        public List<DocumentTypes> GetDocumentTypes()
        {
            return databaseContext.DocumentTypes.ToList();
        }

        public List<PropertyTypes> GetPropertyTypes()
        {
            return databaseContext.PropertyTypes.ToList();
        }

        public void ClearSelectionItemListEntrysWithoutSoftwareVersionsLink()
        {
            //clear properties selection item list
            List<Properties> propertiesWithoutSoftwareVersionLink = databaseContext.Properties.Where(x => x.SoftwareVersionsId == null).ToList();

            foreach (var property in propertiesWithoutSoftwareVersionLink)
                databaseContext.Properties.Remove(property);

            //clear documents selection item list
            List<Documents> documentsWithoutSoftwareVersionLink = databaseContext.Documents.Where(x => x.SoftwareVersionsId == null).ToList();

            foreach (var document in documentsWithoutSoftwareVersionLink)
                databaseContext.Documents.Remove(document);

            //clear softwares selection item list
            List<Softwares> softwaresWithoutSoftwareVersionLink = databaseContext.Softwares.Where(x => x.SoftwareVersionsId == null).ToList();

            foreach (var software in softwaresWithoutSoftwareVersionLink)
                databaseContext.Softwares.Remove(software);

            //clear systemSoftwares selection item list
            List<SystemSoftwares> systemSoftwaresWithoutSoftwareVersionLink = databaseContext.SystemSoftwares.Where(x => x.SystemsId == null).ToList();

            foreach (var systemSoftware in systemSoftwaresWithoutSoftwareVersionLink)
                databaseContext.SystemSoftwares.Remove(systemSoftware);

            databaseContext.SaveChanges();
        }

        public void AddAuthor(string author)
        {
            databaseContext.Authors.Add(new Authors { Author = author });
            databaseContext.SaveChanges();
        }

        public void AddBaseSoftware(string baseSoftware)
        {
            databaseContext.BaseSoftwares.Add(new BaseSoftwares { BaseSoftware = baseSoftware });
            databaseContext.SaveChanges();
        }

        public void AddCustomer(string customer)
        {
            databaseContext.Customers.Add(new Customers { Customer = customer });
            databaseContext.SaveChanges();
        }

        public void AddSystem(string system)
        {
            databaseContext.Systems.Add(new Systems { System = system });
            databaseContext.SaveChanges();
        }

        public void AddDocumentType(string document)
        {
            databaseContext.DocumentTypes.Add(new DocumentTypes { DocumentType = document });
            databaseContext.SaveChanges();
        }

        public void AddPropertyType(string propertyType, string propertyDescription)
        {
            databaseContext.PropertyTypes.Add(new PropertyTypes { PropertyType = propertyType, PropertyDescription = propertyDescription });
            databaseContext.SaveChanges();
        }

        public void AddSystemSoftware(string system, string software)
        {
            Systems systems = new Systems();

            systems = databaseContext.Systems.Where(x => x.System == system).FirstOrDefault();
            if (systems != null)
            {
                systems.SystemSoftwares.Add(new SystemSoftwares { Software = software, Systems = systems });
                databaseContext.SaveChanges();
            }
        }

        /// <summary>
        /// Came later, after some entrys exist but i want the trunk entry on second place
        /// </summary>
        public void AddTrunkVersion()
        {
            List<BaseSoftwares> baseSoftwares = this.GetBaseSoftwares();
            int countBaseSoftwareList = baseSoftwares.Count;

            //delete all elements
            foreach (var item in baseSoftwares)
                this.DeleteBaseSoftware(item.BaseSoftware);

            //Add all with trunkl on second place
            for (int indexBaseSoftwareList = 0; indexBaseSoftwareList < countBaseSoftwareList; indexBaseSoftwareList++)
            {
                this.AddBaseSoftware(baseSoftwares.ElementAt(indexBaseSoftwareList).BaseSoftware);
                if (indexBaseSoftwareList == 0)
                    this.AddBaseSoftware("trunk");
            }
        }

        public void EditAuthor(string author, string oldAuthor)
        {
            Authors item = databaseContext.Authors.Where(x => x.Author == oldAuthor).First();

            if (item != null)
            {
                databaseContext.Authors.Remove(item);
                databaseContext.Authors.Add(new Authors { Author = author });
            }   

            databaseContext.SaveChanges();
        }

        public void EditCustomer(string customer, string oldCustomer)
        {
            Customers item = databaseContext.Customers.Where(x => x.Customer == oldCustomer).First();

            if (item != null) 
            {
                databaseContext.Customers.Remove(item);
                databaseContext.Customers.Add(new Customers { Customer = customer });
            }

            databaseContext.SaveChanges();
        }

        public void EditSystem(string system, string oldSystem)
        {
            Systems item = databaseContext.Systems.Where(x => x.System == oldSystem).First();

            if (item != null)
            {
                databaseContext.Systems.Remove(item);
                databaseContext.Systems.Add(new Systems { System = system });
            }

            databaseContext.SaveChanges();
        }

        public void EditDocumentType(string documentType, string oldDocumentType)
        {
            DocumentTypes item = databaseContext.DocumentTypes.Where(x => x.DocumentType == oldDocumentType).First();

            if (item != null)
            {
                databaseContext.DocumentTypes.Remove(item);
                databaseContext.DocumentTypes.Add(new DocumentTypes { DocumentType = documentType });
            }

            databaseContext.SaveChanges();
        }

        public void EditProperty(string propertyType, string oldPropertyType, string propertyDescription, string oldPropertyDescription)
        {
            PropertyTypes item = databaseContext.PropertyTypes.Where(x => x.PropertyType == oldPropertyType).Where(x => x.PropertyDescription == oldPropertyDescription).First();

            if (item != null)
            {
                databaseContext.PropertyTypes.Remove(item);
                databaseContext.PropertyTypes.Add(new PropertyTypes { PropertyType = propertyType, PropertyDescription = propertyDescription });
            }

            databaseContext.SaveChanges();
        }

        public void DeleteAuthor(string author)
        {
            Authors item = databaseContext.Authors.Where(x => x.Author == author).FirstOrDefault();

            if (item != null)
            {
                databaseContext.Authors.Remove(item);
                databaseContext.SaveChanges();
            }
        }

        public void DeleteBaseSoftware(string baseSoftware)
        {
            BaseSoftwares item = databaseContext.BaseSoftwares.Where(x => x.BaseSoftware == baseSoftware).FirstOrDefault();

            if (item != null)
            {
                databaseContext.BaseSoftwares.Remove(item);
                databaseContext.SaveChanges();
            }
        }

        public void DeleteCustomer(string customer)
        {
            Customers item = databaseContext.Customers.Where(x => x.Customer == customer).FirstOrDefault();

            if (item != null)
            {
                databaseContext.Customers.Remove(item);
                databaseContext.SaveChanges();
            }
        }

        public bool DeleteSystem(string system)
        {
            Systems item = databaseContext.Systems.Where(x => x.System == system).FirstOrDefault();

            if (item != null)
            {
                SoftwareVersions itemSoftwareVersions = databaseContext.SoftwareVersions.Where(x => x.System == system).FirstOrDefault();

                if (itemSoftwareVersions != null)
                    return false;
                else
                {
                    //delect subtables first
                    if (item.SystemSoftwares != null && item.SystemSoftwares.Count != 0)
                        databaseContext.SystemSoftwares.RemoveRange(item.SystemSoftwares.Where(x => x.Systems == item).ToList());
                    else
                    {
                        databaseContext.Systems.Remove(item);
                        databaseContext.SaveChanges();
                    }
                }
            }

            return true;
        }

        public void DeleteDocumentType(string document)
        {
            DocumentTypes item = databaseContext.DocumentTypes.Where(x => x.DocumentType == document).FirstOrDefault();

            if (item != null)
            {
                databaseContext.DocumentTypes.Remove(item);
                databaseContext.SaveChanges();
            }
        }

        public bool DeletePropertType(string propertyType, string propertyDescription)
        {
            PropertyTypes item = databaseContext.PropertyTypes.Where(x => x.PropertyType == propertyType && x.PropertyDescription == propertyDescription).FirstOrDefault();

            if (item != null)
            {
                Properties itemProperties = databaseContext.Properties.Where(x => x.PropertyType == propertyType && x.Description == propertyDescription).FirstOrDefault();
                if (itemProperties != null)
                    return false;
                else
                {
                    databaseContext.PropertyTypes.Remove(item);
                    databaseContext.SaveChanges();
                }
            }

            return true;
        }

        public void DeleteSystemSoftware(string system, string software)
        {
            Systems systems = new Systems();
            SystemSoftwares item = new SystemSoftwares();

            systems = databaseContext.Systems.Where(x => x.System == system).FirstOrDefault();

            if (systems != null)
                item = systems.SystemSoftwares.Where(x => x.Software == software).FirstOrDefault();

            if (item != null)
            {
                systems.SystemSoftwares.Remove(item);
                databaseContext.SaveChanges();
            }
        }
    }
}
