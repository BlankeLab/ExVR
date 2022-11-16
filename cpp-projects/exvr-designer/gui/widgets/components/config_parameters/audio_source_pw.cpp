
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

#include "audio_source_pw.hpp"

// qt-utility
#include "gui/ex_widgets/ex_checkbox_w.hpp"
#include "gui/ex_widgets/ex_combo_box_index_w.hpp"
#include "gui/ex_widgets/ex_text_edit_w.hpp"
#include "gui/ex_widgets/ex_line_edit_w.hpp"
#include "gui/ex_widgets/ex_slider_w.hpp"
#include "gui/ex_widgets/ex_radio_button_w.hpp"

// local
#include "gui/ex_widgets/ex_resource_w.hpp"

using namespace tool::ex;

struct AudioSourceInitConfigParametersW::Impl{
    ExResourceW sound = {"sound"};
    ExResourceW assetBundle = {"asset_bundle"};
    ExCheckBoxW generateNewSound = {"generate_new_sound"};
    ExComboBoxIndexW newSoundChannels = {"new_sound_channel"};
    ExTextEditW infoText;
    std_v1<std::unique_ptr<ExLineEditW>> channelsToCopy;
};

AudioSourceInitConfigParametersW::AudioSourceInitConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void AudioSourceInitConfigParametersW::insert_widgets(){

    // top
    add_widget(ui::F::gen(ui::L::VB(), {m_p->sound(), m_p->assetBundle()}, LStretch{false}, LMargins{true}, QFrame::Box));

    for(int ii = 0; ii < 8; ++ ii){
        m_p->channelsToCopy.emplace_back(std::make_unique<ExLineEditW>("channel_" + QString::number(ii) + "_copy_destination"));
    }

    // infos
    add_widget(ui::F::gen(ui::L::VB(), {ui::W::txt("Input file infos"), m_p->infoText()}, LStretch{true}, LMargins{true}, QFrame::Box));
    m_p->infoText.w->setMinimumHeight(100);

    // new sound
    std_v1<QWidget*> channelsW;
    for(size_t ii = 0; ii < 8; ++ii){
        auto layout = ui::L::HB();
        layout->addWidget(ui::W::txt("Copy channel " + QString::number(ii+1) + " to channel: "));
        layout->addWidget(m_p->channelsToCopy[ii]->w.get());
        layout->setContentsMargins(0,0,0,0);

        auto w = new QWidget();
        w->setLayout(layout);
        channelsW.emplace_back(w);
    }

    auto outputChannelsNb = ui::F::gen(ui::L::HB(),{ui::W::txt("Number channels:"), m_p->newSoundChannels()}, LStretch{true}, LMargins{false});
    add_widget(ui::F::gen(ui::L::VB(), {
        m_p->generateNewSound(), outputChannelsNb,
        channelsW[0],channelsW[1],channelsW[2],channelsW[3],
        channelsW[4],channelsW[5],channelsW[6],channelsW[7]},
        LStretch{false}, LMargins{true}, QFrame::Box)
    );
}

void AudioSourceInitConfigParametersW::init_and_register_widgets(){

    add_input_ui(m_p->sound.init_widget(Resource::Type::Audio, "Audio resource: "));
    add_input_ui(m_p->assetBundle.init_widget(Resource::Type::AssetBundle, "Audio asset bundle (mandatory for ambisonic): "));
    add_input_ui(m_p->generateNewSound.init_widget("Generate new sound", false));
    add_input_ui(m_p->newSoundChannels.init_widget({"1", "2", "3", "4", "5", "6", "7", "8"}, 0, true));

    for(size_t ii = 0; ii < m_p->channelsToCopy.size(); ++ii){
        add_input_ui(m_p->channelsToCopy[ii]->init_widget({}, true));
    }
    m_p->channelsToCopy[0]->w->setText("1 2");
}

void AudioSourceInitConfigParametersW::create_connections(){
}

void AudioSourceInitConfigParametersW::late_update_ui(){}

