/*************************
 *
 * 
 *
 *************************/
#ifndef INI_READER__
#define INI_READER__

#include <iostream>
#include <vector>
#include <map>

class IniReader
{
  public:
    typedef  std::map <std::string, std::string> StringMap;
    typedef  std::map <std::string, StringMap> IniMap;

    IniReader ()
     : m_values ()
    {}

    bool parse (const std::string & ini_file_name);

    const IniMap & get_conf () const
    {
      return m_values;
    }

    bool get_value (const std::string & section, const std::string & attribute, std::string & value) const;

  private:

    // trim from both ends
    std::string & trim (std::string &s);

    IniMap          m_values;
    std::string     m_ini_file_name;
};
#endif


