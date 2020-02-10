/**
 * @file IniFile.h
 * @author Pascal Enderli
 * @date 2020.01.19
 * @brief Configuration file parser and writer.
 */

#include<algorithm>
#include<stdexcept>
#include<fstream>
#include<iostream>
#include<map>
#include<string>
#include<time.h>   // timer

#include <stdio.h> // datetime
#include <time.h>  // datetime

//===================================================================================
// Helpers
unsigned int THRESH = 0;
#define LOG(level, message) if(level>THRESH)do{std::cout<<message<<"\n";}while(0)
#define INI_EXCEPTION(message) IniException(__LINE__, message) 

class IniException: public std::exception
{
   
    public:
         explicit IniException(const int line, const std::string& message);
         virtual const char* what() const throw();

    protected:
        std::string message_;
};

enum DataType
{
    STRING = 0,
    INT = 10,
    FLOAT = 20,
    BOOL = 40,
    EMPTY = 50
};

static void LTrim(std::string& s) noexcept
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
    [](int ch){ return !std::isspace(ch);}
    ));
}

static void RTrim(std::string& s) noexcept
{
    s.erase(std::find_if(s.rbegin(), s.rend(),
    [](int ch) { return !std::isspace(ch);}
    ).base(), s.end());
}

// TrimWhitespace whitespace
static void TrimWhitespace(std::string& s) noexcept
{
    LTrim(s);
    RTrim(s);
}

// Trim Comments
static void TrimComment(std::string& s) noexcept
{
    size_t pos = s.find_first_of(";#");
    if(pos != std::string::npos)
    {
        s.erase(pos,  std::string::npos);
    }
}

static bool ContainsWhiteSpace(const std::string& str) noexcept
{
    return str.find(" ") != std::string::npos;
}

static DataType GetDataType(const std::string& str) noexcept
{
    std::string::size_type start_pos = 0;
    if(start_pos = str.find_first_not_of(" ", start_pos) == std::string::npos)
    {
        return DataType::EMPTY;
    }

    if(0 == str.compare("true") ||
       0 == str.compare("TRUE") ||
       0 == str.compare("True") ||
       0 == str.compare("false") ||
       0 == str.compare("FALSE") ||
       0 == str.compare("False"))
    {
        return DataType::BOOL;
    }

    if((str[start_pos] == '-') || str[start_pos] == '+')
    {
        start_pos = 1;
    }

    if(str.find_first_not_of("0123456789.", start_pos) != std::string::npos)
    {
         return DataType::STRING;
    }
    else if(str.find_first_of(".", start_pos) == std::string::npos)
    {
        return DataType::INT;
    }
    else
    {
        return DataType::FLOAT;
    }
}

static std::string GetDateTime() noexcept
{
    time_t now = time(0);
    struct tm  time_struct;
    char buf[80];
    time_struct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y/%m/%d %X", &time_struct);
    return buf;
}
//===================================================================================
// IniFile Interface

class IniFile
{
    class Section;
    using SectionName_t = std::string;
    using SectionMap_t = std::map<SectionName_t, Section>;

    public:
    void LoadFile(const std::string& fullfilename);
    void WriteFile(const std::string& fullfilename) const;
    void Clear() noexcept;

    template<typename ValueT>
    ValueT GetValue(const std::string& section_name, const std::string& key_name) const;
    
    template<typename ValueT>
    void AddValue(const std::string& section_name, const std::string& key_name, const ValueT& value);

    void AddEmptySection(const std::string& section_name) noexcept;

    private:
    class Section
    {
        public:
        Section() noexcept;

        Section(const std::string& section_name) noexcept;

        ~Section() noexcept;

        template<typename ValueT>
        ValueT GetValue(const std::string& key_name) const;

        const std::string& GetName() const noexcept;

        // overwrites if already exists.
        void AddProperty(const std::string& key, const std::string& value) noexcept;

        void WriteSection(std::ofstream& file);

        private:
        class StringifiedValue
        {
            public:
            StringifiedValue(const std::string& value) noexcept;
            StringifiedValue() noexcept;

            ~StringifiedValue() noexcept;

            template<typename ValueT>
            ValueT GetValue() const;

            const std::string& GetValueAsString() const noexcept;