void AudioSourceInitConfigParametersW::update_with_info(QStringView id, QStringView value){

    if(id == QSL("input_sound_info")){
        if(auto infos = value.split('?'); infos.size() > 5){
            m_p->infoText.w->setText(
                QSL("Name: ") % infos[0] %
                QSL("\nChannels:") % infos[1] %
                QSL("\nFrequency: ") % infos[2] %
                QSL("\nLength: ") % infos[3] %
                QSL("s Samples: ") % infos[4] %
                QSL("\nAmbisonic: ") % infos[5]);
        }
    }
}

struct AudioSourceConfigParametersW::Impl{

    ExCheckBoxW displaySoundOrigin = {"display"};

    QButtonGroup group1;
    ExRadioButtonW playNewBlock     = {"play_new_block"};
    ExRadioButtonW stopNewBlock     = {"stop_new_block"};
    ExRadioButtonW pauseNewBlock    = {"pause_new_block"};
    ExRadioButtonW nothingNewBlock  = {"nothing_new_block"};

    QButtonGroup group2;
    ExRadioButtonW playEndBlock     = {"play_end_block"};
    ExRadioButtonW pauseEndBlock    = {"pause_end_block"};
    ExRadioButtonW stopEndBlock     = {"stop_end_block"};
    ExRadioButtonW nothingEndBlock  = {"nothing_end_block"};

    ExCheckBoxW doMute              = {"mute"};
    ExCheckBoxW doLoop              = {"loop"};
    ExSliderFloatW volume           = {"volume"};
    ExSliderFloatW stereo           = {"stereo"};
    ExSliderFloatW pitch            = {"pitch"};

    ExCheckBoxW isSpatialized       = {"spatialized"};
    ExSliderFloatW spatialBlend     = {"spatial_blend"};
    ExSliderFloatW dopplerLevel     = {"doppler_level"}; // 0 - 5
    ExSliderIntegerW spread         = {"spread"}; // 0 - 360
    ExComboBoxIndexW rolloffMode    = {"rollof_mode"};
    ExSliderFloatW minDistance      = {"min_distance"};
    ExSliderFloatW maxDistance      = {"max_distance"};

    TransformSubPart transfo        = {"transform"};

};

