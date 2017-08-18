﻿//-----------------------------------------------------------------------
// <copyright file="TestErrorMessage.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Model.Application.ErrorTopLevel
{
    /// <summary>
    /// Seperate Entity Framework Datatype from Application. Without references
    /// </summary>
    public class TestErrorMessage
    {
        public string Description;

        public int TestResultId;

        public TestErrorMessage()
        {
        }
    }
}
