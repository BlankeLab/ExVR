
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
#include <set>

// Qt
#include <QStringList>
#include <QDebug>

// base
#include "utility/vector.hpp"

// qt-utility
#include "qt_logger.hpp"

// local
#include "resource.hpp"


namespace tool::ex {

[[maybe_unused]] constexpr static int reloadAudioCode          = 0b1;
[[maybe_unused]] constexpr static int reloadImagesCode         = 0b10;
[[maybe_unused]] constexpr static int reloadMeshesCode         = 0b100;
[[maybe_unused]] constexpr static int reloadTextesCode         = 0b1000;
[[maybe_unused]] constexpr static int reloadVideosCode         = 0b10000;
[[maybe_unused]] constexpr static int reloadAssetBundlesCode   = 0b100000;
[[maybe_unused]] constexpr static int reloadCSharpScriptsCode  = 0b1000000;
[[maybe_unused]] constexpr static int reloadPythonScriptsCode  = 0b10000000;
[[maybe_unused]] constexpr static int reloadCloudsCode         = 0b100000000;
[[maybe_unused]] constexpr static int reloadScanerVideosCode   = 0b1000000000;
[[maybe_unused]] constexpr static int reloadPlotsCode          = 0b10000000000;
[[maybe_unused]] constexpr static int reloadVolumetricVideoCode= 0b100000000000;
[[maybe_unused]] //constexpr static int reloadDirectories         = 0b100000000000;

[[maybe_unused]] constexpr static int reloadDefaultCode = reloadTextesCode | reloadCSharpScriptsCode | reloadPythonScriptsCode;


class ResourcesManager{

public:

    std::vector<Resource*> get_resources(Resource::Type type) const;
    size_t get_type_selected_id(Resource::Type type) const;
    Resource* get_resource(ResourceKey key) const;
    Resource* get_resource(Resource::Type type, ResourceKey key, bool errorIfNotFound = true) const;
    Resource* get_resource(Resource::Type type, const QString &alias, bool errorIfNotFound = true) const;

    void add_resource(std::unique_ptr<Resource> resourceToAdd);
    void add_resource(Resource::Type type, const QString &path);
    void add_resources(Resource::Type type, const QStringList &filesPaths);
    void copy_resource(Resource *resource);

    void update_resource_path(QString currentPath, QString newPath);
    void update_resource_alias(QString currentAlias, QString newAlias);

    void select_resource(Resource::Type type, size_t index);

    void clean_resources(Resource::Type type);
    void clean_resources();
    void remove_resource(Resource::Type type, size_t index);

    void set_reload_code(int code);
    int reload_code()const;

    bool exportMode = false;

    void export_to(QString path);

private:

    void insert_resource(std::unique_ptr<Resource> resource);

    int m_reloadCode = 0;
    std::unordered_map<int, std::unique_ptr<Resource>> m_resources;
    std::unordered_map<Resource::Type, size_t> m_idSelectedPerType;
    std::unordered_map<std::string, Resource*> m_paths;
    std::unordered_map<std::string, Resource*> m_aliases;
    std::unordered_map<Resource::Type, std::vector<Resource*>> m_resourcesPerType;
};
}