AudioSourceConfigParametersW::AudioSourceConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void AudioSourceConfigParametersW::insert_widgets(){

    m_p = std::make_unique<Impl>();

    add_sub_part_widget(m_p->transfo);
    add_widget(ui::F::gen(ui::L::VB(), {
        ui::W::txt("What to do when update block starts:"),
        ui::F::gen(ui::L::HB(),{m_p->playNewBlock(),m_p->pauseNewBlock(), m_p->stopNewBlock(),m_p->nothingNewBlock()}, LStretch{true}, LMargins{true}),
        ui::W::txt("What to do when update block ends:"),
        ui::F::gen(ui::L::HB(),{m_p->playEndBlock(),m_p->pauseEndBlock(),m_p->stopEndBlock(),m_p->nothingEndBlock()}, LStretch{true}, LMargins{true})
        }, LStretch{false}, LMargins{true}, QFrame::Box)
    );

    {
        auto gl = new QGridLayout();

        int rowId = 0;

        gl->addWidget(ui::W::txt("<b>Global</b>"), rowId,   0, 2, 1);
        rowId+=2;
        gl->addWidget(ui::F::gen(ui::L::HB(), {m_p->doMute(), m_p->doLoop()}, LStretch{true}, LMargins{false}, QFrame::NoFrame),rowId++, 0, 1, 6);

        gl->addWidget(m_p->volume.title,    rowId, 0, 1, 1);
        gl->addWidget(m_p->volume.minMax,   rowId, 1, 1, 1);
        gl->addWidget(m_p->volume.value,    rowId, 2, 1, 1);
        gl->addWidget(m_p->volume.valueTxt, rowId, 3, 1, 1);
        gl->addWidget(m_p->volume.less,     rowId, 4, 1, 1);
        gl->addWidget(m_p->volume.more,     rowId++, 5, 1, 1);

        gl->addWidget(m_p->pitch.title,    rowId, 0, 1, 1);
        gl->addWidget(m_p->pitch.minMax,   rowId, 1, 1, 1);
        gl->addWidget(m_p->pitch.value,    rowId, 2, 1, 1);
        gl->addWidget(m_p->pitch.valueTxt, rowId, 3, 1, 1);
        gl->addWidget(m_p->pitch.less,     rowId, 4, 1, 1);
        gl->addWidget(m_p->pitch.more,     rowId++, 5, 1, 1);

        gl->addWidget(m_p->stereo.title,    rowId, 0, 1, 1);
        gl->addWidget(m_p->stereo.minMax,   rowId, 1, 1, 1);
        gl->addWidget(m_p->stereo.value,    rowId, 2, 1, 1);
        gl->addWidget(m_p->stereo.valueTxt, rowId, 3, 1, 1);
        gl->addWidget(m_p->stereo.less,     rowId, 4, 1, 1);
        gl->addWidget(m_p->stereo.more,     rowId++, 5, 1, 1);

        gl->addWidget(ui::W::txt("<b>3D sound</b>"), rowId,   0, 2, 1);
        rowId+=2;
        gl->addWidget(ui::F::gen(ui::L::HB(), {m_p->isSpatialized(), m_p->displaySoundOrigin()}, LStretch{true}, LMargins{false}, QFrame::NoFrame),rowId++, 0, 1, 6);

        gl->addWidget(m_p->spatialBlend.title,    rowId, 0, 1, 1);
        gl->addWidget(m_p->spatialBlend.minMax,   rowId, 1, 1, 1);
        gl->addWidget(m_p->spatialBlend.value,    rowId, 2, 1, 1);
        gl->addWidget(m_p->spatialBlend.valueTxt, rowId, 3, 1, 1);
        gl->addWidget(m_p->spatialBlend.less,     rowId, 4, 1, 1);
        gl->addWidget(m_p->spatialBlend.more,     rowId++, 5, 1, 1);

        gl->addWidget(m_p->dopplerLevel.title,    rowId, 0, 1, 1);
        gl->addWidget(m_p->dopplerLevel.minMax,   rowId, 1, 1, 1);
        gl->addWidget(m_p->dopplerLevel.value,    rowId, 2, 1, 1);
        gl->addWidget(m_p->dopplerLevel.valueTxt, rowId, 3, 1, 1);
        gl->addWidget(m_p->dopplerLevel.less,     rowId, 4, 1, 1);
        gl->addWidget(m_p->dopplerLevel.more,     rowId++, 5, 1, 1);

        gl->addWidget(m_p->spread.title,    rowId, 0, 1, 1);
        gl->addWidget(m_p->spread.minMax,   rowId, 1, 1, 1);
        gl->addWidget(m_p->spread.value,    rowId, 2, 1, 1);
        gl->addWidget(m_p->spread.valueTxt, rowId, 3, 1, 1);
        gl->addWidget(m_p->spread.less,     rowId, 4, 1, 1);
        gl->addWidget(m_p->spread.more,     rowId++, 5, 1, 1);

        gl->addWidget(ui::W::txt("Rollof mode"), rowId,   0, 1, 1);
        gl->addWidget(m_p->rolloffMode(),        rowId++, 2, 1, 1);

        gl->addWidget(m_p->minDistance.title,    rowId, 0, 1, 1);
        gl->addWidget(m_p->minDistance.minMax,   rowId, 1, 1, 1);
        gl->addWidget(m_p->minDistance.value,    rowId, 2, 1, 1);
        gl->addWidget(m_p->minDistance.valueTxt, rowId, 3, 1, 1);
        gl->addWidget(m_p->minDistance.less,     rowId, 4, 1, 1);
        gl->addWidget(m_p->minDistance.more,     rowId++, 5, 1, 1);

        gl->addWidget(m_p->maxDistance.title,    rowId, 0, 1, 1);
        gl->addWidget(m_p->maxDistance.minMax,   rowId, 1, 1, 1);
        gl->addWidget(m_p->maxDistance.value,    rowId, 2, 1, 1);
        gl->addWidget(m_p->maxDistance.valueTxt, rowId, 3, 1, 1);
        gl->addWidget(m_p->maxDistance.less,     rowId, 4, 1, 1);
        gl->addWidget(m_p->maxDistance.more,     rowId++, 5, 1, 1);

        add_widget(ui::F::gen_frame(gl, {}, 0, LMarginsD{4,2,4,2,2}, QFrame::Shape::Box));
    }
}

