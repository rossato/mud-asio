#include "MudProgram.hpp"
#include "MudProgramImpl.hpp"

using namespace Mud::Program;

MudProgram::MudProgram()
    : m_impl(std::make_unique<Mud::Program::MudProgramImpl>(*this)),
      m_dictionary(m_impl->m_dictionary),
      m_tokenizer(m_impl->m_tokenizer),
      m_world(m_impl->m_world),
      m_parser(m_impl->m_parser),
      m_server(m_impl->m_server)
{}

MudProgram::~MudProgram()
{}

void MudProgram::AcceptMudConnections(int port)
{
    m_impl->AcceptMudConnections(port);
}

void MudProgram::AcceptHttpConnections(int port)
{
    m_impl->AcceptHttpConnections(port);
}

void MudProgram::Run()
{
    m_server.Run();
}
