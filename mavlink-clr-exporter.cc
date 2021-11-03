
#using <mscorlib.dll>
#using <System.dll>
#using <System.ComponentModel.dll>
#using <System.Reflection.Emit.dll>

#include <msclr/marshal.h>

#include <stddef.h>
#include <stdint.h>

#pragma warning(push)
# pragma warning(disable:4201)
# pragma warning(disable:4244)
#  include <standard/mavlink.h>
#pragma warning(pop)

#include <vector>
#include <algorithm>

using msclr::interop::marshal_as;
using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Reflection;
using namespace System::Reflection::Emit;


int main() {
    using namespace System;
    using namespace System::Reflection::Emit;

    auto assembly_name = gcnew AssemblyName("mavlink-clr");
    auto assembly_builder = AppDomain::CurrentDomain->DefineDynamicAssembly(
        assembly_name,
        AssemblyBuilderAccess::RunAndSave);
    auto module_builder = assembly_builder->DefineDynamicModule(
        assembly_name->Name,
        assembly_name->Name + ".dll");

    static const auto emit_message_info = [](mavlink_message_info_t const& message_info,
                                             ModuleBuilder^ module_builder) {
        auto type_builder = module_builder->DefineType(
            "MAV2." + marshal_as<String^>(message_info.name),
            TypeAttributes::Public,
            ValueType::typeid,
            PackingSize::Size1);
        std::vector<mavlink_field_info_t const*> field_ptr_list;
        for (unsigned i = 0; i < message_info.num_fields; ++i) {
            field_ptr_list.push_back(&message_info.fields[i]);
        }
        std::stable_sort(field_ptr_list.begin(),
                         field_ptr_list.end(),
                         [](auto const lhs, auto const rhs) -> bool {
                             return lhs->structure_offset < rhs->structure_offset;
                         });
        for (auto field_ptr : field_ptr_list) {
            auto const& field_info = *field_ptr;
            auto name = marshal_as<String^>(field_info.name);
            if (auto length = field_info.array_length) {
                auto type = [&field_info]() {
                    switch (field_info.type) {
                    case MAVLINK_TYPE_CHAR    : return array<char    >::typeid;
                    case MAVLINK_TYPE_UINT8_T : return array<uint8_t >::typeid;
                    case MAVLINK_TYPE_INT8_T  : return array<int8_t  >::typeid;
                    case MAVLINK_TYPE_UINT16_T: return array<uint16_t>::typeid;
                    case MAVLINK_TYPE_INT16_T : return array<int16_t >::typeid;
                    case MAVLINK_TYPE_UINT32_T: return array<uint32_t>::typeid;
                    case MAVLINK_TYPE_INT32_T : return array<int32_t >::typeid;
                    case MAVLINK_TYPE_UINT64_T: return array<uint64_t>::typeid;
                    case MAVLINK_TYPE_INT64_T : return array<int64_t >::typeid;
                    case MAVLINK_TYPE_FLOAT   : return array<float   >::typeid;
                    case MAVLINK_TYPE_DOUBLE  : return array<double  >::typeid;
                    }
                    throw gcnew Exception("bad mavlink field type");
                }();
                auto field_builder = type_builder->DefineField(name + "_",
                                                               type,
                                                               FieldAttributes::Private);
#pragma warning(push)
# pragma warning(disable:4947)
                field_builder->SetMarshal(UnmanagedMarshal::DefineByValArray(length));
#pragma warning(pop)
                auto property_builder = type_builder->DefineProperty(
                    name,
                    PropertyAttributes::HasDefault,
                    type,
                    nullptr);
                auto accessor_attributes =
                    MethodAttributes::Public |
                    MethodAttributes::SpecialName |
                    MethodAttributes::HideBySig;
                auto getter_builder = type_builder->DefineMethod("get_" + name,
                                                                 accessor_attributes,
                                                                 type,
                                                                 nullptr);
                auto getter_il = getter_builder->GetILGenerator();
                getter_il->Emit(OpCodes::Ldarg_0);
                getter_il->Emit(OpCodes::Ldfld, field_builder);
                getter_il->Emit(OpCodes::Ret);
                auto setter_builder = type_builder->DefineMethod("set_" + name,
                                                                 accessor_attributes,
                                                                 nullptr,
                                                                 gcnew array<Type^>{ type });
                auto setter_il = setter_builder->GetILGenerator();
                setter_il->Emit(OpCodes::Ldarg_0);
                setter_il->Emit(OpCodes::Ldarg_1);
                setter_il->Emit(OpCodes::Stfld, field_builder);
                setter_il->Emit(OpCodes::Ret);
                property_builder->SetGetMethod(getter_builder);
                property_builder->SetSetMethod(setter_builder);
            }
            else {
                auto type = [&field_info]() {
                    switch (field_info.type) {
                    case MAVLINK_TYPE_CHAR    : return char    ::typeid;
                    case MAVLINK_TYPE_UINT8_T : return uint8_t ::typeid;
                    case MAVLINK_TYPE_INT8_T  : return int8_t  ::typeid;
                    case MAVLINK_TYPE_UINT16_T: return uint16_t::typeid;
                    case MAVLINK_TYPE_INT16_T : return int16_t ::typeid;
                    case MAVLINK_TYPE_UINT32_T: return uint32_t::typeid;
                    case MAVLINK_TYPE_INT32_T : return int32_t ::typeid;
                    case MAVLINK_TYPE_UINT64_T: return uint64_t::typeid;
                    case MAVLINK_TYPE_INT64_T : return int64_t ::typeid;
                    case MAVLINK_TYPE_FLOAT   : return float   ::typeid;
                    case MAVLINK_TYPE_DOUBLE  : return double  ::typeid;
                    }
                    throw gcnew Exception("bad mavlink field type");
                }();
                auto field_builder = type_builder->DefineField(name + "_",
                                                               type,
                                                               FieldAttributes::Private);
                auto property_builder = type_builder->DefineProperty(name,
                                                                     PropertyAttributes::HasDefault,
                                                                     type,
                                                                     nullptr);
                auto accessor_attributes =
                    MethodAttributes::Public |
                    MethodAttributes::SpecialName |
                    MethodAttributes::HideBySig;
                auto getter_builder = type_builder->DefineMethod("get_" + name,
                                                                 accessor_attributes,
                                                                 type,
                                                                 nullptr);
                auto getter_il = getter_builder->GetILGenerator();
                getter_il->Emit(OpCodes::Ldarg_0);
                getter_il->Emit(OpCodes::Ldfld, field_builder);
                getter_il->Emit(OpCodes::Ret);
                auto setter_builder = type_builder->DefineMethod("set_" + name,
                                                                 accessor_attributes,
                                                                 nullptr,
                                                                 gcnew array<Type^>{ type });
                auto setter_il = setter_builder->GetILGenerator();
                setter_il->Emit(OpCodes::Ldarg_0);
                setter_il->Emit(OpCodes::Ldarg_1);
                setter_il->Emit(OpCodes::Stfld, field_builder);
                setter_il->Emit(OpCodes::Ret);
                property_builder->SetGetMethod(getter_builder);
                property_builder->SetSetMethod(setter_builder);
            }
        }
        type_builder->CreateType();
    };

    mavlink_message_info_t message_info_list[] = MAVLINK_MESSAGE_INFO;
    for (auto const& message_info : message_info_list) {
        emit_message_info(message_info, module_builder);
    }

    assembly_builder->Save(assembly_name->Name + ".dll");
}
