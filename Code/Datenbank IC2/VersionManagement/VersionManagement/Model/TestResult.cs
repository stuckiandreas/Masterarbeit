//-----------------------------------------------------------------------
// <copyright file="TestResult.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.Model
{
    using System.Linq;
    using Database;

    /// <summary>
    /// Works with the Test Result Tables. Request if the software versions is used in the test results
    /// </summary>
    /// <seealso cref="VersionManagement.Model.DatabaseAccessManager" />
    public class TestResult : DatabaseAccessManager
    {
        public TestResult()
        {
        }

        public bool IfValveFirmwwareIsUsedInTestResult(SoftwareVersions softwareVersions)
        {
            return databaseContext.InitialStateFirmware.Where(x => x.SoftwareVersions_Id_Firmware == softwareVersions.Id).Any();
        }

        public bool IfMotionControllerFirmwwareIsUsedInTestResult(SoftwareVersions softwareVersions)
        {
            return databaseContext.InitialStateFirmware.Where(x => x.SoftwareVersions_Id_MotionController == softwareVersions.Id).Any();
        }

        public bool IfInterfaceFirmwwareIsUsedInTestResult(SoftwareVersions softwareVersions)
        {
            return databaseContext.InitialStateFirmware.Where(x => x.SoftwareVersions_Id_Interface == softwareVersions.Id).Any();
        }
    }
}
