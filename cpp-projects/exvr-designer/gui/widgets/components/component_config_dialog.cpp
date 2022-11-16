
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

#include "component_config_dialog.hpp"

// Qt
#include <QIcon>
#include <QImage>
#include <QToolButton>
#include <QLineEdit>
#include <QMessageBox>

// base
#include "utility/types.hpp"
#include "utility/benchmark.hpp"

// local
#include "gui/settings/display.hpp"
#include "experiment/global_signals.hpp"


using namespace tool::ex;

ComponentConfigDialog::ComponentConfigDialog(QWidget *parent, Component *component) :
      componentKey({component->key()}), componentType(component->type){

    m_parent = parent;

    // set base ui of the dialog
    m_ui.setupUi(this);
    m_ui.leName->setText(component->name());
    m_ui.tabConfigs->setMovable(true);
    m_ui.pbReset->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    m_ui.pbPin->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    m_ui.pbHelp->setFocusPolicy(Qt::FocusPolicy::NoFocus);

    setModal(false);
    defaultFlags = windowFlags() & ~Qt::WindowContextHelpButtonHint;
    setWindowFlags(defaultFlags); 
    setWindowTitle(QSL("[") % from_view(Component::get_full_name(component->type)) % QSL("] component settings"));
    setWindowIcon(QIcon(Component::get_icon_path(component->type)));

//    this->setStyleSheet(display::Styles::component_config());

    set_connections();

    Bench::start("[ComponentsConfigDialog generate init config w]"sv, false);
        // add init config widget
        m_initConfigW = new ConfigW(component->initConfig.get(), component, true, component->initConfig->args);
        m_ui.vlInitConfig->addWidget(m_initConfigW);
    Bench::stop();


    Bench::start("[ComponentsConfigDialog init 5]"sv, false);

    // enable configurations
    auto configOpt = Component::get_config_opt(component->type);
    m_ui.twAllConfigs->setEnabled(configOpt !=  Component::ConfigO::None);
    m_ui.twAllConfigs->setTabEnabled(0,configOpt == Component::ConfigO::Initalization || configOpt ==  Component::ConfigO::Both);
    m_ui.twAllConfigs->setTabEnabled(1,configOpt == Component::ConfigO::Condition      || configOpt ==  Component::ConfigO::Both);

    if(m_ui.twAllConfigs->isTabEnabled(0)){
        m_ui.twAllConfigs->setCurrentIndex(0);
    }

    Bench::stop();
    Bench::start("[ComponentsConfigDialog init 6]"sv, false);

    update_from_component(component);

    Bench::stop();
}

void ComponentConfigDialog::set_connections(){

    // change component name
    connect(m_ui.leName, &QLineEdit::textChanged, this, [=](QString text){
        emit GSignals::get()->component_name_changed_signal(componentKey, text);
    });

    // remove selected config
    connect(m_ui.pbRemoveConfig, &QPushButton::clicked, this, [=]{
        auto id = m_ui.tabConfigs->currentIndex();
        if(id >= 0 && id < m_ui.tabConfigs->count()){
            m_ui.tabConfigs->removeTab(id);
            emit GSignals::get()->remove_config_signal(componentKey, RowId{id});
        }
    });

    // pin dialog
    connect(m_ui.pbPin, &QPushButton::clicked, this, [=] {
        if(!pinned){
            setParent(m_parent);
            setWindowFlags(Qt::Dialog);
            setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
            m_ui.pbPin->setIcon(QIcon(":/icons/Unpin"));
        }else{
            setParent(nullptr);
            setWindowFlags(defaultFlags);
            m_ui.pbPin->setIcon(QIcon(":/icons/Pin"));
        }
        pinned = !pinned;
        show();
    });

    // move config
    connect(m_ui.tabConfigs->tabBar(), &QTabBar::tabMoved, this, [&](int from, int to){
        if(m_isUpdating){
            return;
        }
        emit GSignals::get()->move_config_signal(componentKey, RowId{from}, RowId{to});
    });

    // select config
    connect(m_ui.tabConfigs->tabBar(), &QTabBar::currentChanged, this, [&](int id){
        if(m_isUpdating){
            return;
        }
        m_ui.cbSelectedConfig->blockSignals(true);
        m_ui.cbSelectedConfig->setCurrentIndex(id);
        m_ui.cbSelectedConfig->blockSignals(false);
        emit GSignals::get()->select_config_signal(componentKey, RowId{id});
    });
    connect(m_ui.cbSelectedConfig, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [&](int id){
        if(m_isUpdating){
            return;
        }
        m_ui.tabConfigs->blockSignals(true);
        m_ui.tabConfigs->setCurrentIndex(id);
        m_ui.tabConfigs->blockSignals(false);
        emit GSignals::get()->select_config_signal(componentKey, RowId{id});
    });

    // show help
    connect(m_ui.pbHelp, &QPushButton::clicked, this, [=]{
        emit GSignals::get()->display_component_help_window_signal(componentType);
    });

    // dialogs
    connect(m_ui.pbInsertNewConfig, &QPushButton::clicked, this, &ComponentConfigDialog::show_insert_new_config_dialog);
    connect(m_ui.pbDuplicate,       &QPushButton::clicked, this, &ComponentConfigDialog::show_duplicate_config_dialog);
    connect(m_ui.pbRenameConfig,    &QPushButton::clicked, this, &ComponentConfigDialog::show_rename_config_dialog);
    connect(m_ui.pbReset,           &QPushButton::clicked, this, &ComponentConfigDialog::show_reset_config_dialog);

}


