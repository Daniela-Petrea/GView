#include "log.hpp"

using namespace GView::Type::LOG;

namespace GView::Type::LOG::Panels
{
Information::Information(Reference<GView::Type::LOG::LOGFile> log) : TabPage("&Information")
{
    this->log = log;
    general   = Factory::ListView::Create(this, "x:0,y:0,w:100%,h:10", { "n:Field,w:12", "n:Value,w:100" }, ListViewFlags::None);

    issues = Factory::ListView::Create(this, "x:0,y:21,w:100%,h:10", { "n:Info,w:200" }, ListViewFlags::HideColumns);

    this->Update();
}

void Information::OnAfterResize(int newWidth, int newHeight)
{
    RecomputePanelsPositions();
}

void Information::UpdateIssues()
{
}

void Information::Update()
{
    UpdateGeneralInformation();
    UpdateIssues();
    RecomputePanelsPositions();
}

void Information::UpdateGeneralInformation()
{
    general->DeleteAllItems();

    LocalString<256> ls;
    NumericFormatter nf;

    general->AddItem("Info").SetType(ListViewItem::Type::Category);

    general->AddItem({ "Filename", log->obj->GetName() });
    general->AddItem({ "First Timestamp", log->GetEvents().empty() ? "N/A" : log->GetEvents().front()->timestamp });
    general->AddItem({ "Last Timestamp", log->GetEvents().empty() ? "N/A" : log->GetEvents().back()->timestamp });

    std::string dataLines = std::to_string(log->GetEvents().size()) + " entries";
    general->AddItem({ "Extracted Data", dataLines });

    for (int i = 0; i < log->GetEvents().size(); i++) {
        ListViewItem::Type fontType = ListViewItem::Type::Normal;

        if (log->GetEvents()[i]->type == "ERROR")
            fontType = ListViewItem::Type::ErrorInformation;
        else if (log->GetEvents()[i]->type == "WARN")
            fontType = ListViewItem::Type::WarningInformation;

        general
              ->AddItem(
                    { "",
                      ls.Format(
                            "%-26s %-7s %s", log->GetEvents()[i]->timestamp.c_str(), log->GetEvents()[i]->type.c_str(), log->GetEvents()[i]->details.c_str()) })
              .SetType(fontType);
    }
}

void Information::RecomputePanelsPositions()
{
    int w = this->GetWidth();
    int h = this->GetHeight();
    if ((!general.IsValid()) || (!issues.IsValid()))
        return;
    issues->SetVisible(false);

    if (this->general != nullptr)
    {
        if (this->general->GetItemsCount() > 15)
        {
            this->general->Resize(w, 18);
        }
        else
        {
            this->general->Resize(w, this->general->GetItemsCount() + 3);
        }
    }
}

} // namespace GView::Type::LOG::Panels
