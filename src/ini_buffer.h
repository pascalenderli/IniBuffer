/**
 * @file ini_buffer.h
 * @author Pascal Enderli
 * @date 2020.01.19
 * @brief Configuration file parser, writer and manipulater.
 */

#ifndef INI_BUFFER_H_
#define INI_BUFFER_H_

#include<fstream>
#include<iostream>
#include<map>
#include<stdexcept>
#include"str_manip.h"

//===================================================================================
// Helpers
//-----------------------------------------------------------------------------------
// Logger This logger is only for demonstration purposes.

const bool ini_buffer_logger_on = false;

/// Loggs a message in the format [Line: <line_nr>][Msg: <message>]. Logger is turned off by default.
#define LOG(message) if(ini_buffer_logger_on)do{std::cout<<"[Line: "<<__LINE__<<"][Msg: "<<message<<"]\n";}while(0)

//-----------------------------------------------------------------------------------
// Exception for IniBuffer class errors.

/// Macro for placing Exceptions of type IniException.
#define INI_EXCEPTION(message) IniException(__LINE__, message)

/** Exception type thrown by the IniBuffer class.
*
* @details
* The Error message IniException.what() returns a message in the following format:
* @n [IniBufferException][File: <filename>][Line: <line_nr>][What: <message>]
*/
class IniException: public std::exception
{

    public:
         explicit IniException(const int line, const std::string& message);
         virtual const char* what() const throw();

    protected:
        std::string message_;
};

//===================================================================================
// IniBuffer class declaration

/** Represents the data structure of a classical ini file.
*   @n The class supports reading and writing complete files as well as access and modifying the data members in a very easy way.
*   @n Accessed data is automatically type casted. Supported are ints, floats, strings and bools.
*   @n Errors are reported using IniExceptions. A meaningful error message is provided.
*   @n You can find more info in the read me.
*/
class IniBuffer
{
    private:

    /// Forward declaration for internal data structure section.
    class Section;

    /// Data type alias for the names of the section.
    using section_name_t = std::string;

    /// Data type alias for the [section_name | section_data] map.
    using SectionMap_t = std::map<section_name_t, Section>;

    public:

    /// List of supported data types for internal communication.
    enum DataType
    {
        STRING = 0,
        INT = 10,
        FLOAT = 20,
        BOOL = 40,
        EMPTY = 50
    };

    //===================================================================================
    // Begin of public IniBuffer Interface

    /** Loads a specified ini file and parses it into the RAM.
    *
    *   @param fullfilename The path and filename pointing to the configuration file.
    */
    void LoadFile(const std::string& fullfilename);

    /** Writes the internal buffer state into the specified file.
    *   @details
    *   @n The specified path (not the file) must exist.
    *   @n If an error occurres during parsing or opening the file, an IniException is thrown.
    *
    *   @param fullfilename The path and filename pointing to the configuration file.
    */
    void WriteFile(const std::string& fullfilename) const;

    /// Clears the whole state of the IniBuffer object.
    void Clear() noexcept;

    /** Receive a requested value from the buffer.
    *
    *   @details
    *   The requested data member is automatically casted to the requested type.
    *   @n If the requested type is inconsistent with the value, an IniException is thrown.
    *
    *   @param_t ValueT The type of the requested data member. Supported are {int, float, std::string, bool}
    *
    *   @param section_name Name of the section, in which the value is stored in the ini file.
    *   @param key_name Name of the property key where the value is stored in the ini file.
    *
    *   @return The requested value casted to the specified type ValueT.
    */
    template<typename ValueT>
    ValueT GetValue(const std::string& section_name, const std::string& key_name) const;

    /** Inserts a certain value into the ini file buffer.
    *
    *   @details
    *   If an error occurred during the operation an IniException is thrown.
    *
    *   @param_t ValueT The type of the inserted data member. Supported are {int, float, std::string, bool}
    *
    *   @param section_name Name of the section where the value is stored in the ini file.
    *   @param key_name Name of the property key where the value is stored in the ini file.
    *   @param value The value being inserted by the method.
    */
    template<typename ValueT>
    void AddValue(const std::string& section_name, const std::string& key_name, const ValueT& value);

    // End of public IniBuffer interface
    //===================================================================================

    private:

    class Section
    {
        public:

        /// Constructs a section which is a nested data structure of the IniBuffer.
        Section() noexcept;

