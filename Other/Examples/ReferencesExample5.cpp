/**
 * @file ReferencesExample5.cpp
 * @brief Source file for class ReferencesExample5
 * @date 14/03/2018
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
 * the class ReferencesExample5 (public, protected, and private). Be aware that some
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
#include "ClassRegistryDatabase.h"
#include "ErrorLoggerExample.h"
#include "Object.h"
#include "Reference.h"
#include "ReferenceContainer.h"
#include "ReferenceT.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe2Tutorial {


class ControllerEx1: public MARTe::Object {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief NOOP.
     */
ControllerEx1    () {
        gain = 0u;
    }

    virtual ~ControllerEx1() {
        using namespace MARTe;
        if (GetName() != NULL) {
            REPORT_ERROR_STATIC(ErrorManagement::Information, "No more references "
            		"pointing at %s [%s]. The Object will "
            		"be safely deleted.", GetName(), GetClassProperties()->GetName());
        }
    }

    virtual void AFunction () {
        using namespace MARTe;
        REPORT_ERROR_STATIC(ErrorManagement::Information, "AFunction called @ %s.", GetName());
    }

    /**
     * A property.
     */
    MARTe::uint32 gain;
};

CLASS_REGISTER(ControllerEx1, "")

class PIDEx1: public ControllerEx1 {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief NOOP.
     */
PIDEx1    () {
        property3 = 0u;
    }

    virtual ~PIDEx1() {
        using namespace MARTe;
        REPORT_ERROR_STATIC(ErrorManagement::Information, "No more references "
        		"pointing at %s [%s]. The Object will "
        		"be safely deleted.", GetName(), GetClassProperties()->GetName());
    }

    virtual void AFunction () {
        using namespace MARTe;
        REPORT_ERROR_STATIC(ErrorManagement::Information, "AFunction called @ %s.", GetName());
    }
    /**
     * A property.
     */
    MARTe::uint32 property3;
};

CLASS_REGISTER(PIDEx1, "")


class ControllerContainerEx1: public MARTe::ReferenceContainer {
public:
    CLASS_REGISTER_DECLARATION()

	/**
	 * @brief NOOP.
	 */
	ControllerContainerEx1() {
    }

    virtual ~ControllerContainerEx1() {
        using namespace MARTe;
        REPORT_ERROR_STATIC(ErrorManagement::Information, "No more references "
        		"pointing at %s [%s]. The Object will "
        		"be safely deleted.", GetName(), GetClassProperties()->GetName());
    }

    /**
     * @brief Calls AFunction on all of its elements
     */
    void Do() {
    	using namespace MARTe;
    }

};
CLASS_REGISTER(ControllerContainerEx1, "")

}

int main(int argc, char **argv) {
    using namespace MARTe;
    using namespace MARTe2Tutorial;
    SetErrorProcessFunction(&ErrorProcessExampleFunction);

    CCString className1 = "ControllerEx1";
    CCString className2 = "PIDEx1";

    //Automatically generate a new object instance based on the class
    //name and on the correct Heap and with the template reference.
    ReferenceT<ControllerEx1> ref1(className1,
    		GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (ref1.IsValid()) {
        ref1->SetName("ControllerInstance1");
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Successfully "
        		"created an instance of %s", className1.GetList());
    }
    ReferenceT<ControllerEx1> ref2(className2,
    		GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (ref2.IsValid()) {
        ref2->SetName("ControllerInstance2");
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Successfully "
        		"created an instance of %s", className2.GetList());
    }
    //This mechanism also works with compatible subclasses PIDEx1->ControllerEx1
    ReferenceT<PIDEx1> ref3(className2,
    		GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (ref3.IsValid()) {
        ref3->SetName("PIDInstance1");
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Successfully "
        		"created an instance of %s", className2.GetList());
    }

    ReferenceT<ControllerContainerEx1> controllerContainer("ControllerContainerEx1",
    		GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (controllerContainer.IsValid()) {
    	controllerContainer->SetName("Container");
    	controllerContainer->Insert(ref1);
    	controllerContainer->Insert(ref2);
    	controllerContainer->Insert(ref3);
    	controllerContainer->Do();
    }
    return 0;
}

