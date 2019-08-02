#include "UserReader.hpp"

namespace Mud
{
namespace World
{

void NewUserState::HandleLine(std::istream &line)
{
    std::string name;
    int location;
    line >> name >> location;
    m_reader.NewUser(name, location);
    //m_reader.ChangeState<ReadingDescriptionState>();
}

void UserReader::Read(std::istream &file)
{
    std::string line, testWord;
    while (file && !m_currentUser)
    {
        std::getline(file, line);
        std::istringstream lineStream(line);
        m_state->HandleLine(lineStream);
    }
}

}
}
