
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

#include "display.hpp"

using namespace tool::ex;
using namespace tool::ex::display;


QString Styles::object_name_b(const OName &objectName){
    return objectName.value.length() > 0 ? QString("[objectName=\"%1\"]").arg(objectName.value) : "";
}

QString Styles::object_name_contains_b(const BName &baseName){
    return baseName.value.length() > 0 ? QString("[objectName^=\"%1\"]").arg(baseName.value) : "";
}

QString Styles::front_color_b(const FCol &fCol){
    return fCol.hexValue.length() > 0 ? QString("color: %1;").arg(fCol.hexValue) : "";
}

QString Styles::background_color_b(const BGCol &bgCol){
    return bgCol.hexValue.length() > 0 ? QString("background-color: %1;").arg(bgCol.hexValue) : "";
}

QString Styles::border_color(const BDCol &bdCol){
    return bdCol.hexValue.length() > 0 ? QString("border-color: %1;").arg(bdCol.hexValue) : "";
}

QString Styles::border_width(int pixelSize){
    return pixelSize > -1 ? QString("border-width: %1;").arg(pixelSize) : "";
}

QString Styles::border_style(const QString &borderStyle){
    return borderStyle.length() > 0 ? QString("border-style: %1;").arg(borderStyle) : "";
}

QString Styles::font_family(QString family){
    return family.length() > 0 ? QString("font-family: \"%1\";").arg(family) : "";
}

QString Styles::font_size(int fontSize){
    return fontSize > -1 ? QString("font-size: %1px;").arg(fontSize) : "";
}

QString Styles::font_style(QFont::Style style){
    return QString("font-style: %1;").arg(style == QFont::Style::StyleNormal ? "normal" : (style == QFont::Style::StyleItalic ? "italic" : "oblique"));
}

QString Styles::font_weight(QFont::Weight weight){
    using enum QFont::Weight ;
    int w =0;
    switch(weight){
        case Thin: w = 100; break;
        case ExtraLight: w = 200; break;
        case Light: w = 300; break;
        case Normal: w = 400; break;
        case Medium: w = 500; break;
        case DemiBold: w = 600; break;
        case Bold: w = 700; break;
        case ExtraBold: w = 800; break;
        case Black: w = 900; break;
    }
    return QString("font-weight: %1;").arg(w);
}

QString Styles::qwidget(OName oName, BGCol bgCol){
    return QString(
        "QWidget%1{%2}"
    ).arg(
        object_name_b(oName),
        background_color_b(bgCol)
    );
}

QString Styles::qlabel(OName oName, FCol fCol, BGCol bgCol){
    return QString(
        "QLabel%1{%2 %3}"
    ).arg(
        object_name_b(oName),
        front_color_b(fCol),
        background_color_b(bgCol)
    );
}

QString Styles::qcheck_box(OName oName, FCol fCol, BGCol bgCol){
    return QString(
        "QCheckBox%1{%2 %3}"
    ).arg(
        object_name_b(oName),
        front_color_b(fCol),
        background_color_b(bgCol)
    );
}

QString Styles::qspin_box(OName oName, BGCol bgCol){
    return QString(
        "QSpinBox%1{%2}"
    ).arg(
        object_name_b(oName),
        background_color_b(bgCol)
    );
}

QString Styles::qdoublespin_box(OName oName, BGCol bgCol){
    return QString(
       "QDoubleSpinBox%1{%2}"
       ).arg(
        object_name_b(oName),
        background_color_b(bgCol)
    );
}

QString Styles::qabstract_item_view(OName oName, BGCol bgCol){
    return QString(
        "QAbstractItemView%1{%2}"
    ).arg(
        object_name_b(oName),
        background_color_b(bgCol)
    );
}

QString Styles::qcombo_box(OName oName, BGCol bgCol){
    return QString(
        "QComboBox%1{%2}"
    ).arg(
        object_name_b(oName),
        background_color_b(bgCol)
    );
}