void ComponentConfigDialog::update_from_component(Component *component){

    m_isUpdating = true;

    Bench::start("[ComponentsConfigDialog 0]"sv);

    get_init_config_widget()->update_from_config(component->initConfig.get());

    Bench::stop();
    Bench::start("[ComponentsConfigDialog 1]"sv);

    // remove inused configs
    for(int ii = m_ui.tabConfigs->count()-1; ii >= 0; --ii){

        auto configW = get_config_widget(ii);
        bool found = false;
        for(const auto &config : component->configs){
            if(config->key() == configW->configKey.v){
                found = true;
                configW->update_from_config(config.get());
                break;
            }
        }
        if(!found){
            m_ui.tabConfigs->removeTab(ii);
        }
    }


    Bench::stop();
    Bench::start("[ComponentsConfigDialog 2]"sv);

    // reorder
    for(int ii = 0; ii < to_signed(component->configs.size()); ++ii){
        for(int jj = 0; jj < m_ui.tabConfigs->count(); ++jj){

            auto cw = get_config_widget(jj);
            if(cw->configKey.v == component->configs[to_unsigned(ii)]->key()){
                if(ii != jj){
                    m_ui.tabConfigs->tabBar()->moveTab(jj,ii);
                }
                break;
            }
        }
    }

    Bench::stop();

    // add new configs
    for(size_t ii = 0; ii < component->configs.size(); ++ii){
        bool found = false;
        for(int jj = 0; jj < m_ui.tabConfigs->count(); ++jj){
            if(component->configs[ii]->key() == get_config_widget(jj)->configKey.v){
                found = true;
                break;
            }
        }

        if(!found){ // insert tab for the new config
            Config *config = component->configs[ii].get();
            m_ui.tabConfigs->insertTab(to_signed(ii), new ConfigW(config, component, false, config->args), config->name);
        }
    }

    // update tab textes with configs name
    QStringList items;
    for(size_t ii = 0; ii < component->configs.size(); ++ii){
        m_ui.tabConfigs->setTabText(to_signed(ii), component->configs[ii]->name);
        items << component->configs[ii]->name;
    }
    m_ui.pbRemoveConfig->setEnabled(m_ui.tabConfigs->count() > 1);

    // update current config selection
    m_ui.tabConfigs->setCurrentIndex(component->selectedConfigId.v);

    // update config combobox
    m_ui.cbSelectedConfig->clear();
    m_ui.cbSelectedConfig->addItems(items);
    m_ui.cbSelectedConfig->setCurrentIndex(component->selectedConfigId.v);

    m_isUpdating = false;
}

void ComponentConfigDialog::update_with_info(ConfigKey configKey, QStringView id, QStringView value){

    auto initConfigW = get_init_config_widget();
    if(initConfigW->configKey.v == configKey.v){
        initConfigW->update_with_info(id, value);
    }else{        
        for(int ii = 0; ii < m_ui.tabConfigs->count(); ++ii){
            auto configW = get_config_widget(ii);
            if(configW->configKey.v == configKey.v){
                configW->update_with_info(id, value);
                break;
            }
        }
    }
}

void ComponentConfigDialog::show_insert_new_config_dialog(){

    m_modalDialog = std::make_unique<QDialog>();
    m_modalDialog->setWindowTitle(QSL("Add new config"));
    m_modalDialog->setModal(true);
    m_modalDialog->setLayout(new QHBoxLayout());
    m_modalDialog->layout()->addWidget(new QLabel(QSL("Name: ")));
    m_modalDialog->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);
    m_modalDialog->layout()->setSizeConstraint( QLayout::SetFixedSize );

    QLineEdit *le = new QLineEdit(QSL("Config ") % QString::number(m_ui.tabConfigs->count()));
    m_modalDialog->layout()->addWidget(le);
    QPushButton *pbV = new QPushButton(QSL("Ok"));
    QPushButton *pbC = new QPushButton(QSL("Cancel"));
    connect(pbV, &QPushButton::clicked, this, [&]{
        m_modalDialog->done(1);
    });
    connect(pbC, &QPushButton::clicked, this, [&]{
        m_modalDialog->done(0);
    });
    m_modalDialog->layout()->addWidget(pbV);
    m_modalDialog->layout()->addWidget(pbC);
    le->setFocus();

    connect(m_modalDialog.get(), &QDialog::finished, [&, le](int ret){

        if(ret == 1){ // if config validated
            int id = m_ui.tabConfigs->currentIndex();
            if(id == -1){
                id = 0;
            }
            emit GSignals::get()->insert_config_signal(componentKey, RowId{id}, le->text());
        }
        m_modalDialog = nullptr;
    });

    m_modalDialog->open();
}