            private:
            template<typename ValueT>
            ValueT CastValue() const;

            DataType type_;
            std::string value_;
        };

        using KeyName_t = std::string;
        using PropertyMap_t = std::map<KeyName_t,  StringifiedValue>;

        std::string name_;
        PropertyMap_t properties_;
    };
    // overwrites if already exists.
    void AddSection(const Section& section) noexcept;
    void ParseFile(const std::string& fullfilename);

    template<typename ValueT>
    std::string Stringify(const ValueT& value) const;

    SectionMap_t sections_;
};
//===================================================================================

// Implementation

//-----------------------------------------------------------------------------------
// IniException

IniException::IniException(const int line, const std::string& message)
{
    message_ = "[IniFileException][File: " + std::string(__FILE__)+"][Line: " + std::to_string(line) + "][What: " + message + "]\n";
}

const char* IniException::what() const throw()
{
    return message_.c_str();
}

//-----------------------------------------------------------------------------------
// Stringified Value

IniFile::Section::StringifiedValue::StringifiedValue() noexcept
: value_(""),
  type_(DataType::EMPTY)
{}

IniFile::Section::StringifiedValue::StringifiedValue(const std::string& value) noexcept
{
    type_ = GetDataType(value);
    value_ = value;
}

template<typename ValueT>
ValueT IniFile::Section::StringifiedValue::GetValue() const
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

const std::string&  IniFile::Section::StringifiedValue::GetValueAsString() const noexcept
{
    return value_;
}

template<>
std::string IniFile::Section::StringifiedValue::CastValue<std::string>() const
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
int IniFile::Section::StringifiedValue::CastValue<int>() const
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
float IniFile::Section::StringifiedValue::CastValue<float>() const
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
bool IniFile::Section::StringifiedValue::CastValue<bool>() const
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
ValueT IniFile::Section::StringifiedValue::CastValue() const
{
     throw INI_EXCEPTION("Stored value is not supported for typecasting.");
}


IniFile::Section::StringifiedValue::~StringifiedValue() noexcept
{
    value_ = "";
    type_ = DataType::EMPTY;
}

//-----------------------------------------------------------------------------------
// Section
IniFile::Section::Section() noexcept
: name_("")
{
    properties_.clear();
}

IniFile::Section::Section(const std::string& name) noexcept
: name_(name)
{
    properties_.clear();
}

const std::string& IniFile::Section::GetName() const noexcept
{
    return name_;
}

template<typename ValueT>
ValueT IniFile::Section::GetValue(const std::string& key_name) const
{
    if(properties_.find(key_name) != properties_.end())
    {   
        try
        {
            return properties_.at(key_name).GetValue<ValueT>();
        }
        catch(const std::out_of_range& e)
        {
              LOG(1, std::string("Out of Range: ") + e.what());
        }
    }
    else
    {
        throw INI_EXCEPTION("Requested key_name is not present in the data structure: " + key_name);
    }
}

void IniFile::Section::AddProperty(const std::string& key, const std::string& value) noexcept
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

void IniFile::Section::WriteSection(std::ofstream& file)
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

IniFile::Section::~Section() noexcept
{
    properties_.clear();
}

//-----------------------------------------------------------------------------------
// IniFile

void IniFile::LoadFile(const std::string& fullfilename)
{
    try
    {
        return ParseFile(fullfilename);
    }
    catch(IniException& e) {throw;}
    catch(std::exception& e){throw INI_EXCEPTION( e.what());}
    catch(...){throw INI_EXCEPTION("Unknown exception occurred.");}
}

void IniFile::WriteFile(const std::string& fullfilename) const
{
    std::ofstream file;
    file.exceptions(std::ofstream::failbit);
    try
    {
        file.open (fullfilename, std::ios::out | std::ios::trunc);
    }
    catch(std::ios_base::failure& e){throw INI_EXCEPTION("Can not open file. Maybe fullfilename is invalid.");}
    try
    {
        file << "# Configuration File\n";
        file << "# "<<GetDateTime()<<"\n\n";

        for(auto item : sections_)
        {
            item.second.WriteSection(file);
        }

        file.close();
    }
    catch(std::ios_base::failure& e){throw INI_EXCEPTION("Error writing header to file: " + std::string(e.what()));}
    catch(IniException& e){throw;}
    catch(std::exception& e){throw INI_EXCEPTION(e.what());}
    catch(...){throw INI_EXCEPTION("Unknown exception occurred.");}
}

