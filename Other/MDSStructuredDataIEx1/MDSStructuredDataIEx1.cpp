/**
 * @file MDSStructuredDataIEx1.cpp
 * @brief Source file for class MDSStructuredDataIEx1
 * @date 08/04/2018
 * @author Andre' Neto
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class MDSStructuredDataIEx1 (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "BasicFile.h"
#include "MDSStructuredDataI.h"
#include "Object.h"
#include "ObjectRegistryDatabase.h"
#include "StandardParser.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
void ErrorProcessExampleFunction(const MARTe::ErrorManagement::ErrorInformation &errorInfo, const char * const errorDescription) {
    using namespace MARTe;
    StreamString errorCodeStr;
    ErrorManagement::ErrorCodeToStream(errorInfo.header.errorType, errorCodeStr);
    if (errorInfo.objectName) {
        printf("[%s - %s:%d - %s]: %s\n", errorCodeStr.Buffer(), errorInfo.fileName, errorInfo.header.lineNumber, errorInfo.objectName, errorDescription);
    }
    else {
        printf("[%s - %s:%d]: %s\n", errorCodeStr.Buffer(), errorInfo.fileName, errorInfo.header.lineNumber, errorDescription);
    }
}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

int main(int argc, char **argv) {
    using namespace MARTe;
	SetErrorProcessFunction(&ErrorProcessExampleFunction);
    //TODO all parameters assumed
    bool createTree = false;
    bool createTreeFromCfg = false;
    bool ok = true;
    if (StringHelper::Compare(argv[1], "-f") == 0) {
        ok = (argc > 2);
        createTreeFromCfg = ok;
    }
    if (StringHelper::Compare(argv[1], "-c") == 0) {
        createTree = true;
    }
    if (createTreeFromCfg) {
        BasicFile f;
        ok = f.Open(argv[2], BasicFile::ACCESS_MODE_R);
        if (ok) {
            ok = f.Seek(0);
        }
        ConfigurationDatabase cdb;
        if (ok) {
            StreamString err;
            StandardParser parser(f, cdb, &err);
            ok = parser.Parse();
            if (!ok) {
                REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed to parse %s", err.Buffer());
            }
        }
        MDSStructuredDataI mdsi;
        if (ok) {
            ok = mdsi.CreateTree("mdssdi", true);
        }
        if (ok) {
            ok = mdsi.OpenTree("mdssdi", -1);
        }
        if (ok) {
            ok = mdsi.SetEditMode(true);
        }
        if (ok) {
            ok = cdb.Copy(mdsi);
        }
        if (ok) {
            ok = mdsi.SaveTree();
        }
        if (ok) {
            ok = mdsi.CloseTree();
        }
        
        (void) f.Close();
    }
    else if (createTree) {
        MDSStructuredDataI mdsi;
        if (ok) {
            ok = mdsi.CreateTree("mdssdi", true);
        }
        if (ok) {
            ok = mdsi.OpenTree("mdssdi", -1);
        }
        if (ok) {
            ok = mdsi.SetEditMode(true);
        }
        if (ok) {
            ok = mdsi.CreateAbsolute("_MyCtrl1");
        }
        if (ok) {
            ok = mdsi.Write("Tau", 0.1);
        }
        if (ok) {
            ok = mdsi.CreateRelative("Gains1.High");
        }
        if (ok) {
            ok = mdsi.Write("Gain1", 2.1);
        }
        if (ok) {
            ok = mdsi.Write("Gain2", 3);
        }
        if (ok) {
            float32 arr[6] = {1, 2, 3, 4, 5, 6};
            ok = mdsi.Write("Gain3", arr);
        }
        if (ok) {
            ok = mdsi.MoveToAncestor(1);
        }
        if (ok) {
            ok = mdsi.CreateRelative("Low");
        }
        if (ok) {
            ok = mdsi.Write("Gain1", -1.1);
        }
        if (ok) {
            ok = mdsi.Write("Gain2", 6);
        }
        if (ok) {
            float32 arr[6] = {1.1, 2.2, 3.2, 4.3, 5.34, 6.2321};
            ok = mdsi.Write("Gain3", arr);
        }
        if (ok) {
            ok = mdsi.MoveToAncestor(2);
        }
        if (ok) {
            ok = mdsi.CreateRelative("Gains2.High");
        }
        if (ok) {
            ok = mdsi.Write("Gain1", -2.1);
        }
        if (ok) {
            ok = mdsi.Write("Gain2", -3);
        }
        if (ok) {
            float32 arr[6] = {-1, -2, -3, -4, -5, -6};
            ok = mdsi.Write("Gain3", arr);
        }
        if (ok) {
            ok = mdsi.MoveToAncestor(1);
        }
        if (ok) {
            ok = mdsi.CreateRelative("Low");
        }
        if (ok) {
            ok = mdsi.Write("Gain1", -9.3);
        }
        if (ok) {
            ok = mdsi.Write("Gain2", 8);
        }
        if (ok) {
            float32 arr[6] = {4.3, 1.3, 8.3, 5.6, 7.4, 9.32};
            ok = mdsi.Write("Gain3", arr);
        }

        if (ok) {
            ok = mdsi.SaveTree();
        }
        if (ok) {
            ok = mdsi.CloseTree();
        }
    }
    else {
        MDSStructuredDataI mdsi;
        ReferenceContainer::AddBuildToken('_');
        if (ok) {
            ok = mdsi.OpenTree("mdssdi", -1);
        }
        if (ok) {
            ok = mdsi.MoveRelative("_MyCtrl1");
        }
        ReferenceT<Object> myCntrl("ControllerEx1");
        if (myCntrl.IsValid()) {
            myCntrl->Initialise(mdsi);
        }
        if (ok) {
            ok = mdsi.CloseTree();
        }
    }

    return 0;
}

