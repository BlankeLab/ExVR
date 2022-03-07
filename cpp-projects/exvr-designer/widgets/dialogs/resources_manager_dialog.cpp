
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

#include "resources_manager_dialog.hpp"

// Qt
#include <QInputDialog>
#include <QDesktopServices>

// qt-utility
#include "qt_ui.hpp"

// local
#include "utility/path_utility.hpp"

using namespace tool::ex;

ResourcesManagerDialog::ResourcesManagerDialog(){


    m_ui.setupUi(this);
    m_ui.twCategories->tabBar()->setStyle(new VerticalTextTabWidgetStyle());
    setWindowTitle("Resources manager");    
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowIcon(QIcon(":/icons/Resources"));

    connect(m_ui.cbReloadAudio,         &QCheckBox::clicked, this, &ResourcesManagerDialog::update_resources_to_reload);
    connect(m_ui.cbReloadImages,        &QCheckBox::clicked, this, &ResourcesManagerDialog::update_resources_to_reload);
    connect(m_ui.cbReloadMeshes,        &QCheckBox::clicked, this, &ResourcesManagerDialog::update_resources_to_reload);
    connect(m_ui.cbReloadClouds,        &QCheckBox::clicked, this, &ResourcesManagerDialog::update_resources_to_reload);
    connect(m_ui.cbReloadTextes,        &QCheckBox::clicked, this, &ResourcesManagerDialog::update_resources_to_reload);
    connect(m_ui.cbReloadVideos,        &QCheckBox::clicked, this, &ResourcesManagerDialog::update_resources_to_reload);
    connect(m_ui.cbReloadAssetBundles,  &QCheckBox::clicked, this, &ResourcesManagerDialog::update_resources_to_reload);
    connect(m_ui.cbReloadCSharpScripts, &QCheckBox::clicked, this, &ResourcesManagerDialog::update_resources_to_reload);
    connect(m_ui.cbReloadPythonScripts, &QCheckBox::clicked, this, &ResourcesManagerDialog::update_resources_to_reload);
    connect(m_ui.cbReloadScanerVideos,  &QCheckBox::clicked, this, &ResourcesManagerDialog::update_resources_to_reload);
    connect(m_ui.cbReloadPlots,         &QCheckBox::clicked, this, &ResourcesManagerDialog::update_resources_to_reload);

    for(const auto &type : Resource::get_types()){

        auto tabW = new QWidget();
        const QString name          = from_view(Resource::get_name(type));
        const QString iconPath      = from_view(Resource::get_icon_path(type));
        const QString displayName   = from_view(Resource::get_display_name(type));
        const bool canOpen          = Resource::can_open(type);
        const bool canGenerate      = Resource::can_generate(type);

        QString filters = std::string(Resource::get_filters(type)).c_str();

        Ui::ResourceTypeW typeUi;
        typeUi.setupUi(tabW);        
        typeUi.laTitle->setText(QString("All files with extensions: ") + filters);
        auto lw = typeUi.lwFiles;               

        connect(typeUi.pbAdd, &QPushButton::clicked, this, [=]{

            QString resourceDir;
            switch (type) {
                case Resource::Type::AssetBundle:
                resourceDir = Paths::assetsBundlesDir;
                break;
                case Resource::Type::CSharpScript:
                resourceDir = Paths::scriptsCSharpDir;
                break;
                case Resource::Type::PythonScript:
                resourceDir = Paths::scriptsPythonDir;
                break;
                case Resource::Type::ScanerVideo:
//                resourceDir = Paths::scriptsPythonDir;
                break;
            default:
                break;
            }

            if(type == Resource::Type::Directory){
                auto directoryPath = QFileDialog::getExistingDirectory(nullptr, "Select resource directory to add", resourceDir);
                if(directoryPath.length() > 0){
                    emit add_resources_signal(type, {directoryPath});
                }
                return;
            }else{

                auto filesPath = QFileDialog::getOpenFileNames(nullptr, "Selection resources files to add", resourceDir, QString(name) + " " + filters);
                if(filesPath.size() > 0){
                    emit add_resources_signal(type, filesPath);
                }
            }
        });
        connect(typeUi.pbRemove, &QPushButton::clicked, this, [=]{
            if(auto id = lw->currentRow(); id >= 0){
                emit remove_resource_signal(type, to_unsigned(id));
            }
        });
        connect(typeUi.pbClear, &QPushButton::clicked, this, [=]{
            emit clean_resources_signal(type);
        });

        if(type == Resource::Type::Directory){
            connect(typeUi.pbFind, &QPushButton::clicked, this, [=]{
                auto currentPath = typeUi.laPath->text();
                auto newPath = QFileDialog::getExistingDirectory(nullptr, "Find selected resource directory", currentPath);
                if(newPath.length() > 0){
                    emit update_resource_path_signal(currentPath, newPath);
                }
            });
        }else{
            connect(typeUi.pbFind, &QPushButton::clicked, this, [=]{
                auto currentPath = typeUi.laPath->text();
                auto newPath = QFileDialog::getOpenFileName(nullptr, "Find selected resource", currentPath, QString(name) + " " + filters, nullptr);
                if(newPath.length() > 0){
                    emit update_resource_path_signal(currentPath, newPath);
                }
            });
        }


        connect(typeUi.pbSetAlias, &QPushButton::clicked, this, [=]{

            auto currentAlias = typeUi.laAlias->text();
            bool ok;
            QString newAlias = QInputDialog::getText(
                this, tr("Resource alias"),
                tr("Enter new alias name:"), QLineEdit::Normal,
                currentAlias, &ok);
            if(ok && !newAlias.isEmpty()){
                emit update_resource_alias_signal(currentAlias, newAlias);
            }
        });

        if(type == Resource::Type::Directory){
            typeUi.pbOpenFile->setEnabled(false);
        }else{
            connect(typeUi.pbOpenFile, &QPushButton::clicked, this, [=]{
                auto currentPath = typeUi.laPath->text().remove('\n');
                if(currentPath.length() > 0){
                    if(QFileInfo(currentPath).exists()){
                        QDesktopServices::openUrl(QUrl::fromLocalFile(currentPath));
                    }
                }
            });
        }
        connect(typeUi.pbOpenDir, &QPushButton::clicked, this, [=]{
            auto currentPath = typeUi.laPath->text().remove('\n');           
            if(currentPath.length() > 0){
                if(QDir(currentPath).exists()){
                    QDesktopServices::openUrl(QUrl::fromLocalFile(currentPath));
                }else{
                    QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(currentPath).absolutePath()));
                }
            }
        });

        connect(lw, &QListWidget::currentItemChanged, this, [=]{
            if(auto id = lw->currentIndex().row(); id >= 0){
                emit resource_selected_signal(type, to_unsigned(id));
            }
        });

        if(type == Resource::Type::CSharpScript){
            connect(typeUi.pbGenerate, &QPushButton::clicked, this, &ResourcesManagerDialog::generate_csharp_script);
            typeUi.pbGenerate->setText("Generate script");
        }else if(type == Resource::Type::Text){
            // TODO: ...
        }

