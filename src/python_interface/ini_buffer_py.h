/**
 * @file ini_buffer_py.h
 * @author Pascal Enderli
 * @date 2020.02.18
 * @brief IniBuffer Enhancement for python types
 */

#ifndef INI_BUFFER_PY_H_
#define INI_BUFFER_PY_H_

#include<pybind11/pybind11.h>
#include"ini_buffer.h"

namespace py = pybind11;

// Enhance IniBuffer template specialization for python specific types.

//------------------------------------------------------------------------------
// Cast Value
template<>
py::str IniBuffer::Section::StringifiedValue::CastValue<py::str>() const
{
    if(type_ == DataType::STRING)
    {
        return py::cast(value_);
    }
    else
    {
        throw INI_EXCEPTION("Stored value is not a std::string.");
    }
}

template<>
py::int_ IniBuffer::Section::StringifiedValue::CastValue<py::int_>() const
{
    try
    {
        if(type_ == DataType::INT)
        {
            return py::cast(std::stoi(value_));
        }
        else
        {
            throw INI_EXCEPTION("Stored value is not an int.");
        }
    }
    catch(const std::invalid_argument& e){throw INI_EXCEPTION("Invalid Argument: " + std::string(e.what()));}
    catch(const std::out_of_range& e){throw INI_EXCEPTION("Out Of Range: " + std::string(e.what()));}
    catch(const IniException& e){throw;}
    catch(const std::exception& e){throw INI_EXCEPTION( std::string(e.what()));}
    catch(...){throw;}
}

template<>
py::float_ IniBuffer::Section::StringifiedValue::CastValue<py::float_>() const
{
    try
    {
        if(type_ == DataType::FLOAT)
        {
            return py::cast(std::stod(value_));
        }
        else
        {
            throw INI_EXCEPTION("Stored value is not a float.");
        }
    }
    catch(const std::invalid_argument& e){throw INI_EXCEPTION("Invalid Argument: " + std::string(e.what()));}
    catch(const std::out_of_range& e){throw INI_EXCEPTION("Out Of Range: " + std::string(e.what()));}
    catch(const IniException& e){throw;}
    catch(const std::exception& e){throw INI_EXCEPTION( std::string(e.what()));}
    catch(...){throw;}
}

template<>
py::bool_ IniBuffer::Section::StringifiedValue::CastValue<py::bool_>() const
{
    try
    {
        if(type_ == DataType::BOOL)
        {
            if(0 == value_.compare("true") ||
               0 == value_.compare("TRUE") ||
               0 == value_.compare("True"))
            {
                return py::cast(true);
            }
            else
            {
                return py::cast(false);
            }
        }
        else
        {
            throw INI_EXCEPTION("Stored value is not a boolean.");
        }
    }
    catch(const std::out_of_range& e){throw INI_EXCEPTION("Out Of Range: " + std::string(e.what()));}
    catch(const IniException& e){throw;}
    catch(const std::exception& e){throw INI_EXCEPTION( std::string(e.what()));}
    catch(...){throw;}
}

//-----------------------------------------------------------------------------
// Stringify

template<>
std::string IniBuffer::Stringify<py::str>(const py::str& value) const
{
    return value.cast<std::string>();
}

template<>
std::string IniBuffer::Stringify<py::int_>(const py::int_& value) const
{

    try
    {
    	int number = value.cast<int>();
    	return std::to_string(number);
    }
    catch(const std::length_error& e){throw INI_EXCEPTION("Stringified value is too long. length_error: "+std::string(e.what()));}
    catch(std::bad_alloc& e){throw INI_EXCEPTION("String bad_alloc: "+std::string(e.what()));}
}

template<>
std::string IniBuffer::Stringify<py::float_>(const py::float_& value) const
{
    try
    {
        float number = value.cast<float>();
    	return std::to_string(number);
    }
    catch(const std::length_error& e){throw INI_EXCEPTION("Stringified value is too long. length_error: "+std::string(e.what()));}
    catch(std::bad_alloc& e){throw INI_EXCEPTION("String bad_alloc: "+std::string(e.what()));}
}

template<>
std::string IniBuffer::Stringify<py::bool_>(const py::bool_& value) const
{
	bool val = value.cast<bool>();
    if(val==true)
    {
        return std::string("true");
    }
    else
    {
        return std::string("false");
    }
}

#endif
