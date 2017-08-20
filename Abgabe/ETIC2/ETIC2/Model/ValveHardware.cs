//-----------------------------------------------------------------------
// <copyright file="ValveHardware.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Model
{
    using System.Collections.Generic;
    using System.Linq;

    /// <summary>
    /// Gets the Information from the ValveHardware table
    /// Shows a single valve hardware. With the important information about the test valve hardware
    /// </summary>
    public class ValveHardware : DatabaseAccessManager
    {
        public ValveHardware()
        {
        }

        /// <summary>
        /// Gets the entity framework valve hardwares.
        /// </summary>
        /// <returns>List with all ValveHardware Entries</returns>
        public List<EntityFramework.ValveHardware> GetEntityFrameworkValveHardwares()
        {
            return databaseContext.ValveHardware.ToList();
        }

        /// <summary>
        /// Gets a List with all ValveHardware Entries. Without any ID's -> Application Type ValveHardware
        /// </summary>
        /// <returns>List with all ValveHardware Entries</returns>
        public List<Application.HardwareView.ValveHardware> GetApplicationValveHardwares()
        {
            List<EntityFramework.ValveHardware> valveHardwareList = this.GetEntityFrameworkValveHardwares();
            List<Application.HardwareView.ValveHardware> applicationValveHardwareList = new List<Application.HardwareView.ValveHardware>();
            Application.HardwareView.ValveHardware emptyValveHardware;

            foreach (var valveHardware in valveHardwareList)
            {
                emptyValveHardware = new Application.HardwareView.ValveHardware()
                {
                    Id = default(int), ValveSerie = default(string), InterfaceType = default(string), ControllerType = default(string),
                    OptionType = default(string), ExternalIsolationValve = default(bool), ControllerHardwareVersion = default(string),
                    InterfaceHardwareVersion = default(string), ControllerAssemblyVariant = default(string),
                    InterfaceAssemblyVariant = default(string), Module1Type = default(string), Module1HardwareVersion = default(string),
                    Module1AssemblyVariant = default(string), Module2Type = default(string), Module2HardwareVersion = default(string),
                    Module2AssemblyVariant = default(string), Module3Type = default(string), Module3HardwareVersion = default(string),
                    Module3AssemblyVariant = default(string), Module4Type = default(string), Module4HardwareVersion = default(string),
                    Module4AssemblyVariant = default(string)
                };

                emptyValveHardware.Id = (int)valveHardware.Id;
                emptyValveHardware.ValveSerie = this.GetValveSerieEnum((int)valveHardware.Id);
                emptyValveHardware.InterfaceType = this.GetInterfaceTypeEnum((int)valveHardware.Id);
                emptyValveHardware.ControllerType = this.GetControllerTypeEnum((int)valveHardware.Id);
                emptyValveHardware.OptionType = this.GetOptionTypeEnum((int)valveHardware.Id);
                emptyValveHardware.ExternalIsolationValve = this.GetExternalIsolationValve((int)valveHardware.Id);
                emptyValveHardware.ControllerHardwareVersion = this.GetControllerHardwareVersionEnum((int)valveHardware.Id);
                emptyValveHardware.InterfaceHardwareVersion = this.GetInterfaceHardwareVersionEnum((int)valveHardware.Id);
                emptyValveHardware.ControllerAssemblyVariant = this.GetControllerAssemblyVariantEnum((int)valveHardware.Id);
                emptyValveHardware.InterfaceAssemblyVariant = this.GetInterfaceAssemblyVariantEnum((int)valveHardware.Id);
                emptyValveHardware.Module1Type = this.GetModule1TypeEnum((int)valveHardware.Id);
                emptyValveHardware.Module1HardwareVersion = this.GetModule1HardwareVersionEnum((int)valveHardware.Id);
                emptyValveHardware.Module1AssemblyVariant = this.GetModule1AssemblyVariantEnum((int)valveHardware.Id);
                emptyValveHardware.Module2Type = this.GetModule2TypeEnum((int)valveHardware.Id);
                emptyValveHardware.Module2HardwareVersion = this.GetModule2HardwareVersionEnum((int)valveHardware.Id);
                emptyValveHardware.Module2AssemblyVariant = this.GetModule2AssemblyVariantEnum((int)valveHardware.Id);
                emptyValveHardware.Module3Type = this.GetModule3TypeEnum((int)valveHardware.Id);
                emptyValveHardware.Module3HardwareVersion = this.GetModule3HardwareVersionEnum((int)valveHardware.Id);
                emptyValveHardware.Module3AssemblyVariant = this.GetModule3AssemblyVariantEnum((int)valveHardware.Id);
                emptyValveHardware.Module4Type = this.GetModule4TypeEnum((int)valveHardware.Id);
                emptyValveHardware.Module4HardwareVersion = this.GetModule4HardwareVersionEnum((int)valveHardware.Id);
                emptyValveHardware.Module4AssemblyVariant = this.GetModule4AssemblyVariantEnum((int)valveHardware.Id);

                applicationValveHardwareList.Add(emptyValveHardware);
            }

            return applicationValveHardwareList;
        }
    }
}