        /// Constructs a named IniBuffer section.
        Section(const std::string& section_name) noexcept;

        ~Section() noexcept;

        /** Get the value from a property (key value pair) in the section.
        *
        *   @details
        *   If an error occurred during the operation an IniException is thrown.
        *
        *   @param_t ValueT Data type of the requested value.
        *   @param key_name Key of he requested value.
        */
        template<typename ValueT>
        ValueT GetValue(const std::string& key_name) const;

        /// Returns the name of this section.
        const std::string& GetName() const noexcept;

        /** Adds a new property [key | value pair] to the section.
        *
        *   @details
        *   A new property is added or an existing one updated.
        *
        *   @param key Name of the key where the new value should be stored.
        *   @param value The value which belongs to the key as string. (type is detected at insertion.)
        */
        void AddProperty(const std::string& key, const std::string& value) noexcept;

        /// Writes this section to the file.
        void WriteSection(std::ofstream& file);

        private:

        /// Internal data structure of a single value, which is stored as a string and an auto detected data type.
        class StringifiedValue
        {
            public:

            /// Creates a stringified value.
            StringifiedValue(const std::string& value) noexcept;

            /// Creates an empty stringified value.
            StringifiedValue() noexcept;

            ~StringifiedValue() noexcept;

            /** Get this value casted in the specified type ValueT.
            *
            * @details
            * If the function fails, an IniException is thrown.
            */
            template<typename ValueT>
            ValueT GetValue() const;

            /// Same as GetValue but does not perform typecasting to ValueT.
            const std::string& GetValueAsString() const noexcept;

            private:

            /** Underlaying function which casts the stored strings of the values to the requested data type.
            *
            *   @details
            *   If the cast is not successful an IniBufferException is thrown.
            */
            template<typename ValueT>
            ValueT CastValue() const;

            /// The data type specification of this stringified value.
            DataType type_;

            /// The stored value.
            std::string value_;
        };

        /// Data type alias for keys.
        using KeyName_t = std::string;

        /// Data type alias for the map of properties [Key | Value pairs].
        using PropertyMap_t = std::map<KeyName_t,  StringifiedValue>;

        /// Name of this section.
        std::string name_;

        /// Actual place where the properties (key/value pairs) are stored.
        PropertyMap_t properties_;
    };

    /** Inserting a new section to the IniBuffer.
    *
    *   @details
    *   If already a section with the same name is present in the buffer, the old section will be overwritten.
    */
    void AddSection(const Section& section) noexcept;

    /// Interprets all lines of an ini configuration file and stores its content in the buffer.
    void ParseFile(const std::string& fullfilename);

    /// Adds an empty section (without any properties) to the IniBuffer.
    void AddEmptySection(const std::string& section_name) noexcept;

    /** Converts any supported type into a std::string.
    *
    *   @details
    *   This function throws an IniException if it fails.
    *   @n Supported Types are: {int, bool, float, std::string;}
    */
    template<typename ValueT>
    std::string Stringify(const ValueT& value) const;

    /// A Map of sections with their names [section_name | section_object]
    SectionMap_t sections_;
};

/// Namespace for internal functions which are used by the IniBuffer Class.
namespace internal
{
    /** Finds the data type of a string.
    *
    *   @details
    *   Used internally by the IniBuffer class.
    */
    IniBuffer::DataType GetDataType(const std::string& str) noexcept;
}

/** Gets the current DateTime and returns it in a formatted line.
* @details
* Format is: YYYY/MM/DD hh:mm:ss
*/
static std::string GetDateTime() noexcept;


//===================================================================================
// Implementation

//-----------------------------------------------------------------------------------
// IniException

IniException::IniException(const int line, const std::string& message)
{
    message_ = "[IniBufferException][File: " + std::string(__FILE__)+"][Line: " + std::to_string(line) + "][What: " + message + "]\n";
}

const char* IniException::what() const throw()
{
    return message_.c_str();
}


//-----------------------------------------------------------------------------------
// Stringified Value

IniBuffer::Section::StringifiedValue::StringifiedValue() noexcept
: value_(""),
  type_(DataType::EMPTY)
{}

IniBuffer::Section::StringifiedValue::StringifiedValue(const std::string& value) noexcept
{
    type_ = internal::GetDataType(value);
    value_ = value;
}

