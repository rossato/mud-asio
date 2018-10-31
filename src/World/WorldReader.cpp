#include "WorldReader.hpp"

#include "Dictionary/Tokenizer.hpp"
#include "Server/Ansi.hpp"

// None of this has particularly robust error handling

namespace Mud
{
namespace World
{

namespace
{
    const std::string EXITS("EXITS");
    const std::string ITEMS("ITEMS");
}

class ReadingDescriptionState : public IWorldReaderState
{
public:
    explicit ReadingDescriptionState(WorldReader &reader)
        : IWorldReaderState(reader)
    {}

    ~ReadingDescriptionState()
    {
        m_reader.CurrentLocation().SetDescription(m_description.str());
    }

    virtual void HandleLine(std::istream &line) override
    {
        if (m_description.tellp()) m_description << NEWLINE;
        m_description << line.rdbuf();
    }

private:
    std::ostringstream m_description;
};

class ReadingExitsState : public IWorldReaderState
{
public:
    explicit ReadingExitsState(WorldReader &reader)
        : IWorldReaderState(reader)
    {}

    virtual void HandleLine(std::istream &line) override
    {
        int dir, destination;
        line >> dir >> destination;
        m_reader.CurrentLocation().SetExit(Direction(dir), destination);
    }
};

class ReadingItemsState : public IWorldReaderState
{
public:
    explicit ReadingItemsState(WorldReader &reader)
        : IWorldReaderState(reader)
    {}

    virtual void HandleLine(std::istream &line) override
    {
        m_reader.Builder().NewItem(
            std::vector<std::string>(std::istream_iterator<std::string>(line),
                                     std::istream_iterator<std::string>())
            );
    }
};

void NewRoomState::HandleLine(std::istream &line)
{
    int number;
    line >> number;

    std::istream::sentry sentry(line);
    std::string title;
    std::getline(line, title);

    m_reader.NewLocation(number, title);
    m_reader.ChangeState<ReadingDescriptionState>();
}

void WorldReader::Read(std::istream &file)
{
    std::string line, testWord;
    while (file)
    {
        std::getline(file, line);
        std::istringstream lineStream(line);

        std::string word;
        if (lineStream >> word)
        {
            if (word == EXITS)
            {
                ChangeState<ReadingExitsState>();
            }
            else if (word == ITEMS)
            {
                ChangeState<ReadingItemsState>();
            }
            else
            {
                lineStream.seekg(0);
                m_state->HandleLine(lineStream);
            }
        }
        else
        {
            ChangeState<NewRoomState>();
        }
    }
}

}
}
