import qbs.base 1.0

Project {
    name: "Mod Organizer"

    cpp.warningLevel: "all"
    cpp.optimization: "fast"
    cpp.debugInformation: true
    qbs.architecture: 'x86_64'

    nsis.compressor: "lzma-solid"

    references: [ "organizer/organizer.qbs", "nxmhandler/nxmhandler.qbs", "uibase/uibase.qbs", "shared/shared.qbs",
                  "bsatk/bsatk.qbs", "hookdll/hookdll.qbs" ]
}
