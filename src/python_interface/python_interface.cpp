/**
 * @file python_interface.cpp
 * @author Pascal Enderli
 * @date 2020.01.14
 * @brief Python binding for IniBuffer.
 */

#include<pybind11/pybind11.h>
#include"ini_buffer_py.h"

namespace py = pybind11;

PYBIND11_MODULE(IniBuffer, m)
{
//-----------------------------------------------------------------------------------------------------------------------------------------
// Module Docstring

    m.doc() = R"pbdoc(
        This module exposes an interface to work with an ini-configuration file.
        There are functionalities to read or write a file or to manipulate the content in the buffer.
        The buffer is developed by considering user friendliness, simplicity performance, platform independence and proper error handling.
        )pbdoc";
//-----------------------------------------------------------------------------------------------------------------------------------------
// Class IniBuffer

    py::class_<IniBuffer>(m, "IniBuffer")

//-----------------------------------------------------------------------------------------------------------------------------------------
// Constructor

        .def(py::init<>())

//-----------------------------------------------------------------------------------------------------------------------------------------
// Load File

        .def("LoadFile", &IniBuffer::LoadFile,
        R"pbdoc(
        Loads a specified ini-file and parses it into the buffer.

        Args:
        fullfilename: The path and filename pointing to the configuration file.)pbdoc",
        py::arg("fullfilename")
        )

        .def("WriteFile", &IniBuffer::WriteFile,
        R"pbdoc(
        Writes the internal buffer state into the specified file.

        Details:
        The specified path (not the file) must exist.
        If an error occurres during parsing or opening the file, an IniException is thrown.

        Args:
        fullfilename: The path and filename pointing to the configuration file.)pbdoc",
        py::arg("fullfilename")
        )

//-----------------------------------------------------------------------------------------------------------------------------------------
// Clear

        .def("Clear", &IniBuffer::Clear,
        R"pbdoc(Clears the entire state of the IniBuffer object.)pbdoc")

//-----------------------------------------------------------------------------------------------------------------------------------------
// Erase Section

        .def("EraseSection", &IniBuffer::EraseSection,
        R"pbdoc(Deletes a whole section in the buffer.

        Args:
        section_name: The name of the section to be deleted.

        )pbdoc",
        py::arg("section_name")
        )

//-----------------------------------------------------------------------------------------------------------------------------------------
// Erase Property

        .def("EraseProperty", &IniBuffer::EraseProperty,
        R"pbdoc(Deletes a key/value pair in the buffer.

        Args:
        section_name: Specifier of the section to which the property belongs to.
        property_key: Specifier of the property to be deleted.
        )pbdoc",
        py::arg("section_name"),
        py::arg("property_key")
        )

//-----------------------------------------------------------------------------------------------------------------------------------------
// GetValue<int>

        .def("GetValue_int", &IniBuffer::GetValue<int>,
        R"pbdoc(Receive a requested value from the buffer.

        Details:
        If the requested type is inconsistent with the value, an IniException is thrown.

        Args:
        section_name: Specifier of the section to which the value belongs to.
        key_name: Corresponding value key.

        Return:
        The requested value.

        )pbdoc",
        py::arg("section_name"),
        py::arg("key_name")
        )

//-----------------------------------------------------------------------------------------------------------------------------------------
// GetValue<double>

        .def("GetValue_float", &IniBuffer::GetValue<py::float_>,
        R"pbdoc(Receive a requested value from the buffer.

        Details:
        If the requested type is inconsistent with the value, an IniException is thrown.

        Args:
        section_name: Specifier of the section to which the value belongs to.
        key_name: Corresponding value key.

        Return:
        The requested value.

        )pbdoc",
        py::arg("section_name"),
        py::arg("key_name")
        )

//-----------------------------------------------------------------------------------------------------------------------------------------
// GetValue<bool>

        .def("GetValue_bool", &IniBuffer::GetValue<bool>,
        R"pbdoc(Receive a requested value from the buffer.

        Details:
        If the requested type is inconsistent with the value, an IniException is thrown.

        Args:
        section_name: Specifier of the section to which the value belongs to.
        key_name: Corresponding value key.

        Return:
        The requested value.

        )pbdoc",
        py::arg("section_name"),
        py::arg("key_name")
        )

//-----------------------------------------------------------------------------------------------------------------------------------------
// GetValue<std::string>

        .def("GetValue_string", &IniBuffer::GetValue<std::string>,
        R"pbdoc(Receive a requested value from the buffer.

        Details:
        If the requested type is inconsistent with the value, an IniException is thrown.

        Args:
        section_name: Specifier of the section to which the value belongs to.
        key_name: Corresponding value key.

        Return:
        The requested value.

        )pbdoc",
        py::arg("section_name"),
        py::arg("key_name")
        )

//-----------------------------------------------------------------------------------------------------------------------------------------
// AddValue<int>

        .def("AddValue", &IniBuffer::AddValue<int>,
        R"pbdoc(Inserts a certain value into the ini file buffer.
        Details:
        If an error occurred during the operation an IniException is thrown.

        Args:
        section_name: Name of the section where the value is stored in the ini-file.
        key_name: Name of the property key where the value is stored in the ini-file.
        value: The value being inserted by the method.

        )pbdoc",
        py::arg("section_name"),
        py::arg("key_neame"),
        py::arg("value")
        )

//-----------------------------------------------------------------------------------------------------------------------------------------
// AddValue<float>

        .def("AddValue", &IniBuffer::AddValue<float>,
        R"pbdoc(Inserts a certain value into the ini file buffer.
        Details:
        If an error occurred during the operation an IniException is thrown.

        Args:
        section_name: Name of the section where the value is stored in the ini-file.
        key_name: Name of the property key where the value is stored in the ini-file.
        value: The value being inserted by the method.

        )pbdoc",
        py::arg("section_name"),
        py::arg("key_neame"),
        py::arg("value")
        )

//-----------------------------------------------------------------------------------------------------------------------------------------
// AddValue<bool>

        .def("AddValue", &IniBuffer::AddValue<bool>,
        R"pbdoc(Inserts a certain value into the ini file buffer.
        Details:
        If an error occurred during the operation an IniException is thrown.

        Args:
        section_name: Name of the section where the value is stored in the ini-file.
        key_name: Name of the property key where the value is stored in the ini-file.
        value: The value being inserted by the method.

        )pbdoc",
        py::arg("section_name"),
        py::arg("key_neame"),
        py::arg("value")
        )

//-----------------------------------------------------------------------------------------------------------------------------------------
// AddValue<std::string>

        .def("AddValue", &IniBuffer::AddValue<std::string>,
        R"pbdoc(Inserts a certain value into the ini file buffer.
        Details:
        If an error occurred during the operation an IniException is thrown.

        Args:
        section_name: Name of the section where the value is stored in the ini-file.
        key_name: Name of the property key where the value is stored in the ini-file.
        value: The value being inserted by the method.

        )pbdoc",
        py::arg("section_name"),
        py::arg("key_neame"),
        py::arg("value")
        );

    m.attr("__version__") = "dev";
}
