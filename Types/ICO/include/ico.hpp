#pragma once

#include "GView.hpp"

namespace GView
{
namespace Type
{
    namespace ICO
    {
#pragma pack(push, 2)
        constexpr uint32_t MAGIC_FORMAT_ICO = 0x00010000;
        constexpr uint32_t MAGIC_FORMAT_CUR = 0x00020000;

        struct Header
        {
            uint32_t magic;
            uint16_t count;
        };
        struct IconDirectoryEntry
        {
            uint8_t width;
            uint8_t height;
            uint8_t colorPallette;
            uint8_t reserved;
            uint16_t colorPlanes;
            uint16_t bitsPerPixels;
            uint32_t size;
            uint32_t offset;
        };
        struct CursorDirectoryEntry
        {
            uint8_t width;
            uint8_t height;
            uint8_t colorPallette;
            uint8_t reserved;
            uint16_t hotstopX;
            uint16_t hotstopY;
            uint32_t size;
            uint32_t offset;
        };
        union DirectoryEntry
        {
            IconDirectoryEntry ico;
            CursorDirectoryEntry cursor;
        };

#pragma pack(pop) // Back to 4 byte packing.

        class ICOFile : public TypeInterface
        {
          public:
            bool isIcoFormat;

            enum class ErrorType : unsigned char
            {
                Error,
                Warning
            };
            struct ErrorInformation
            {
                ErrorType type;
                FixSizeString<252> text;
            };
            
          public:
            Reference<GView::Utils::FileCache> file;
            std::vector<DirectoryEntry> dirs;

            void AddError(ErrorType type, std::string_view message);

          public:
            ICOFile(Reference<GView::Utils::FileCache> file);
            virtual ~ICOFile()
            {
            }

            bool Update();
            void UpdateBufferViewZones(Reference<GView::View::BufferViewerInterface> bufferView);

            std::string_view GetTypeName() override
            {
                return "ICO";
            }
        };
        namespace Panels
        {
            class Information : public AppCUI::Controls::TabPage
            {
                Reference<GView::Type::ICO::ICOFile> ico;
                Reference<AppCUI::Controls::ListView> general;
                Reference<AppCUI::Controls::ListView> issues;

                void UpdateGeneralInformation();
                void UpdateIssues();
                void RecomputePanelsPositions();

              public:
                Information(Reference<GView::Type::ICO::ICOFile> ico);

                void Update();
                virtual void OnAfterResize(int newWidth, int newHeight) override
                {
                    RecomputePanelsPositions();
                }
            };
            class Directories : public AppCUI::Controls::TabPage
            {
                Reference<GView::Type::ICO::ICOFile> ico;
                Reference<GView::View::WindowInterface> win;
                Reference<AppCUI::Controls::ListView> list;

                void GoToSelectedDirectory();
                void SelectCurrentDirectory();
              public:
                Directories(Reference<GView::Type::ICO::ICOFile> ico, Reference<GView::View::WindowInterface> win);
                bool OnUpdateCommandBar(AppCUI::Application::CommandBar& commandBar) override;
                bool OnEvent(Reference<Control> ctrl, Event evnt, int controlID) override;
                void Update();
            };

        }; // namespace Panels
    }      // namespace ICO
} // namespace Type
} // namespace GView