void AudioSourceConfigParametersW::init_and_register_widgets(){


    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(m_p->group1,
            {&m_p->playNewBlock, &m_p->pauseNewBlock, &m_p->stopNewBlock, &m_p->nothingNewBlock},
            {"Play","Pause","Stop", "Nothing"},
            {true, false, false, false}
        )
    );

    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(m_p->group2,
            {&m_p->playEndBlock,&m_p->pauseEndBlock, &m_p->stopEndBlock, &m_p->nothingEndBlock},
            {"Play","Pause","Stop", "Nothing"},
            {false, false, true, false}
        )
    );


    add_input_ui(m_p->displaySoundOrigin.init_widget("Display sound origin", false));
    add_input_ui(m_p->doLoop.init_widget("Loop", true));
    add_input_ui(m_p->doMute.init_widget("Mute", false));
    add_input_ui(m_p->isSpatialized.init_widget("Spatialized", true));
    add_input_ui(m_p->volume.init_widget("Volume", MinV<qreal>{0.}, V<qreal>{1.}, MaxV<qreal>{1.}, StepV<qreal>{0.01}));
    add_input_ui(m_p->stereo.init_widget("Stereo", MinV<qreal>{-1.}, V<qreal>{0.0}, MaxV<qreal>{1.}, StepV<qreal>{0.01}));
    add_input_ui(m_p->spatialBlend.init_widget("Spatial blend", MinV<qreal>{0.}, V<qreal>{1.}, MaxV<qreal>{1.}, StepV<qreal>{0.01}));
    add_input_ui(m_p->pitch.init_widget("Pitch", MinV<qreal>{-3.}, V<qreal>{1.}, MaxV<qreal>{3.}, StepV<qreal>{0.01}));
    add_input_ui(m_p->dopplerLevel.init_widget("Doppler level", MinV<qreal>{0.}, V<qreal>{1.}, MaxV<qreal>{5.}, StepV<qreal>{0.01}));
    add_input_ui(m_p->spread.init_widget("Spread", MinV<int>{0}, V<int>{0}, MaxV<int>{360}, StepV<int>{1}));
    add_input_ui(m_p->rolloffMode.init_widget({"Logarithmic", "Linear"}, 1));
    add_input_ui(m_p->minDistance.init_widget("Min distance", MinV<qreal>{0.}, V<qreal>{0.01}, MaxV<qreal>{10.}, StepV<qreal>{0.01}));
    add_input_ui(m_p->maxDistance.init_widget("Max distance", MinV<qreal>{0.}, V<qreal>{5.}, MaxV<qreal>{10.}, StepV<qreal>{0.01}));
    map_sub_part(m_p->transfo.init_widget());
    m_p->transfo.set_scale_values({0.3,0.3,0.3});

    m_p->dopplerLevel.set_information("Sets the Doppler scale for this AudioSource.");
    m_p->spread.set_information(
        "Sets the spread angle (in degrees) of a 3d stereo or multichannel sound in speaker space.\n"
        "   0 = all sound channels are located at the same speaker location and is 'mono'\n"
        "   360 = all subchannels are located at the opposite speaker location to the speaker location that it should be according to 3D position. Default = 0."
    );
    m_p->rolloffMode.set_information(
        "Sets/Gets how the AudioSource attenuates over distance.\n"
        "   Logarithmic: Use this mode when you want a real-world rolloff.\n"
        "   Linear: Use this mode when you want to lower the volume of your sound over the distance."
    );
    m_p->minDistance.set_information("Within the Min distance the AudioSource will cease to grow louder in volume.");
    m_p->maxDistance.set_information("(Logarithmic rolloff) MaxDistance is the distance a sound stops attenuating at.");
}

void AudioSourceConfigParametersW::create_connections(){}

void AudioSourceConfigParametersW::late_update_ui(){}
