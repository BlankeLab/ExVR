
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
#include <optional>

// Qt
#include <QStringList>
#include <QStringBuilder>

// base
#include "utility/types.hpp"
#include "utility/vector.hpp"
#include "utility/tuple_array.hpp"

// qt-utility
#include "data/id_key.hpp"
#include "qt_str.hpp"

namespace tool::ex {

using namespace std::literals::string_view_literals;

struct Resource{

    enum class Type : std::uint8_t {
        Image, Video, Audio, Text, AssetBundle, Mesh, PythonScript, CSharpScript, Cloud, ScanerVideo, Plot, Directory,
        VolumetricVideo,
        SizeEnum
    };

    using Filters     = std::string_view;
    using Name        = std::string_view;
    using IconPath    = std::string_view;
    using DisplayName = std::string_view;
    using CanOpen     = bool;
    using CanGenerate = bool;
    static constexpr Filters videoFilters   = "(*.avi *.dv *.m4v *.mov *.mp4 *.mpg *.mpeg *.ogv *.webm *.wmv)"sv;
    static constexpr Filters txtFilters     = "(*.txt *.calib *.config *.xml *.json *.csv)"sv;
    static constexpr Filters audioFilters   = "(*.ogg *.wav *.aiff *.aif)"sv;
    static constexpr Filters imageFilters   = "(*.jpg *.jpeg *.png *.PNG)"sv;

    using TResource = std::tuple<
        Type,                  Filters,          Name,                 IconPath,                       DisplayName,              CanOpen, CanGenerate>;
    static constexpr TupleArray<Type::SizeEnum,TResource> resources{{
        TResource
        {Type::Image,          imageFilters,     "Image"sv,            ":/icons/Image"sv,              "Images"sv,               true,    false},
        {Type::Video,          videoFilters,     "Video"sv,            ":/icons/Video"sv,              "Videos"sv,               true,    false},
        {Type::Audio,          audioFilters,     "Audio"sv,            ":/icons/Sound"sv,              "Audio"sv,                true,    false},
        {Type::Text,           txtFilters,       "Text"sv,             ":/icons/Text"sv,               "Textes"sv,               true,    false},
        {Type::AssetBundle,    "(*.unity3d)"sv,  "UnityAssetBundle"sv, ":/icons/Unity_scene_bundle"sv, "Unity assets bundles"sv, false,   false},
        {Type::Mesh,           "(*.obj)"sv,      "Mesh"sv,             ":/icons/Cube"sv,               "Meshes"sv,               true,    false},
        {Type::PythonScript,   "(*.py)"sv,       "PythonScript"sv,     ":/icons/Python_script"sv,      "Python scripts"sv,       true,    true},
        {Type::CSharpScript,   "(*.cs)"sv,       "CSharpScript"sv,     ":/icons/CSharp"sv,             "C# scripts"sv,           true,    true},
        {Type::Cloud,          "(*.asc)"sv,      "Cloud"sv,            ":/icons/Sphere"sv,             "Clouds points"sv,        false,   false},
        {Type::ScanerVideo,    "(*.kvid)"sv,     "ScanerVideo"sv,      ":/icons/Video"sv,              "Scaner videos"sv,        false,   false},
        {Type::Plot,           "(*.csv)"sv,      "Plot"sv,             ":/icons/Plot"sv,               "Plots"sv,                true,    false},
        {Type::Directory,      ""sv,             "Directory"sv,        ":/icons/Folder"sv,             "Directories"sv,          true,    false},
        {Type::VolumetricVideo,"(*.kvid)"sv,     "VolumetricVideo"sv,  ":/icons/Video_cloud"sv,        "Volumetric video"sv,     false,    false},
    }};

    static auto get_types() {
        return resources.tuple_column<0>();
    }

    static auto get_names() {
        return resources.tuple_column<2>();
    }

    [[maybe_unused]] static std::optional<Type> get_type(const Name name) {
        return resources.optional_at<2,0>(name);
    }

    static constexpr Filters get_filters(Type t) {
        return resources.at<0,1>(t);
    }

    static constexpr Name get_name(Type t) {
        return resources.at<0,2>(t);
    }

    static constexpr IconPath get_icon_path(Type t) {
        return resources.at<0,3>(t);
    }

    static constexpr DisplayName get_display_name(Type t) {
        return resources.at<0,4>(t);
    }

    static constexpr bool can_open(Type t) {
        return resources.at<0,5>(t);
    }

    static constexpr bool can_generate(Type t) {
        return resources.at<0,6>(t);
    }

    Resource() = delete;
    Resource(Type t, ResourceKey id, QString path, QString alias = "") : type(t), m_key(IdKey::Type::Resource, id.v){

        this->path  = path;
        this->alias = alias;

        // separate path using .
        const auto split = this->path.split("/").last().split(".");

        // update alias from path is empty
        if(this->alias.length() == 0){
            this->alias = split.first();
        }

        // update extension
        if(split.size() > 1){
            extension = split.last();
        }else{
            extension = ""; // no extension or directory
        }
    }
    Resource(const Resource &) = delete;
    Resource& operator=(const Resource&) = delete;

    static std::unique_ptr<Resource> copy_with_new_element_id(Resource *resourceToCopy){
        auto resource = std::make_unique<Resource>(
            resourceToCopy->type,
            ResourceKey{-1},
            resourceToCopy->path,
            resourceToCopy->alias
        );

        resource->preview   = resourceToCopy->preview;
        resource->exist     = resourceToCopy->exist;
        resource->extension = resourceToCopy->extension;

        return resource;
    }

    inline QString display_name() const noexcept{
        if(extension.length() == 0){
            return alias;
        }else{
            return QSL("[") % extension % QSL("] ") % alias;
        }
    }

    constexpr int key() const noexcept{ return m_key();}
    constexpr ResourceKey r_key() const noexcept {return ResourceKey{key()};}

    Type type;
    bool exist = false;
    bool preview = false;
    QString alias;
    QString path;
    QString extension;

private:

    IdKey m_key;
};
}
