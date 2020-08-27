#include "builder.h"

#define VERSION_FMW(Major, Minor, Patch, Build) \
    (((Major) << 24) | ((Minor) << 16) | ((Patch) << 8) | (Build))

BUILDER::BUILDER(uint32_t major, uint32_t minor, uint32_t patch, uint32_t build) {
    this->version = VERSION_FMW(major, minor, patch, build);
}

BUILDER::~BUILDER() {

}

uint32_t BUILDER::versionToNumber() {
    return this->version;
}

char* BUILDER::versionToString() {
    static char templateStr[] = "m.mi.ph";

    // In ASCII Code
    //templateStr[0]  = 0x30 + (((this->version >> 24) & 0xFF) / 10);
    templateStr[0]  = 0x30 + (((this->version >> 24) & 0xFF) % 10);

    templateStr[2]  = 0x30 + (((this->version >> 16) & 0xFF) / 10);
    templateStr[3]  = 0x30 + (((this->version >> 16) & 0xFF) % 10);

    templateStr[5]  = 0x30 + (((this->version >> 8) & 0xFF)  / 10);
    templateStr[6]  = 0x30 + (((this->version >> 8) & 0xFF)  % 10);

    return templateStr;
}