template<typename ValueT>
ValueT IniBuffer::Section::StringifiedValue::GetValue() const
{
    try
    {
        return CastValue<ValueT>();
    }
    catch(const std::logic_error& e)
    {
        throw INI_EXCEPTION("Bad Cast: " + std::string(e.what()));
    }
    catch(...)
    {
        throw INI_EXCEPTION("Unknown exception occurred.");
    }
}

const std::string&  IniBuffer::Section::StringifiedValue::GetValueAsString() const noexcept
{
    return value_;
}

template<>
std::string IniBuffer::Section::StringifiedValue::CastValue<std::string>() const
{
    if(type_ == DataType::STRING)
    {
        return value_;
    }
    else
    {
        throw INI_EXCEPTION("Stored value is not a std::string.");
    }
}

template<>
int IniBuffer::Section::StringifiedValue::CastValue<int>() const
{
    try
    {
        if(type_ == DataType::INT)
        {
            return std::stoi(value_);
        }
        else
        {
            throw INI_EXCEPTION("Stored value is not an int.");
        }
    }
    catch(const std::invalid_argument& e){INI_EXCEPTION("Invalid Argument: " + std::string(e.what()));}
    catch(const std::out_of_range& e){INI_EXCEPTION("Out Of Range: " + std::string(e.what()));}
    catch(const IniException& e){throw;}
    catch(const std::exception& e){throw INI_EXCEPTION( std::string(e.what()));}
    catch(...){throw INI_EXCEPTION("Unknown exception occurred.");}
}

template<>
float IniBuffer::Section::StringifiedValue::CastValue<float>() const
{
    try
    {
        if(type_ == DataType::FLOAT)
        {
            return std::stof(value_);
        }
        else
        {
            throw INI_EXCEPTION("Stored value is not a float.");
        }
    }
    catch(const std::invalid_argument& e){INI_EXCEPTION("Invalid Argument: " + std::string(e.what()));}
    catch(const std::out_of_range& e){INI_EXCEPTION("Out Of Range: " + std::string(e.what()));}
    catch(const IniException& e){throw;}
    catch(const std::exception& e){throw INI_EXCEPTION( std::string(e.what()));}
    catch(...){throw INI_EXCEPTION("Unknown exception occurred.");}
}

template<>
bool IniBuffer::Section::StringifiedValue::CastValue<bool>() const
{
    try
    {
        if(type_ == DataType::BOOL)
        {
            if(0 == value_.compare("true") ||
               0 == value_.compare("TRUE") ||
               0 == value_.compare("True"))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            INI_EXCEPTION("Stored value is not a boolean.");
        }
    }
    catch(const std::out_of_range& e){INI_EXCEPTION("Out Of Range: " + std::string(e.what()));}
    catch(const IniException& e){throw;}
    catch(const std::exception& e){throw INI_EXCEPTION( std::string(e.what()));}
    catch(...){throw INI_EXCEPTION("Unknown exception occurred.");}
}

template<typename ValueT>
ValueT IniBuffer::Section::StringifiedValue::CastValue() const
{
     throw INI_EXCEPTION("Stored value is not supported for typecasting.");
}


IniBuffer::Section::StringifiedValue::~StringifiedValue() noexcept
{
    value_ = "";
    type_ = DataType::EMPTY;
}

//-----------------------------------------------------------------------------------
// Section
IniBuffer::Section::Section() noexcept
: name_("")
{
    properties_.clear();
}

IniBuffer::Section::Section(const std::string& name) noexcept
: name_(name)
{
    properties_.clear();
}

const std::string& IniBuffer::Section::GetName() const noexcept
{
    return name_;
}

template<typename ValueT>
ValueT IniBuffer::Section::GetValue(const std::string& key_name) const
{
    if(properties_.find(key_name) != properties_.end())
    {
        try
        {
            return properties_.at(key_name).GetValue<ValueT>();
        }
        catch(const std::out_of_range& e)
        {
              INI_EXCEPTION(std::string("properties_ variable access out of range: ") + e.what());
        }
    }
    else
    {
        throw INI_EXCEPTION("Requested key_name is not present in the data structure: " + key_name);
    }
}

void IniBuffer::Section::AddProperty(const std::string& key, const std::string& value) noexcept
{
    if(properties_.emplace(key, StringifiedValue(value)).second == true)
    {
        return;
    }
    else
    {   // property is already present. -> overwrite.
        properties_.at(key) = value;
    }
}

