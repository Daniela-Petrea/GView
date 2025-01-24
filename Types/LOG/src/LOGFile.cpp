#include "log.hpp"
#include <regex>

using namespace GView::Type::LOG;

LOGFile::LOGFile() : panelsMask(0)
{
    this->panelsMask |= (1ULL << (unsigned char) Panels::IDs::Information);
}

std::string_view LOGFile::GetTypeName()
{
    return "LOG File";
}
bool LOGFile::Update(Reference<GView::Object> _obj)
{
    this->obj = _obj;

    // Get the content of the file as a BufferView
    const auto contentBuffer = this->obj->GetData().GetEntireFile();

    // Convert BufferView to string
    const std::string content(reinterpret_cast<const char*>(contentBuffer.GetData()), contentBuffer.GetLength());

    // Parse log lines
    std::istringstream stream(content);

    std::string line;
    while (std::getline(stream, line)) {
        // Extract timestamps and events
        std::regex regex(R"(\[([^\]]+)\]\s+(\w+)\s+-\s+(.*))");
        std::smatch match;
        if (std::regex_search(line, match, regex)) {
            if (match.size() == 4) {
                events.push_back(new event{match[1].str(), match[2].str(), match[3].str()}); // timestamp, type, details
            }
            else
            {
                events.push_back(new event{match[1].str(), "UNKNOWN", line});
            }
        }
    }

    std::sort(events.begin(), events.end(), [](const event* a, const event* b) { return a->type < b->type; });

    return true;
}


bool LOGFile::HasPanel(Panels::IDs id)
{
    return (this->panelsMask & (1ULL << ((unsigned char) id))) != 0;
}

void LOGFile::UpdateBufferViewZones(GView::View::BufferViewer::Settings& settings)
{
    const auto color = ColorPair{ Color::Gray, Color::Transparent };

    // Get the content of the file as a BufferView
    const auto contentBuffer = this->obj->GetData().GetEntireFile();

    // Convert BufferView to string
    const std::string content(reinterpret_cast<const char*>(contentBuffer.GetData()), contentBuffer.GetLength());

    std::istringstream stream(content);

    uint64 offset = 0;
    std::string line;
    while (std::getline(stream, line)) {
        settings.AddZone(offset, line.size(), color, "LOG Entry");
        offset += line.size() + 1; // +1 for newline
    }
}