QString Styles::qtext_edit(OName oName, FCol fCol, BGCol bgCol){
    return QString(
        "QTextEdit%1{%2 %3}"
    ).arg(
        object_name_b(oName),
        front_color_b(fCol),
        background_color_b(bgCol)
    );
}

QString Styles::qtable_widget(OName oName, BGCol bCol){
    return QString(
        "QTableWidget%1{%2}"
    ).arg(
        object_name_b(oName),
        background_color_b(bCol)
    );
}

QString Styles::qheader_view(OName oName, BGCol bgCol, FCol fCol, QFont::Weight weight){
    return QString(
        "QHeaderView%1::section{%2 %3 %4}"
    ).arg(
        object_name_b(oName),
        front_color_b(fCol),
        background_color_b(bgCol),
        font_weight(weight)
    );
}

QString Styles::qpush_button(OName oName, BGCol bgColor, BGCol bgHoverColor, BGCol bgPressedColor, BDCol bdColor, BDCol bdHoverColor, BDCol bdPressedColor){
    return QString(
       "QPushButton%1{%2 %3}"
       "QPushButton%1:hover{%4 %5}"
       "QPushButton%1:pressed {%6 %7}"
    ).arg(
        object_name_b(oName),
        background_color_b(bgColor),        border_color(bdColor),
        background_color_b(bgHoverColor),   border_color(bdHoverColor),
        background_color_b(bgPressedColor), border_color(bdPressedColor)
    );
}

QString Styles::qlist_widget(OName oName, BGCol bColor, BGCol bItemColor, BGCol bSelectedItemColor){
    return QString(
        "QListWidget%1{%2}"
        "QListWidget%1:item{%3}"
        "QListWidget%1:item:selected{%4}"
    ).arg(
        object_name_b(oName),
        background_color_b(bColor),
        background_color_b(bItemColor),
        background_color_b(bSelectedItemColor)
    );
}

QString Styles::qtab_widget(OName oName, BGCol bgColor){
    return QString(
        "QTabWidget%1{%2}"
        "QTabWidget%1::pane{%2}"
    ).arg(
        object_name_b(oName),
        background_color_b(bgColor)
    );
}

QString Styles::qtabbar(OName oName, BGCol bgColor, FCol fColor, BGCol bgHoverColor, FCol fHoverColor, BGCol bgSelectedColor, FCol fSelectedColor, BGCol bgSelectedHoverColor, FCol fSelectedHoverColor){

    return QString(
        "QTabBar%1::tab{%2 %3}"
        "QTabBar%1::tab:selected:hover{%8 %9}"
        "QTabBar%1::tab:selected{%6 %7}"
        "QTabBar%1::tab:!selected{%4 %5}"
    ).arg(
        object_name_b(oName),
        background_color_b(bgColor),               front_color_b(fColor),
        background_color_b(bgHoverColor),          front_color_b(fHoverColor),
        background_color_b(bgSelectedColor),       front_color_b(fSelectedColor),
        background_color_b(bgSelectedHoverColor),  front_color_b(fSelectedHoverColor)
    );
}

QString Styles::qline(BName bName, BDCol bdColor){
    return QString(
        "QFrame%1{border: none; border-bottom: 1px solid; %2}"
    ).arg(
        object_name_contains_b(bName),
        border_color(bdColor)
    );
}

QString Styles::qtext_browser(OName oName, BGCol bgColor, FCol fColor, QFont::Weight w, int sizeFont){
    return QString(
        "QTextBrowser%1{%2 %3 %4 %5}"
    ).arg(
        object_name_b(oName),
        background_color_b(bgColor),
        front_color_b(fColor),
        font_weight(w),
        font_size(sizeFont)
    );
}

QString Styles::qdock_widget(OName oName, BGCol bgColor, FCol fColor, QFont::Weight w, int sizeFont){
    return QString(
       "QDockWidget%1{%2 %3 %4 %5}"
    ).arg(
        object_name_b(oName),
        background_color_b(bgColor),
        front_color_b(fColor),
        font_weight(w),
        font_size(sizeFont)
    );
}

