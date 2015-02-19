/**

GDevelop - Light Extension
Copyright (c) 2010-2015 Florian Rival (Florian.Rival@gmail.com)
This project is released under the MIT License.
*/

#include "GDCpp/ExtensionBase.h"
#include "GDCore/Tools/Version.h"
#include "LightObstacleAutomatism.h"
#include "LightObject.h"
#include "SceneLightObstacleDatas.h"


/**
 * \brief This class declares information about the extension.
 */
class Extension : public ExtensionBase
{
public:

    /**
     * Constructor of an extension declares everything the extension contains: objects, actions, conditions and expressions.
     */
    Extension()
    {
        SetExtensionInformation("Light",
                              _("Light"),
                              _("Allow to display lights and use light obstacles."),
                              "Florian Rival",
                              "Open source (MIT License)");

            {
                gd::ObjectMetadata & obj = AddObject("Light",
                           _("Light"),
                           _("Emits light that can be stopped by objects"),
                           "CppPlatform/Extensions/lightIcon32.png",
                           &CreateLightObject);

                AddRuntimeObject(obj, "RuntimeLightObject", CreateRuntimeLightObject);

                #if defined(GD_IDE_ONLY)
                LightObject::LoadEdittimeIcon();
                obj.SetIncludeFile("Light/LightObject.h");

                obj.AddAction("ChangeColor",
                               _("Color"),
                               _("Change light color."),
                               _("Change color of _PARAM0_ to _PARAM1_"),
                               _("Setup"),
                               "CppPlatform/Extensions/lightIcon24.png",
                               "CppPlatform/Extensions/lightIcon16.png")
                    .AddParameter("object", _("Object"), "Light", false)
                    .AddParameter("color", _("Color"))
                    .codeExtraInformation.SetFunctionName("SetColor").SetIncludeFile("Light/LightObject.h");


                obj.AddAction("Intensity",
                               _("Intensity"),
                               _("Modify the intensity of a light"),
                               _("Do _PARAM1__PARAM2_ to the intensity of _PARAM0_"),
                               _("Setup"),
                               "CppPlatform/Extensions/lightIcon24.png",
                               "CppPlatform/Extensions/lightIcon16.png")
                    .AddParameter("object", _("Object"), "Light", false)
                    .AddParameter("operator", _("Modification's sign"))
                    .AddParameter("expression", _("Value"))
                .codeExtraInformation.SetFunctionName("SetIntensity").SetManipulatedType("number").SetAssociatedGetter("GetIntensity").SetIncludeFile("Light/LightObject.h");


                obj.AddCondition("Intensity",
                               _("Intensity"),
                               _("Test the intensity of a light."),
                               _("Intensity of _PARAM0_ is _PARAM1__PARAM2_"),
                               _("Setup"),
                               "CppPlatform/Extensions/lightIcon24.png",
                               "CppPlatform/Extensions/lightIcon16.png")
                    .AddParameter("object", _("Object"), "Light", false)
                    .AddParameter("relationalOperator", _("Sign of the test"))
                    .AddParameter("expression", _("Value to test"))
                .codeExtraInformation.SetFunctionName("GetIntensity").SetManipulatedType("number").SetIncludeFile("Light/LightObject.h");


                obj.AddAction("Radius",
                               _("Radius"),
                               _("Modify the radius of a liht"),
                               _("Do _PARAM1__PARAM2_ to radius of _PARAM0_"),
                               _("Setup"),
                               "CppPlatform/Extensions/lightIcon24.png",
                               "CppPlatform/Extensions/lightIcon16.png")
                    .AddParameter("object", _("Object"), "Light", false)
                    .AddParameter("operator", _("Modification's sign"))
                    .AddParameter("expression", _("Value"))
                .codeExtraInformation.SetFunctionName("SetRadius").SetManipulatedType("number").SetAssociatedGetter("GetRadius").SetIncludeFile("Light/LightObject.h");


                obj.AddCondition("Radius",
                               _("Radius"),
                               _("Test the radius of a light."),
                               _("The radius of _PARAM0_ is _PARAM2_ _PARAM1_"),
                               _("Setup"),
                               "CppPlatform/Extensions/lightIcon24.png",
                               "CppPlatform/Extensions/lightIcon16.png")
                    .AddParameter("object", _("Object"), "Light", false)
                    .AddParameter("relationalOperator", _("Sign of the test"))
                    .AddParameter("expression", _("Value to test"))
                .codeExtraInformation.SetFunctionName("GetRadius").SetManipulatedType("number").SetIncludeFile("Light/LightObject.h");


                obj.AddAction("Quality",
                               _("Quality"),
                               _("Modify the quality of a light"),
                               _("Do _PARAM1__PARAM2_ to the quality of _PARAM0_"),
                               _("Setup"),
                               "CppPlatform/Extensions/lightIcon24.png",
                               "CppPlatform/Extensions/lightIcon16.png")
                    .AddParameter("object", _("Object"), "Light", false)
                    .AddParameter("operator", _("Modification's sign"))
                    .AddParameter("expression", _("Value"))
                .codeExtraInformation.SetFunctionName("SetQuality").SetManipulatedType("number").SetAssociatedGetter("GetQuality").SetIncludeFile("Light/LightObject.h");


                obj.AddCondition("Quality",
                               _("Quality"),
                               _("Test the quality of a light"),
                               _("The quality of _PARAM0_ is _PARAM1__PARAM2_"),
                               _("Setup"),
                               "CppPlatform/Extensions/lightIcon24.png",
                               "CppPlatform/Extensions/lightIcon16.png")
                    .AddParameter("object", _("Object"), "Light", false)
                    .AddParameter("relationalOperator", _("Sign of the test"))
                    .AddParameter("expression", _("Value to test"))
                .codeExtraInformation.SetFunctionName("GetQuality").SetManipulatedType("number").SetIncludeFile("Light/LightObject.h");


                obj.AddAction("ChangeGlobalColor",
                               _("Global color"),
                               _("Change scene color for a global light."),
                               _("Change scene global color of _PARAM0_ to _PARAM1_"),
                               _("Setup"),
                               "CppPlatform/Extensions/lightIcon24.png",
                               "CppPlatform/Extensions/lightIcon16.png")
                    .AddParameter("object", _("Object"), "Light", false)
                    .AddParameter("color", _("Color"))
                .codeExtraInformation.SetFunctionName("SetGlobalColor").SetIncludeFile("Light/LightObject.h");


                obj.AddAction("SetGlobalLight",
                               _("Make a light global"),
                               _("Make a light global or simple."),
                               _("Make _PARAM0_ global : _PARAM1_"),
                               _("Light type"),
                               "CppPlatform/Extensions/lightIcon24.png",
                               "CppPlatform/Extensions/lightIcon16.png")
                    .AddParameter("object", _("Object"), "Light", false)
                    .AddParameter("yesorno", _("Make light global"))
                .codeExtraInformation.SetFunctionName("SetGlobalLight").SetIncludeFile("Light/LightObject.h");


                obj.AddCondition("GlobalLight",
                               _("A light is global"),
                               _("Return true if light is global"),
                               _("_PARAM0_ is a global light"),
                               _("Light type"),
                               "CppPlatform/Extensions/lightIcon24.png",
                               "CppPlatform/Extensions/lightIcon16.png")
                    .AddParameter("object", _("Object"), "Light", false)
                .codeExtraInformation.SetFunctionName("IsGlobalLight").SetIncludeFile("Light/LightObject.h");


/*
                obj.AddAction("Angle",
                               _("Change the angle"),
                               _("Modify the angle of a Text object."),
                               _("Do _PARAM1__PARAM2_ to the angle of _PARAM0_"),
                               _("Rotation"),
                               "CppPlatform/Extensions/lightIcon24.png",
                               "CppPlatform/Extensions/lightIcon16.png",
                               &LightObject::ActAngle)
                    .AddParameter("object", _("Object"), "Light", false)
                    .AddParameter("operator", _("Modification's sign"))
                    .AddParameter("expression", _("Value"))



                obj.AddCondition("Angle",
                               _("Angle d'un objet texte"),
                               _("Test the value of the angle of a text object."),
                               _("The angle of _PARAM0_ is _PARAM1__PARAM2_"),
                               _("Rotation"),
                               "CppPlatform/Extensions/lightIcon24.png",
                               "CppPlatform/Extensions/lightIcon16.png",
                               &LightObject::CondAngle)
                    .AddParameter("object", _("Object"), "Light", false)
                    .AddParameter("relationalOperator", _("Sign of the test"))
                    .AddParameter("expression", _("Value to test"))

*/

                obj.AddExpression("Intensity", _("Intensity"), _("Intensity"), _("Setup"), "CppPlatform/Extensions/lightIcon16.png")
                    .AddParameter("object", _("Object"), "Light", false)
                    .codeExtraInformation.SetFunctionName("GetIntensity").SetIncludeFile("Light/LightObject.h");

                obj.AddExpression("Radius", _("Radius"), _("Radius"), _("Setup"), "CppPlatform/Extensions/lightIcon16.png")
                    .AddParameter("object", _("Object"), "Light", false)
                    .codeExtraInformation.SetFunctionName("GetRadius").SetIncludeFile("Light/LightObject.h");

                obj.AddExpression("Quality", _("Quality"), _("Quality"), _("Setup"), "CppPlatform/Extensions/lightIcon16.png")
                    .AddParameter("object", _("Object"), "Light", false)
                    .codeExtraInformation.SetFunctionName("GetQuality").SetIncludeFile("Light/LightObject.h");


                /*
                obj.AddExpression("Angle", _("Angle"), _("Angle"), _("Light"), "CppPlatform/Extensions/lightIcon16.png", &LightObject::ExpAngle)
                    .AddParameter("object", _("Object"), "Light", false)
*/

                #endif

            }

            {
                AddAutomatism("LightObstacleAutomatism",
                      _("Light obstacle"),
                      _("LightObstacle"),
                      _("Mark the objects as obstacles for Light objects."),
                      "",
                      "CppPlatform/Extensions/lightObstacleIcon32.png",
                      "LightObstacleAutomatism",
                      std::shared_ptr<gd::Automatism>(new LightObstacleAutomatism),
                      std::shared_ptr<gd::AutomatismsSharedData>(new SceneLightObstacleDatas));

            };

        GD_COMPLETE_EXTENSION_COMPILATION_INFORMATION();
    };
};

/**
 * Used by GDevelop to create the extension class
 * -- Do not need to be modified. --
 */
extern "C" ExtensionBase * GD_EXTENSION_API CreateGDExtension() {
    return new Extension;
}