void IniBuffer::Section::WriteSection(std::ofstream& file)
{
    try
    {
        file<<"["<<name_<<"]\n";
        for(auto item : properties_)
        {
          file<<item.first<<" = "<<item.second.GetValueAsString()<<"\n";
        }
        file<<"\n";
    }
    catch(std::ios_base::failure& e){throw INI_EXCEPTION("Error writing sections to file. Section Name:"+ name_ +"; Message: "+ std::string(e.what()));}
    catch(...){throw INI_EXCEPTION("Unknown Exception occurred").what();}
}

IniBuffer::Section::~Section() noexcept
{
    properties_.clear();
}

//-----------------------------------------------------------------------------------
// IniBuffer

void IniBuffer::LoadFile(const std::string& fullfilename)
{
    try
    {
        return ParseFile(fullfilename);
    }
    catch(IniException& e) {throw;}
    catch(std::exception& e){throw INI_EXCEPTION( e.what());}
    catch(...){throw INI_EXCEPTION("Unknown exception occurred.");}
}

void IniBuffer::WriteFile(const std::string& fullfilename) const
{
    std::ofstream file;
    file.exceptions(std::ofstream::failbit);
    try
    {
        file.open (fullfilename, std::ios::out | std::ios::trunc);
    }
    catch(std::ios_base::failure& e){throw INI_EXCEPTION("Can not open file. Maybe fullfilename is invalid. Filename: "+fullfilename);}
    try
    {
        file << "# Configuration File\n";
        file << "# "<<GetDateTime()<<"\n\n";

        for(auto item : sections_)
        {
            item.second.WriteSection(file);
        }

        file.close();
        LOG("Wrote ini-file to disk: "+fullfilename);
    }
    catch(std::ios_base::failure& e){throw INI_EXCEPTION("Error writing header to file: " + std::string(e.what()));}
    catch(IniException& e){throw;}
    catch(std::exception& e){throw INI_EXCEPTION(e.what());}
    catch(...){throw INI_EXCEPTION("Unknown exception occurred.");}
}

void IniBuffer::Clear() noexcept
{
    sections_.clear();
    LOG("Cleared IniBuffer.");
}

template<typename ValueT>
ValueT IniBuffer::GetValue(const std::string& section_name, const std::string& key_name) const
{
    if(sections_.find(section_name) != sections_.end())
    {
        // std::out_of_range exception is impossible because of check in "if"
        return sections_.at(section_name).GetValue<ValueT>(key_name);
    }
    else
    {
        throw INI_EXCEPTION("Requested Section Name is not present in the data structure. Requested Name: " + section_name);
    }
}

template<typename ValueT>
void IniBuffer::AddValue(const std::string& section_name, const std::string& key_name, const ValueT& value)
{
    std::string stringified_value;
    try
    {
        stringified_value = Stringify<ValueT>(value);
    }
    catch(IniException& e){throw;}
    catch(std::exception& e){throw INI_EXCEPTION(e.what());}
    catch(...){throw INI_EXCEPTION("Unknown exception occurred.");}

    if(sections_.find(section_name)!=sections_.end())
    {
        sections_[section_name].AddProperty(key_name, stringified_value);
    }
    else
    {
        Section section(section_name);
        section.AddProperty(key_name, stringified_value);
        AddSection(section);
    }
}

void IniBuffer::AddEmptySection(const std::string& section_name) noexcept
{
    Section section(section_name);
    AddSection(section);
}

void IniBuffer::AddSection(const Section& section) noexcept
{
    if(!sections_.emplace(section.GetName(), section).second)
    { // Section already present -> overwrite
        sections_.at(section.GetName()) = section;
    }
}