//        typeUi.pbOpenFile->setEnabled(canOpen);
        typeUi.pbOpenFile->setVisible(canOpen);
        typeUi.pbGenerate->setVisible(canGenerate);


        m_typesW[type] = std::make_tuple(std::move(typeUi),tabW);

        m_tabsIdW[type] = m_ui.twCategories->count();
        m_ui.twCategories->addTab(tabW, displayName);        
        m_ui.twCategories->setTabIcon(m_ui.twCategories->count()-1, QIcon(iconPath));

        if(
            type == Resource::Type::Mesh ||
            type == Resource::Type::PythonScript){
            tabW->setEnabled(false);
        }
    }
}

void ResourcesManagerDialog::update_from_resources_manager(const ResourcesManager *resM){

    auto code = resM->reload_code();
    ui::w_blocking(m_ui.cbReloadAudio)->setChecked(code & reloadAudioCode);
    ui::w_blocking(m_ui.cbReloadImages)->setChecked(code & reloadImagesCode);
    ui::w_blocking(m_ui.cbReloadMeshes)->setChecked(code & reloadMeshesCode);
    ui::w_blocking(m_ui.cbReloadTextes)->setChecked(code & reloadTextesCode);
    ui::w_blocking(m_ui.cbReloadVideos)->setChecked(code & reloadVideosCode);
    ui::w_blocking(m_ui.cbReloadAssetBundles)->setChecked(code & reloadAssetBundlesCode);
    ui::w_blocking(m_ui.cbReloadCSharpScripts)->setChecked(code & reloadCSharpScriptsCode);
    ui::w_blocking(m_ui.cbReloadPythonScripts)->setChecked(code & reloadPythonScriptsCode);


    // reset display info
    for(const auto type : Resource::get_types()){
        auto &ui = std::get<0>(m_typesW[type]);
        ui.pbSetAlias->setEnabled(false);
        ui.laPath->setEnabled(false);
        ui.pbFind->setEnabled(false);
        ui.pbOpenDir->setEnabled(false);
        if(type != Resource::Type::Directory){
            ui.pbOpenFile->setEnabled(false);
        }
        ui.laAlias->setText("...");
        ui.laPath->setText("...");
        ui.lwFiles->clear();
    }

    for(const auto type : Resource::get_types()){

        auto &ui = std::get<0>(m_typesW[type]);
        auto idSelected = resM->get_type_selected_id(type);

        // add items in list
        ui.lwFiles->blockSignals(true);
        auto resources = resM->get_resources(type);
        for(auto resource : resources){
            auto info = QFileInfo(resource->path);
            ui.lwFiles->addItem(resource->display_name());
            ui.lwFiles->item(ui.lwFiles->count()-1)->setForeground(info.exists() ? Qt::darkGreen : Qt::red);
        }

        if(to_signed(idSelected) < ui.lwFiles->count()){
            ui.lwFiles->setCurrentRow(to_signed(idSelected));
        }
        ui.lwFiles->blockSignals(false);

        // update resource displayed info
        if(idSelected < resources.size()){
            const auto selectedResource = resources[idSelected];
            ui.laAlias->setText(selectedResource->alias);

            auto newP = selectedResource->path;
            for(int ii = 0; ii < selectedResource->path.length()/100; ++ii){
                newP.insert((ii+1)*100, '\n');
            }
            ui.laPath->setText(newP);

            ui.laPath->setEnabled(true);
            ui.pbFind->setEnabled(true);
            ui.pbSetAlias->setEnabled(true);

            ui.pbOpenDir->setEnabled(true);
            if(type != Resource::Type::Directory){
                ui.pbOpenFile->setEnabled(true);
            }
        }
    }
}