void IniFile::Clear() noexcept
{
    sections_.clear();
}

template<typename ValueT>
ValueT IniFile::GetValue(const std::string& section_name, const std::string& key_name) const
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
void IniFile::AddValue(const std::string& section_name, const std::string& key_name, const ValueT& value)
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

void IniFile::AddEmptySection(const std::string& section_name) noexcept
{
    Section section(section_name);
    AddSection(section);
}

//write new! Try to emplace a section to the section list
void IniFile::AddSection(const Section& section) noexcept
{
    if(!sections_.emplace(section.GetName(), section).second)
    { // Section already present -> overwrite
        LOG(1, "Overwritten duplicated section: " + section.GetName());
        sections_.at(section.GetName()) = section;
    }
}

void IniFile::ParseFile(const std::string& fullfilename)
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
            if(fstrm.eof()){LOG(1, "ini-file: Reached end of file."); break;}
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
std::string IniFile::Stringify<std::string>(const std::string& value) const
{
    return value;
}

template<>
std::string IniFile::Stringify<int>(const int& value) const
{
    try {return std::to_string(value);}
    catch(const std::length_error& e){throw INI_EXCEPTION("Stringified value is too long. length_error: "+std::string(e.what()));}
    catch(std::bad_alloc& e){throw INI_EXCEPTION("String bad_alloc: "+std::string(e.what()));}
}

template<>
std::string IniFile::Stringify<float>(const float& value) const
{
    try {return std::to_string(value);}
    catch(const std::length_error& e){throw INI_EXCEPTION("Stringified value is too long. length_error: "+std::string(e.what()));}
    catch(std::bad_alloc& e){throw INI_EXCEPTION("String bad_alloc: "+std::string(e.what()));}
}

// Booleans are stored as string and identified later.
template<typename ValueT>
std::string IniFile::Stringify(const ValueT& value) const
{
    throw INI_EXCEPTION("The Requestet Datatype is not supported to stringify.");
}

//===================================================================================
// Main
int main()
{
    //-----------------------------------------------------------------------------------
    // Configuration

    bool run_test = true;
    bool run_benchmark = true;
    //-----------------------------------------------------------------------------------
    // Simple Test

    if(run_test)
    {
        IniFile ini;
        ini.LoadFile("test.ini");
        ini.AddValue("aded_test_section", "key1", std::string("text1"));
        ini.WriteFile("test_copy.ini");

        try
        {
            bool c1 = ini.GetValue<bool>("06_booleans", "key01");
            bool c2 = ini.GetValue<bool>("06_booleans", "key02");
            bool c3 = ini.GetValue<bool>("06_booleans", "key03");
            std::cout<<"06_booleans\n"<<"key01: "<<c1<<"\n"<<"key02: "<<c2<<"\n"<<"key03: "<<c3<<"\n\n";

            int d1 = ini.GetValue<int>("04_integers", "key01");
            int d2 = ini.GetValue<int>("04_integers", "key11");
            int d3 = ini.GetValue<int>("04_integers", "key26");
            std::cout<<"04_integers\n"<<"key01: "<<d1<<"\n"<<"key11: "<<d2<<"\n"<<"key26: "<<d3<<"\n\n";
         }
        catch(IniException& e)
        {
            LOG(1, e.what());
            abort();
        }
    }
    //-----------------------------------------------------------------------------------
    // Benchmark

    if(run_benchmark)
    {   
        THRESH = 10;
        clock_t t;
        clock_t dt;
        for(int i = 1; i<=1000; ++i)
        {
          dt = clock();
          IniFile ini_performance;
          ini_performance.LoadFile("performance.ini");
          ini_performance.WriteFile("performance2.ini");
          dt = clock()-dt;
          t = t + (dt-t)/i;
          ini_performance.Clear();
        }
        std::cout<<"Average parsing time: "<<1000*((float)t)/(CLOCKS_PER_SEC)<<" ms\n";
    }
    return 0;
}