void IniBuffer::ParseFile(const std::string& fullfilename)
{
    std::ifstream fstrm;

    fstrm.exceptions(std::ifstream::failbit);

    std::string line;
    std::string current_section_name;
    size_t pos = 0;
    size_t line_nr = 0;

    try
    {
        fstrm.open(fullfilename);
    }
    catch(std::ios_base::failure& e)
    {
        throw INI_EXCEPTION("Opening file failed: " + fullfilename);
    }

    while(true)
    {
        ++line_nr;

        try
        {
            getline(fstrm, line);
        }
        catch(std::ios_base::failure& e)
        {
            if(fstrm.eof()){LOG("Reached end of ini file."); break;}
            else {throw;}
        }

        TrimComment(line);
        TrimWhitespace(line);

        // Empty line
        if (line.empty())
        {
            continue;
        }

        // Comment
        if (line[0] == ';' || line[0] == '#')
        {
            continue;
        }

        // Section
        else if (line[0] == '[')
        {
            pos = line.find_first_of(']');
            if (pos == std::string::npos)
            {
                throw INI_EXCEPTION("Ivalid ini-file ] is missing in section. Line: " + std::to_string(line_nr));
            }
            else
            {
                current_section_name = line.substr(1, pos - 1);
                TrimWhitespace(current_section_name);
                AddEmptySection(current_section_name);
            }
        }

        // Property
        else if((pos = line.find_first_of("=")) != std::string::npos)
        {
            if(current_section_name.empty())
            {
                throw INI_EXCEPTION("Invalid ini-file. Property must belong to a section. Line: " + std::to_string(line_nr));
            }
            std::string key_name = line.substr(0, pos);
            std::string value = "";
            try
            {
                value = line.substr(pos + 1);
                TrimWhitespace(key_name);
                TrimWhitespace(value);
            }
            catch(const std::out_of_range& e){throw INI_EXCEPTION("Invalid ini-file. Value is missing. Line: " + std::to_string(line_nr));}

            if(ContainsWhiteSpace(key_name))
            {
                throw INI_EXCEPTION("Invalid ini-file. Key contains white spaces: " + std::to_string(line_nr));
            }
            try
            {
                AddValue(current_section_name, key_name, value);
            }
            catch(IniException& e){throw;}
            catch(std::exception& e){throw INI_EXCEPTION(e.what());}
            catch(...){throw INI_EXCEPTION("Unknown exception occurred.");}
        }
        else
        {
            throw INI_EXCEPTION("Invalid ini-file. Line is not a valid ini-file component: " + std::to_string(line_nr));
        }
    }
}

template<>
std::string IniBuffer::Stringify<std::string>(const std::string& value) const
{
    return value;
}

template<>
std::string IniBuffer::Stringify<int>(const int& value) const
{
    try {return std::to_string(value);}
    catch(const std::length_error& e){throw INI_EXCEPTION("Stringified value is too long. length_error: "+std::string(e.what()));}
    catch(std::bad_alloc& e){throw INI_EXCEPTION("String bad_alloc: "+std::string(e.what()));}
}

template<>
std::string IniBuffer::Stringify<float>(const float& value) const
{
    try {return std::to_string(value);}
    catch(const std::length_error& e){throw INI_EXCEPTION("Stringified value is too long. length_error: "+std::string(e.what()));}
    catch(std::bad_alloc& e){throw INI_EXCEPTION("String bad_alloc: "+std::string(e.what()));}
}

// Booleans are stored as string and identified later.
template<typename ValueT>
std::string IniBuffer::Stringify(const ValueT& value) const
{
    throw INI_EXCEPTION("The Requestet Datatype is not supported to stringify.");
}

std::string GetDateTime() noexcept
{
    time_t now = time(0);
    struct tm  time_struct;
    char buf[80];
    time_struct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y/%m/%d %X", &time_struct);
    return buf;
}

//-----------------------------------------------------------------------------------
// Helper

namespace internal
{
    IniBuffer::DataType GetDataType(const std::string& str) noexcept
    {
        std::string::size_type start_pos = 0;
        if(start_pos = str.find_first_not_of(" ", start_pos) == std::string::npos)
        {
            return IniBuffer::DataType::EMPTY;
        }

        if(0 == str.compare("true") ||
           0 == str.compare("TRUE") ||
           0 == str.compare("True") ||
           0 == str.compare("false") ||
           0 == str.compare("FALSE") ||
           0 == str.compare("False"))
        {
            return IniBuffer::DataType::BOOL;
        }

        if((str[start_pos] == '-') || str[start_pos] == '+')
        {
            start_pos = 1;
        }

        if(str.find_first_not_of("0123456789.", start_pos) != std::string::npos)
        {
             return IniBuffer::DataType::STRING;
        }
        else if(str.find_first_of(".", start_pos) == std::string::npos)
        {
            return IniBuffer::DataType::INT;
        }
        else
        {
            return IniBuffer::DataType::FLOAT;
        }
    }
}
#endif
