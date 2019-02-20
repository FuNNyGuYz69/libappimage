#pragma once

// system
#include <string>

// libraries
#include <appimage/core/AppImage.h>

// local
#include "PayloadEntriesCache.h"
#include "DesktopIntegrationResources.h"

namespace appimage {
    namespace desktop_integration {
        namespace integrator {
            /**
             * Extract the resources (files) required to integrate an AppImage into the desktop environment
             * in an efficient way.
             */
            class ResourcesExtractor {
            public:
                explicit ResourcesExtractor(const core::AppImage& appImage);

                /**
                 * @brief Extract the desktop integration resources in a reliable way.
                 *
                 * Using the `PayloadIterator::read` method on symlinks is not reliable as it's not supported on
                 * AppImages of type 1 (blame on `libarchive`). To overcome this limitation two iterations over the
                 * AppImage will be performed. One to resolve all the links entries and other to actually extract
                 * the resources.
                 *
                 * @return extracted integration resources
                 * @throw PayloadIteratorError if there is a links cycle, e.g. A links to A who links to C who links to A
                 */
                DesktopIntegrationResources extract();

                void setExtractDesktopFile(bool extractDesktopFile);

                void setExtractIconFiles(bool extractIconFiles);

                void setExtractAppDataFile(bool extractAppDataFile);

                void setExtractMimeFiles(bool extractMimeFiles);

            private:
                bool extractDesktopFile = false;
                bool extractIconFiles = false;
                bool extractAppDataFile = false;
                bool extractMimeFiles = false;

                core::AppImage appImage;

                bool isIconFile(const std::string& fileName) const;

                bool isMainDesktopFile(const std::string& fileName) const;

                bool isAppDataFile(const std::string& filePath) const;

                bool isMimeFile(const std::string& filePath) const;

                std::vector<char> readWholeFile(std::istream& istream);

                /**
                 * Resolve the final paths of the resource entries.
                 *
                 * Resource entries can be links to other entries therefore several iterations will be required
                 * to extract a regular file for the given resource. This method uses the <entriesCache> to
                 * resolve the final paths of the regular entries to allow their extraction in the next iteration.
                 *
                 * @param resources
                 */
                void resolveResourcesFinalPaths(DesktopIntegrationResources& resources, PayloadEntriesCache cache);

                /**
                 * Read the data of the entries listed in <resources>
                 *
                 * @param resources
                 */
                void readResourceFiles(DesktopIntegrationResources& resources);
            };
        }
    }
}