void ResourcesManagerDialog::update_resources_to_reload(){

    const int reloadAudio         = m_ui.cbReloadAudio->isChecked() ?           reloadAudioCode : 0;
    const int reloadImages        = m_ui.cbReloadImages->isChecked() ?          reloadImagesCode : 0;
    const int reloadMeshes        = m_ui.cbReloadMeshes->isChecked() ?          reloadMeshesCode : 0;
    const int reloadTextes        = m_ui.cbReloadTextes->isChecked() ?          reloadTextesCode : 0;
    const int reloadVideos        = m_ui.cbReloadVideos->isChecked() ?          reloadVideosCode : 0;
    const int reloadAssetBundles  = m_ui.cbReloadAssetBundles->isChecked() ?    reloadAssetBundlesCode : 0;
    const int reloadCSharpScripts = m_ui.cbReloadCSharpScripts->isChecked() ?   reloadCSharpScriptsCode : 0;
    const int reloadPythonScripts = m_ui.cbReloadPythonScripts->isChecked() ?   reloadPythonScriptsCode : 0;
    const int reloadClouds        = m_ui.cbReloadClouds->isChecked()?           reloadCloudsCode : 0;
    const int reloadScanerVideos  = m_ui.cbReloadScanerVideos->isChecked()?     reloadScanerVideosCode : 0;
    const int reloadPlots         = m_ui.cbReloadPlots->isChecked()?            reloadPlotsCode : 0;

    const int reloadCode = reloadAudio | reloadImages | reloadMeshes | reloadTextes | reloadVideos | reloadAssetBundles | reloadCSharpScripts | reloadPythonScripts | reloadClouds;
    emit update_reload_resource_code_signal(reloadCode);
}

void ResourcesManagerDialog::generate_csharp_script(){

    bool ok;
    auto componentName = QInputDialog::getText(this, tr("Set custom csharp script component name"), tr("Name:"), QLineEdit::Normal, "TemplateComponent", &ok);
    if(!ok){        
        return;
    }
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Define the CSharp script file to be generated", Paths::scriptsCSharpDir,  "CSharp scirpt (*.cs)");
    if(filePath.size() == 0){
        return;
    }

    QFile defaultScript(Paths::templateCSharpScript);
    if(!defaultScript.open(QFile::ReadOnly)){
        QtLogger::error("Cannot find template C# script file to copy.");
        return;
    }

    QTextStream in(&defaultScript);
    QString content = in.readAll();
    content = content.replace("DefaultNameComponent", componentName);

    QFile output(filePath);
    if(!output.open(QFile::WriteOnly)){
        QtLogger::error("Cannot create new C# script file.");
        return;
    }

    QTextStream w(&output);
    w << content;

    emit add_resources_signal(Resource::Type::CSharpScript, {filePath});
}

void ResourcesManagerDialog::show_section(Resource::Type type){

    if(type != Resource::Type::SizeEnum){
        m_ui.twCategories->setCurrentIndex(m_tabsIdW[type]);
    }

    show();
    raise();
}


QSize VerticalTextTabWidgetStyle::sizeFromContents(QStyle::ContentsType type, const QStyleOption *option, const QSize &size, const QWidget *widget) const {
    QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
    if (type == QStyle::CT_TabBarTab) {
        s.transpose();
    }
    return s;
}

void VerticalTextTabWidgetStyle::drawControl(QStyle::ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const {
    if (element == CE_TabBarTabLabel) {
        if (const QStyleOptionTab* tab = qstyleoption_cast<const QStyleOptionTab*>(option)) {
            QStyleOptionTab opt(*tab);
            opt.shape = QTabBar::RoundedNorth;
            QProxyStyle::drawControl(element, &opt, painter, widget);
            return;
        }
    }
    QProxyStyle::drawControl(element, option, painter, widget);
}

#include "moc_resources_manager_dialog.cpp"
