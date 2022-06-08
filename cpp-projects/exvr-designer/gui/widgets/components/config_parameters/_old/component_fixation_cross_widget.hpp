

#pragma once

// expy
#include "component_widget.hpp"


namespace expy {


//class FixationCrossComponentParametersW : public ComponentParametersW{

//    Q_OBJECT

//public:

//    FixationCrossComponentParametersW(const QString &name) : ComponentParametersW(name){

//        Ui::FixationCrossComponentAllW uiAll;
//        configsCommonPartW = new CommonPartConfigsW();
//        uiAll.setupUi(configsCommonPartW);
//        basedialogUI.glAllConfigs->addWidget(configsCommonPartW);

//        // add default configuration
//        add_config_tab(generate_config_widget(), "Default");
//    }

////    QWidget *generate_all_configs_widget();

//    ConfigW *generate_config_widget(){
//        Ui::FixationCrossComponentW ui;
//        ConfigW *w = new ConfigW();
//        ui.setupUi(w);
//        return w;
//    }


//    static int count;
//};


//class FixationCrossComponentW : public ComponentW{

//    Q_OBJECT

//public :

//    FixationCrossComponentW(bool allComponents) {
//        type = ComponentType::Fixation_cross;
//        name = type_2_string(type) + (allComponents ? "" : (" " + QString::number(count()++)));
//        icon = QIcon(QPixmap(":/icons/fixation_cross"));
//        m_parameters = std::make_unique<FixationCrossComponentParametersW>(name);
//        initialize(allComponents);
//    }

//private:
//    static int& count() { static int nb{0}; return nb; }
//};

}
