
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

// std
#include <unordered_map>

// Qt
#include <QColor>

// local
#include "data/flow_elements/flow_element.hpp"

namespace tool::ex::display {

struct Colors{

    static inline QString whiteH     = "#FFFFFF";
    static inline QString lightGrayH = "#BDBDBD";
    static inline QString darkGrayH  = "#2D2D2D";
    static inline QString blackH     = "#000000";

    static inline QString redH       = "#FF0000";
    static inline QString greenH     = "#279340";
    static inline QString orangeH    = "#FF8C00";

    static inline QString lightBlueH = "#8FBBBC";
    static inline QString blueH      = "#5C8EBC";
    static inline QString navyBlueH  = "#427B89";

    using QC = QColor;
    static inline QC whiteC     = QC(whiteH);
    static inline QC lightGrayC = QC(lightGrayH);
    static inline QC darkGrayC  = QC(darkGrayH);
    static inline QC blackC     = QC(blackH);

    static inline QC greenC     = QC(greenH);
    static inline QC orangeC    = QC(orangeH);

    static inline QC lightBlueC = QC(lightBlueH);
    static inline QC blueC      = QC(blueH);
    static inline QC navyBlueC  = QC(navyBlueH);

    struct FlowElementColors{
        QC selectedTextColor;
        QC unselectedTextColor;
        QC selectedLineBoxColor;
        QC unselectedLineBoxColor;
        QC selectedFillBoxColor;
        QC unselectedFillBoxColor;
    };

    struct ButtonColors{
        QC unselectedTextColor;
        QC unselectedLineBoxColor;
        QC unselectedFillBoxColor;
    };

    using FT = tool::ex::FlowElement::Type;
    static inline const std::unordered_map<tool::ex::FlowElement::Type, FlowElementColors> flowElements = {
                         // sel_txt unsel_txt sel_line    unsel_line   sel_box    unsel_box
        {FT::Node,       {blackC,   blackC,   blackC,     blackC,      blackC,    blackC}},
        {FT::LoopStart,  {whiteC,   whiteC,   navyBlueC,  blackC,      navyBlueC, navyBlueC}},
        {FT::LoopEnd,    {whiteC,   whiteC,   navyBlueC,  blackC,      navyBlueC, navyBlueC}},
        {FT::Routine,    {whiteC,   whiteC,   greenC,     blackC,      greenC,    greenC}},
        {FT::Isi,        {whiteC,   whiteC,   orangeC,    blackC,      orangeC,   orangeC}},
        {FT::Loop,       {whiteC,   whiteC,   navyBlueC,  blackC,      navyBlueC, navyBlueC}},
    };

    using BT = tool::ex::ButtonType;
    static inline const std::unordered_map<tool::ex::ButtonType, ButtonColors> buttons = {
        // sel_txt           unsel_txt  unsel_line  unsel_box
        {BT::AddElement,     {whiteC,   blackC,     whiteC}},
        {BT::RemoveElement,  {whiteC,   blackC,     blackC}},
        {BT::MoveElement,    {blackC,   blackC,     whiteC}},
    };

    static inline const QColor& text(bool selected, tool::ex::FlowElement::Type type){
        return selected ? flowElements.at(type).selectedTextColor : flowElements.at(type).unselectedTextColor;
    }

    static inline const QColor& line_box(bool selected, tool::ex::FlowElement::Type type){
        return selected ? flowElements.at(type).selectedLineBoxColor : flowElements.at(type).unselectedLineBoxColor;
    }

    static inline const QColor& fill_box(bool selected, tool::ex::FlowElement::Type type){
        return selected ? flowElements.at(type).selectedFillBoxColor : flowElements.at(type).unselectedFillBoxColor;
    }

    static inline const QColor& text(tool::ex::ButtonType type){
        return buttons.at(type).unselectedTextColor;
    }

    static inline const QColor& line_box( tool::ex::ButtonType type){
        return buttons.at(type).unselectedLineBoxColor;
    }

    static inline const QColor& fill_box(tool::ex::ButtonType type){
        return buttons.at(type).unselectedFillBoxColor;
    }
};

struct Styles{

    using C = Colors;

    struct OName{QString value;};
    struct BName{QString value;};
    struct FCol{QString hexValue;};
    struct BGCol{QString hexValue;};
    struct BDCol{QString hexValue;};

    static QString object_name_b(const OName &objectName);
    static QString object_name_contains_b(const BName &baseName);
    static QString front_color_b(const FCol &fCol);
    static QString background_color_b(const BGCol &bgCol);
    static QString border_color(const BDCol &bdCol);
    static QString border_width(int pixelSize);
    // dashed dot-dash dot-dot-dash dotted double groove inset outset ridge solid none
    static QString border_style(const QString &borderStyle);


    static QString font_family(QString family);
    static QString font_size(int fontSize = -1);
    static QString font_style(QFont::Style style = QFont::Style::StyleNormal);
    static QString font_weight(QFont::Weight weight);

    static QString qwidget(OName oName, BGCol bgCol);
    static QString qlabel(OName oName, FCol fCol, BGCol bgCol);
    static QString qcheck_box(OName oName, FCol fCol, BGCol bgCol);
    static QString qspin_box(OName oName, BGCol bgCol);
    static QString qdoublespin_box(OName oName, BGCol bgCol);
    static QString qabstract_item_view(OName oName, BGCol bgCol);
    static QString qcombo_box(OName oName, BGCol bgCol);
    static QString qtext_edit(OName oName, FCol fCol, BGCol bgCol);
    static QString qtable_widget(OName oName, BGCol bCol);
    static QString qheader_view(OName oName, BGCol bgCol, FCol fCol, QFont::Weight weight = QFont::Weight:: Bold);
    static QString qline(BName bName, BDCol bdColor);
    static QString qtab_widget(OName oName, BGCol bgColor);
    static QString qlist_widget(OName oName, BGCol bColor, BGCol bItemColor, BGCol bSelectedItemColor);
    static QString qtoolbar(OName oName, BGCol bgColor);
    static QString qtoolbutton(OName oName, BGCol bgColor);

    static QString qpush_button(
        OName oName,
        BGCol bgColor, BGCol bgHoverColor, BGCol bgPressedColor,
        BDCol bdColor, BDCol bdHoverColor, BDCol bdPressedColor);

    static QString qtabbar(
        OName oName,
        BGCol bgColor, FCol fColor,
        BGCol bgHoverColor, FCol fHoverColor,
        BGCol bgSelectedColor, FCol fSelectedColor,
        BGCol bgSelectedHoverColor, FCol fSelectedHoverColor);

    static QString qtext_browser(
        OName oName,
        BGCol bgColor,
        FCol fColor,
        QFont::Weight w = QFont::Weight::Normal, int sizeFont = -1);

    static QString qdock_widget(
        OName oName,
        BGCol bgColor,
        FCol fColor,
        QFont::Weight w = QFont::Weight::Normal, int sizeFont = -1);

    static QString qmenu(
        OName oName,
        FCol fColor, FCol fItemColor, FCol fSelectedItemColor,
        BGCol bgColor, BGCol bgItemColor, BGCol bgSelectedItemColor,
        int sizeFont);

    static QString qtable_qlabel();
    static QString toolbar();
    static QString logger();
    static QString docks();
    static QString menu();
    static QString global();
    static QString component_config();
};

}