QString Styles::qmenu(OName oName, FCol fColor, FCol fItemColor, FCol fSelectedItemColor, BGCol bgColor, BGCol bgItemColor, BGCol bgSelectedItemColor, int sizeFont){
    return QString(
        "QMenu%1{%8 %2 %5}"
        "QMenu%1::item{%3 %6}"
        "QMenu%1::item:selected{%4 %7}"
    ).arg(
        object_name_b(oName),
        front_color_b(fColor),front_color_b(fItemColor),front_color_b(fSelectedItemColor),
        background_color_b(bgColor),background_color_b(bgItemColor),background_color_b(bgSelectedItemColor),
        font_size(sizeFont)
    );
}


QString Styles::qtoolbutton(OName oName, BGCol bgColor){
    return QString(
               "QToolButton%1:hover{%2}"
               ).arg(
            object_name_b(oName),
        background_color_b(bgColor)
    );
}

QString Styles::qtoolbar(OName oName, BGCol bgColor){
    return QString(
        "QToolBar%1{%2}"
    ).arg(
        object_name_b(oName),
        background_color_b(bgColor)
    );
}


QString Styles::qtable_qlabel(){
    return
        qlabel(OName{""}, FCol{C::blackH}, BGCol{C::lightGrayH});
}

QString Styles::toolbar(){
    return
        qtoolbar(OName{""}, BGCol{C::darkGrayH}) %

        qtoolbutton(OName{""}, BGCol{C::lightGrayH});
}

QString Styles::logger(){
    return
        qtext_browser(OName{""}, BGCol{C::darkGrayH}, FCol{""}, QFont::Normal, 11);
}

QString Styles::docks(){
    return
        qdock_widget(OName{""}, BGCol{C::darkGrayH}, FCol{C::whiteH}, QFont::Weight::Bold, 14);
}

QString Styles::menu(){
    return
        qmenu(OName{""}, FCol{""}, FCol{C::whiteH}, FCol{C::blackH}, BGCol{C::darkGrayH}, BGCol{C::darkGrayH}, BGCol{C::whiteH}, 12);
}

QString Styles::global(){
    return
        qwidget(            OName{""},     BGCol{C::darkGrayH}) %
        qlabel(             OName{""},     FCol{C::whiteH}, BGCol{C::darkGrayH}) %
        qcheck_box(         OName{""},     FCol{C::whiteH}, BGCol{C::darkGrayH}) %
        qtext_edit(         OName{""},     FCol{C::blackH}, BGCol{C::lightGrayH}) %
        qspin_box(          OName{""},     BGCol{C::lightGrayH}) %
        qdoublespin_box(    OName{""},     BGCol{C::lightGrayH}) %
        qcombo_box(         OName{""},     BGCol{C::lightGrayH}) %
        qabstract_item_view(OName{""},     BGCol{C::lightGrayH}) %
        qpush_button(       OName{""},     BGCol{C::lightGrayH}, BGCol{C::lightBlueH}, BGCol{C::blueH}, BDCol{C::blackH}, BDCol{C::blueH}, BDCol{C::blueH}) %
        qtable_widget(      OName{""},     BGCol{C::darkGrayH}) %
        qline(              BName{"line"}, BDCol{C::whiteH}) %
        qlist_widget(       OName{""},     BGCol{C::darkGrayH}, BGCol{C::lightGrayH}, BGCol{C::lightBlueH}) %
        qheader_view(       OName{""},     BGCol{C::darkGrayH}, FCol{C::whiteH}, QFont::Weight::DemiBold) %
        qtab_widget(        OName{""},     BGCol{C::darkGrayH}) %
        qtabbar(            OName{""},     BGCol{C::darkGrayH}, FCol{C::whiteH}, BGCol{C::darkGrayH}, FCol{C::whiteH}, BGCol{C::darkGrayH}, FCol{C::whiteH}, BGCol{C::darkGrayH}, FCol{C::whiteH});
    ;
}