void ComponentConfigDialog::show_duplicate_config_dialog(){

    m_modalDialog = std::make_unique<QDialog>();
    m_modalDialog->setWindowTitle(QSL("Duplicate selected config"));
    m_modalDialog->setModal(true);
    m_modalDialog->setLayout(new QHBoxLayout());
    m_modalDialog->layout()->addWidget(new QLabel(QSL("Name: ")));
    m_modalDialog->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);
    m_modalDialog->layout()->setSizeConstraint( QLayout::SetFixedSize );

    QLineEdit *le = new QLineEdit(m_ui.tabConfigs->tabText(m_ui.tabConfigs->currentIndex()) % QSL("(copy)"));
    m_modalDialog->layout()->addWidget(le);
    QPushButton *pbV = new QPushButton(QSL("Ok"));
    QPushButton *pbC = new QPushButton(QSL("Cancel"));
    connect(pbV, &QPushButton::clicked, this, [&]{
        m_modalDialog->done(1);
    });
    connect(pbC, &QPushButton::clicked, this, [&]{
        m_modalDialog->done(0);
    });
    m_modalDialog->layout()->addWidget(pbV);
    m_modalDialog->layout()->addWidget(pbC);
    le->setFocus();

    connect(m_modalDialog.get(), &QDialog::finished, [&, le](int ret){

        if(ret == 1){ // if config validated
            int id = m_ui.tabConfigs->currentIndex();
            if(id == -1){
                id = 0;
            }
            emit GSignals::get()->copy_config_signal(componentKey, RowId{id}, le->text());
        }
        m_modalDialog = nullptr;
    });

    m_modalDialog->open();
}

void ComponentConfigDialog::show_rename_config_dialog(){

    m_modalDialog = std::make_unique<QDialog>();
    m_modalDialog->setWindowTitle(QSL("Rename config"));
    m_modalDialog->setModal(true);
    m_modalDialog->setLayout(new QHBoxLayout());
    m_modalDialog->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);
    m_modalDialog->layout()->setSizeConstraint( QLayout::SetFixedSize );

    m_modalDialog->layout()->addWidget(new QLabel(QSL("Name: ")));
    QLineEdit *le = new QLineEdit(m_ui.tabConfigs->tabText(m_ui.tabConfigs->currentIndex()));
    m_modalDialog->layout()->addWidget(le);
    QPushButton *pbV = new QPushButton(QSL("Ok"));
    QPushButton *pbC = new QPushButton(QSL("Cancel"));
    connect(pbV, &QPushButton::clicked, this, [&]{
        m_modalDialog->done(1);
    });
    connect(pbC, &QPushButton::clicked, this, [&]{
        m_modalDialog->done(0);
    });
    m_modalDialog->layout()->addWidget(pbV);
    m_modalDialog->layout()->addWidget(pbC);
    le->setFocus();

    connect(m_modalDialog.get(), &QDialog::finished, [&, le](int ret){

        if(ret == 1){ // if config validated
            int id = m_ui.tabConfigs->currentIndex();
            if(id == -1){
                id = 0;
            }
            emit GSignals::get()->rename_config_signal(componentKey, RowId{id}, le->text());
        }
        m_modalDialog = nullptr;
    });

    m_modalDialog->open();
}

void ComponentConfigDialog::show_reset_config_dialog(){

    const bool resetInitConfig  = m_ui.twAllConfigs->currentIndex() == 0;
    const int idCurrentConfig   = m_ui.tabConfigs->currentIndex();
    ConfigW *configW = nullptr;
    if(resetInitConfig){
        configW = get_init_config_widget();
    }else{
        configW = get_config_widget(idCurrentConfig);
    }

    m_modalDialog = std::make_unique<QDialog>();
    m_modalDialog->setWindowTitle(QSL("Reset config"));
    m_modalDialog->setModal(true);
    m_modalDialog->setLayout(new QHBoxLayout());
    m_modalDialog->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);
    m_modalDialog->layout()->setSizeConstraint( QLayout::SetFixedSize );

    m_modalDialog->layout()->addWidget(new QLabel(QSL("You are going to reset ") %
            QString(resetInitConfig ? QSL("init") : QSL("current selected")) % QSL(" config values.")));

    QPushButton *pbV = new QPushButton(QSL("Ok"));
    QPushButton *pbC = new QPushButton(QSL("Cancel"));
    connect(pbV, &QPushButton::clicked, this, [&]{
        m_modalDialog->done(1);
    });
    connect(pbC, &QPushButton::clicked, this, [&]{
        m_modalDialog->done(0);
    });
    m_modalDialog->layout()->addWidget(pbV);
    m_modalDialog->layout()->addWidget(pbC);

    connect(m_modalDialog.get(), &QDialog::finished, [&,configW](int ret){
        if(ret == 1){ // if validated
            configW->reset_args();
        }
        m_modalDialog = nullptr;
    });

    m_modalDialog->open();
}


#include "moc_component_config_dialog.cpp"
