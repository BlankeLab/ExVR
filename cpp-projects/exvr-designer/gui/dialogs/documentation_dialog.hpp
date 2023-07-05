
/***********************************************************************************
** exvr-designer                                                                  **
** MIT License                                                                    **
** Copyright (c) [2018] [Florian Lance][EPFL-LNCO]                                **
** Permission is hereby granted, free of charge, to any person obtaining a copy   **
** of this software and associated documentation files (the "Software"), to deal  **
** in the Software without restriction, including without limitation the rights   **
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      **
** copies of the Software, and to permit persons to whom the Software is          **
** furnished to do so, subject to the following conditions:                       **
**                                                                                **
** The above copyright notice and this permission notice shall be included in all **
** copies or substantial portions of the Software.                                **
**                                                                                **
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     **
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       **
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    **
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         **
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  **
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  **
** SOFTWARE.                                                                      **
************************************************************************************/

#pragma once

// base
#include "utility/unordered_map.hpp"

// Qt
#include <QDialog>
#include <QListWidget>
#include <QTextBrowser>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

// qt-utility
#include "gui/ex_widgets/ex_code_editor_w.hpp"

// local
#include "data/component.hpp"
#include "data/connector.hpp"

namespace tool::ex {

using namespace std::literals::string_view_literals;

enum class DocSection {
    General=0,
    UiFlowCreation,
    UiElementSelection,
    UiComponentsList,
    UiConnectorsList,
    UiRoutinesConditions,
    UiResourcesManager,
    UiRandomization,
    UiToobar,
    UiLogs,
    UiSettings,
    ContentComponentsDescription,
    ContentConnectorsDescription,
    ContentScripting,
    ContentVisualScripting,
    ContentExpLauncher,
    ContentSamples,
    SizeEnum
};

enum class UiDocType  {
    TextBrowser,
    Widget
};

using Name = std::string_view;
using MarkdownFile = Name;
using SectionName = Name;

using Id = int;
using TDocSection = std::tuple<
    DocSection,                                 Id, UiDocType,              MarkdownFile,                              SectionName>;
static constexpr TupleArray<DocSection::SizeEnum, TDocSection> sections = {{
    TDocSection
    {DocSection::General,                       0,  UiDocType::TextBrowser, "doc_general.md"sv,                        "Index"sv,                          },
    {DocSection::UiFlowCreation,                1,  UiDocType::TextBrowser, "doc_ui_flow_creation.md"sv,               "[UI] Flow creation"sv,             },
    {DocSection::UiElementSelection,            2,  UiDocType::TextBrowser, "doc_ui_element_selection.md"sv,           "[UI] Element selection"sv,         },
    {DocSection::UiComponentsList,              3,  UiDocType::TextBrowser, "doc_ui_component_list.md"sv,              "[UI] Components list"sv,           },
    {DocSection::UiConnectorsList,              4,  UiDocType::TextBrowser, "doc_ui_connectors_list.md"sv,             "[UI] Connectors list"sv,           },
    {DocSection::UiRoutinesConditions,          5,  UiDocType::TextBrowser, "doc_ui_routines_conditions.md"sv,         "[UI] Routines conditions"sv,       },
    {DocSection::UiResourcesManager,            6,  UiDocType::TextBrowser, "doc_ui_resources_manager.md"sv,           "[UI] Resources manager"sv,         },
    {DocSection::UiRandomization,               7,  UiDocType::TextBrowser, "doc_ui_randomization.md"sv,               "[UI] Randomization"sv,             },
    {DocSection::UiToobar,                      8,  UiDocType::TextBrowser, "doc_ui_toolboar.md"sv,                    "[UI] Toolbar"sv,                   },
    {DocSection::UiLogs,                        9,  UiDocType::TextBrowser, "doc_ui_logs.md"sv,                        "[UI] Logs"sv,                      },
    {DocSection::UiSettings,                    10, UiDocType::TextBrowser, "doc_ui_settings.md"sv,                    "[UI] Settings"sv,                  },
    {DocSection::ContentComponentsDescription,  11, UiDocType::Widget,      "components"sv,                            "[Content] Components description"sv},
    {DocSection::ContentConnectorsDescription,  12, UiDocType::Widget,      "connectors"sv,                            "[Content] Connectors description"sv},
    {DocSection::ContentScripting,              13, UiDocType::TextBrowser, "doc_content_scripting.md"sv,              "[Content] Scripting"sv,            },
    {DocSection::ContentVisualScripting,        14, UiDocType::TextBrowser, "doc_content_visual_scripting.md"sv,       "[Content] Visual scripting"sv,     },
    {DocSection::ContentExpLauncher,            15, UiDocType::TextBrowser, "doc_content_exp_launcher.md"sv,           "[Content] Exp-launcher"sv,         },
    {DocSection::ContentSamples,                16, UiDocType::TextBrowser, "doc_content_samples.md"sv,                "[Content] Samples"sv,              },
}};

[[maybe_unused]] static auto all_sections() {
    return sections.tuple_column<0>();
}

[[maybe_unused]] static auto all_sections_names() {
    return sections.tuple_column<4>();
}

[[maybe_unused]] static std::optional<DocSection> get_doc_section(Id id) {
    return sections.optional_at<1,0>(id);
}

[[maybe_unused]] static std::optional<DocSection> get_doc_section(MarkdownFile mdf) {
    return sections.optional_at<3,0>(mdf);
}


[[maybe_unused]] static Id section_id(DocSection ds) {
    return sections.at<0,1>(ds);
}

[[maybe_unused]] static UiDocType ui_doc_type(DocSection ds) {
    return sections.at<0,2>(ds);
}

[[maybe_unused]] static MarkdownFile markdown_file(DocSection ds) {
    return sections.at<0,3>(ds);
}

[[maybe_unused]] static SectionName section_name(DocSection ds) {
    return sections.at<0,4>(ds);
}


class SectionW : public QWidget{

public:

