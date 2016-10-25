#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include "IniReader.h"

#define TRACE( x ) std::cout << x << std::endl;
#define PAR( x ) #x << "=\"" << x << "\" "

bool IniReader::parse (const std::string & ini_file_name)
{
  TRACE ("Parsing " << ini_file_name);
  m_values.clear ();
  m_ini_file_name = ini_file_name;
  std::ifstream ini_file (ini_file_name.c_str ());
  if (!ini_file)
    return false;

  std::string line, key, value, section ("Default");
  size_t line_number = 0;
  bool ok = true;
  while (ok && std::getline (ini_file, line))
  {
    ++line_number;
    if (!line.empty () && '#' == * line.begin ())
    {
      TRACE ("comment found : " << line);
      continue;
    }
      
    size_t sec_start = line.find ("[");
    if (std::string::npos != sec_start)
    { //parse section name
      size_t sec_end = line.find ("]");
      if (std::string::npos == sec_end)
        TRACE ("Ini parse error - cant find \"]\" at line " << line_number);
      section = line.substr (sec_start + 1, sec_end - 1);
      TRACE ("Parsing [" << section << "] section");
      m_values [section] = StringMap ();
      continue;
     }

    if (std::string::npos != (sec_start = line.find ("=")))
    {   
      std::stringstream ss (line);
      key = value = "";
      //ss >> key >> value;
      key = line.substr (0, sec_start -1);
      trim (key);

      value = line.substr (sec_start +1, line.size ());
      trim (value);

      TRACE (PAR (key) << " = " PAR (value));
      if (m_values.end () == m_values.find (key))
        m_values[section].insert (StringMap::value_type (key, value));
      else
        ok = false;
    }
  }

  return true;
} //parse


bool IniReader::get_value (const std::string & section, const std::string & attribute, std::string & value) const
{
  IniMap::const_iterator cfg_it = m_values.find (section);
  if (m_values.end () == cfg_it)
  {
    TRACE ("[WARNING] File \"" << m_ini_file_name << "\" doesn't contain section [" << section << "]");
    return false;
  }

  StringMap::const_iterator section_it = cfg_it->second.find (attribute);
  if (cfg_it->second.end () == section_it)
  {
    TRACE ("[WARNING] File \"" << m_ini_file_name << "\" in section [" << section << "] doesn't contain attribute \"" << attribute << "\"");
    return false;
  }

  value = section_it->second;
  return true;
} //get_value

// trim from start
inline std::string & ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
inline std::string & rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
std::string & IniReader::trim (std::string &s)
{
    return ltrim(rtrim(s));
}