    SectionW(QString title);

    void set_current_row(const QString &txt);

    void reload(){

    }

    QLabel *laTitle  = nullptr;
    QListWidget *lwContent = nullptr;
};

class DocumentationDialog : public QDialog{

    Q_OBJECT

public :

    DocumentationDialog();

    static QTextBrowser *generate_text_browser();
    void init_components_doc();
    void init_connectors_doc();

public slots:

    void show_window();    
    void show_components_section(Component::Type type, bool resetWindow);
    void show_connectors_section(Connector::Type type, bool resetWindow);
    void show_section(tool::ex::DocSection section, bool resetWindow);

private slots:

    void display_components_section(Component::Type type);
    void display_connectors_section(Connector::Type type);
    void display_other_section();

    void update_current_component_doc(Component::Type type);
    void update_current_connector_doc(Connector::Type type);

    void update_current_category_components_list();

private:

    void reload_sub_section_markdown_file(DocSection section, bool forceReload);
    void reload_info_file(const QString &infoFilePath);
    void reload_connections_file(const QString &connectionsFilePath);
    void reload_csharp_markdown_file(const QString &csFilePath);


    bool m_lncoComponents = false;

    DocSection currentSection = DocSection::General;

    QHBoxLayout *mainLayout = nullptr;
    SectionW *documentationsCategoriesW = nullptr;
    // sub categories
    umap<DocSection, QWidget*> sectionsWidgets;
    umap<DocSection, QString> sectionsDocContent;

    // buttons
    QPushButton *genPb = nullptr;
    QPushButton *openPb = nullptr;
    QPushButton *reloadPb = nullptr;
    QString currentDocPath = "";

    // components    
    SectionW *componentsCategoriesSectionW = nullptr;
    SectionW *componentsSectionW = nullptr;
    QTabWidget *tabComponentsDocW = nullptr;
    QTextBrowser *componentsInfoW = nullptr;
    QTextBrowser *componentsConnectionsW = nullptr;
    ExCodeEditorW componentsCsharpScriptingW;
    QTextBrowser *componentsPythonScriptingW = nullptr;
    Component::Category currentComponentCategory = Component::Category::Output;
    Component::Type currentComponent = Component::T::Logger;
    QStringList componentsCategoriesStr;
    QStringList componentsFullStr;

    // connectors    
    SectionW *connectorsCategoriesSectionW = nullptr;
    SectionW *connectorsSectionW = nullptr;
    QTabWidget *tabConnectorsDocW = nullptr;
    QTextBrowser *connectorsInfoW = nullptr;
    QTextBrowser *connectorsConnectionsW = nullptr;
    Connector::Category currentConnectorCategory = Connector::Category::Generator;
    Connector::Type currentConnector = Connector::Type::Integer;
    QStringList connectorsCategoriesStr;
    QStringList connectorsFullStr;

    // hightlighers
    // PythonHighlighter *pythonHighlighter = nullptr;    
};
}
